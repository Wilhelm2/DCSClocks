//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//

#include "Node.h"

Define_Module(Node);

unsigned int Node::idCounter = 0;

void Node::initialize()
{
	cSimpleModule::initialize();
	if (DeliveryControl == Delivery::DCS)
		scheduleAt(SimTime(1, SIMTIME_S), &clockManagment.componentSizeCheckTimer);
	ut = dynamic_cast<SimulationParameters*>(getModuleByPath("DCS.ut"));
	control = dynamic_cast<DeliveryController*>(getModuleByPath("DCS.control"));
	clockManagment.clock = DCS(ut->clockLength);

	setOutGate();
	sendInitMessage();
}

void Node::setOutGate()
{
	cModule::GateIterator i(this);
	while (!i.end() && (*i)->getType() == cGate::INPUT)
		i++;
	outGate = (*i);
}

void Node::sendInitMessage()
{
	Init * m = new Init();
	m->setSourceId(id);
	send(m, outGate);
}

void Node::handleMessage(cMessage *msg)
{
	if (msg == &clockManagment.componentSizeCheckTimer)
		PeriodicComponentCheck();
	else
	{
		if (dynamic_cast<BroadcastNotify*>(msg))
			sendAppMsg();
		else if (AppMsg* m = dynamic_cast<AppMsg*>(msg))
			RecvAppMsg(m);
		else if (AckComponent* m = dynamic_cast<AckComponent*>(msg))
			RecvAckComponent(m);
		else if (AckRep* m = dynamic_cast<AckRep*>(msg))
			RecvAckRep(m);
		else if (AckRoundDecision* m = dynamic_cast<AckRoundDecision*>(msg))
			RecvAckRoundDecision(m);
		else
		{
			std::cerr << "MESSAGE DE TYPE INCONNU REÇU !" << msg << endl;
			exit(1);
		}
		delete msg;
	}
}

void Node::sendAppMsg()
{
	if (DeliveryControl == Delivery::DCS)
	{
		if (clockManagment.expandDecision(nbReceivedLastSecond()))
			clockManagment.expandClock(id);
	}

	seq++;
	clockManagment.IncrementPC(ut->clockEntries[id], clockManagment.incrComponent);
	delivered.push_back(dep(id, seq, simTime(), clockManagment.incrComponent, clockManagment.clock));
	send(createAppMsg(), outGate);

	stat.nbBroadcasts++;
	stat.localActiveComponentsWhenBroadcast += clockManagment.nbLocalActiveComponents;
	stat.controlDataSize += clockManagment.clock.activeComponents * sizeof(ut->clockLength) * sizeof(unsigned int);

	control->notifySendMessage(id, seq);
	control->notifyDeliverMessage( { id, seq }, id);
	assert(clockManagment.clock.activeComponents >= clockManagment.incrComponent); // Verifies that does not increment an inactive component
	clearDelivered();
}

AppMsg* Node::createAppMsg()
{
	AppMsg* m = new AppMsg("broadcast");
	m->setSeq(seq);
	m->setSourceId(id);
	m->setIncrComponent(clockManagment.incrComponent);
	m->setPC(clockManagment.clock);
	return m;
}

void Node::RecvAppMsg(AppMsg*m)
{
	receivedTime.push_back(simTime());

	if (DeliveryControl == Delivery::NOTHING)
		deliverMsg(m->getSourceId(), m->getSeq(), m->getPC(), simTime(), m->getIncrComponent());
	else if (DeliveryControl == Delivery::PCcomparision || DeliveryControl == Delivery::DCS)
	{
		if (PC_test(m->getSourceId(), m->getPC(), m->getIncrComponent()))
		{
			deliverMsg(m->getSourceId(), m->getSeq(), m->getPC(), simTime(), m->getIncrComponent());
			iterativeDelivery();
		}
		else
			pendingMsg.push_back(dep(m->getSourceId(), m->getSeq(), simTime(), m->getIncrComponent(), m->getPC()));
	}
}

bool Node::PC_test(unsigned int idMsg, const DCS &PCMsg, unsigned int PCMsgIncrComponent)
{
	if (clockManagment.ackData.inAckRound && PCMsg.size() > clockManagment.clock.size())
		return false;
	clockManagment.prepareComparison(PCMsg, id);
	return clockManagment.clock.satisfiesDeliveryConditions(PCMsg, { PCMsgIncrComponent }, ut->clockEntries[idMsg]);
}

bool Node::deliverMsg(unsigned int idMsg, unsigned int seqMsg, DCS msgClock, simtime_t rcvTime,
		unsigned int MsgIncrComponent)
{
	stat.nbDeliveries++;
	if (!control->canCausallyDeliverMessage( { idMsg, seqMsg }, id))
		stat.falseDeliveredMsg++;

	delivered.push_back(dep(idMsg, seqMsg, rcvTime, MsgIncrComponent, msgClock));
	clockManagment.IncrementPC(ut->clockEntries[idMsg], MsgIncrComponent);
	return control->notifyDeliverMessage( { idMsg, seqMsg }, id);
}

void Node::iterativeDelivery()
{
	bool hasDeliveredMessage = true;
	while (hasDeliveredMessage)
	{
		hasDeliveredMessage = false;
		for (vector<dep>::iterator it = pendingMsg.begin(); it != pendingMsg.end();
		/*fait dans la boucle à cause du erase*/)
		{
			if (PC_test(it->id.id, it->PC, it->incrComponent))
			{
				hasDeliveredMessage = true;
				deliverMsg(it->id.id, it->id.seq, it->PC, it->recvtime, it->incrComponent);
				it = pendingMsg.erase(it);
			}
			else
				it++;
		}
	}
}

void Node::PeriodicComponentCheck()
{
	if (clockManagment.reduceDecision(nbReceivedLastSecond()))
	{
		if (id == 0) // only node 0 launches ack rounds to avoid implementing concurrent acks
		{
			send(createAckComponent(), outGate);
			cerr << "Node " << id << " AckComponent message, active components : "
					<< clockManagment.clock.activeComponents << endl;
			cerr << "Observed load : " << nbReceivedLastSecond() << endl;
			clockManagment.ackData.beginAckRound();
		}
		clockManagment.reduceLocalActiveComponents(id);
	}
	scheduleAt(simTime() + SimTime(1, SIMTIME_S), &clockManagment.componentSizeCheckTimer);
}

AckComponent* Node::createAckComponent()
{
	AckComponent * m = new AckComponent();
	m->setSourceId(id);
	m->setComponent(clockManagment.clock[clockManagment.clock.activeComponents - 1].clock);
	m->setComponentIndex(clockManagment.clock.activeComponents - 1);
	return m;
}

unsigned int Node::nbReceivedLastSecond()
{
	vector<simtime_t>::iterator it = receivedTime.begin();
	while (it != receivedTime.end() && *it < (simTime() - clockManagment.measureTime))
		it++;
	receivedTime.erase(receivedTime.begin(), it);
	return receivedTime.size();
}

void Node::RecvAckComponent(AckComponent* m)
{
	bool reply = clockManagment.acknowledgesComponentDeactivation(m->getComponentIndex(), m->getComponent());
	reply |= !componentUsefulToPendingMessage(m->getComponentIndex(), m->getComponent());
	cerr << "Node " << id << " replies to ackComponent: " << reply << " activeComp "
			<< clockManagment.clock.activeComponents << " localActiveComp " << clockManagment.nbLocalActiveComponents
			<< endl;

	clockManagment.ackData.beginAckRound();
	send(createAckRep(reply, m->getSourceId(), m->getComponentIndex()), outGate);
}

bool Node::componentUsefulToPendingMessage(unsigned int componentIndex, const ProbabilisticClock& component)
{
	for (const dep& pendm : pendingMsg)
	{
		if (pendm.PC.size() >= componentIndex)
		{
			if (!(component == pendm.PC[componentIndex].clock))
				return true;
		}
		else
			return true;
	}
	return false;
}

AckRep* Node::createAckRep(bool reply, unsigned int destination, unsigned int component)
{
	AckRep* mrep = new AckRep();
	mrep->setAck(reply);
	mrep->setIdDest(destination);
	mrep->setSourceId(id);
	mrep->setComponentIndex(component);
	return mrep;
}

void Node::RecvAckRep(AckRep* m)
{
	clockManagment.ackData.getReply(m->getAck());
	if (clockManagment.ackData.gotReplies() == ut->nbNodes)
	{
		cerr << "NODE " << id << " recv ackPositifs: " << clockManagment.ackData.positiveAck << ", ackNegatifs: "
				<< clockManagment.ackData.negativeAck << endl;
		send(createDeleteComponent(clockManagment.acknowledgementDecision(ut->nbNodes), id, m->getComponentIndex()),
				outGate);
		if (clockManagment.ackData.positiveAck == ut->nbNodes)
			clockManagment.reduceClock();
		clockManagment.ackData.reset();
	}
}

AckRoundDecision* Node::createDeleteComponent(bool decision, unsigned int sourceId, unsigned int componentIndex)
{
	AckRoundDecision* msg = new AckRoundDecision();
	msg->setComponentIndex(componentIndex);
	msg->setSourceId(id);
	msg->setDecision(decision);
	return msg;
}

void Node::RecvAckRoundDecision(AckRoundDecision* m)
{
	clockManagment.getAckRoundDecision(m->getDecision(), m->getComponentIndex(), id);
	iterativeDelivery();
}

void Node::clearDelivered()
{
	vector<dep>::iterator it = delivered.begin();
	while (it != delivered.end() && it->recvtime < simTime() - SimTime(1, SIMTIME_S))
		it++;
	delivered.erase(delivered.begin(), it);
}

