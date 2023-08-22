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
		scheduleAt(SimTime(1001, SIMTIME_MS), &clockManagment.componentSizeCheckTimer);
	ut = dynamic_cast<Utilitaries*>(getModuleByPath("DynamicClockSet.ut"));
	control = dynamic_cast<DeliveryController*>(getModuleByPath("DynamicClockSet.control"));
	memset(stat.nbMsgCombinaisons, 0, sizeof(stat.nbMsgCombinaisons));
	memset(stat.msgSize, 0, sizeof(stat.msgSize));

	setOutGate();
	sendInitMessage();
	DEBUGRECEPTIONTIME.open("data/DEBUGRECEPTIONTIME.dat", std::ios::out);
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
		else if (DeleteComponent* m = dynamic_cast<DeleteComponent*>(msg))
			RecvDeleteComponent(m);
		else
		{
			std::cerr << "MESSAGE DE TYPE INCONNU REÇU !" << msg << endl;
			exit(1);
		}
		delete msg;
	}
}

AppMsg* Node::sendAppMsg()
{
	if (DeliveryControl == Delivery::DCS)
		expandClockCheck();
	control->sendMessage(id, seq);
	control->recvMessage(id, seq, id, true);

	stat.nbBroadcasts++;
	stat.localActiveComponentsWhenBroadcast += clockManagment.nbLocalActiveComponents;
	stat.controlDataSize += clockManagment.clock.activeComponents * sizeof(ut->clockLength) * sizeof(unsigned int);

	seq++;
	IncrementPC();
	AppMsg* m = createAppMsg();

	dep d(id, seq, simTime(), clockManagment.incrComponent, clockManagment.clock);
	delivered.push_back(d);
	send(m, outGate);
	assert(clockManagment.clock.activeComponents < clockManagment.incrComponent); // Verifies that does not increment an inactive component
	clearDelivered();
	return m;
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
	{
		deliverMsg(m->getSourceId(), m->getSeq(), m->getPC(), simTime(), m->getIncrComponent());
	}
	else if (DeliveryControl == Delivery::PCcomparision || DeliveryControl == Delivery::DCS)
	{

		if (PC_test(m->getSourceId(), m->getPC(), m->getIncrComponent()))
		{
			deliverMsg(m->getSourceId(), m->getSeq(), m->getPC(), simTime(), m->getIncrComponent());
			iterativeDelivery();
		}
		else
		{
			dep d(m->getSourceId(), m->getSeq(), simTime(), m->getIncrComponent(), m->getPC());
			pendingMsg.push_back(d);
		}
		return;
	}
}

bool Node::PC_test(unsigned int idMsg, const DCS &PCMsg, unsigned int PCMsgIncrComponent)
{
	if (PCMsg.size() > clockManagment.clock.size())
	{ // adds a component to the clock
		if (clockManagment.ackData.inAckRound)
			return false; // does not expand clock during ackRound
		clockManagment.expandClock();
	}
	else if (clockManagment.clock.activeComponents < PCMsg.size())
	{ // checks whether needs to activate a component
		if (!(PCMsg[clockManagment.clock.activeComponents].clock
				<= clockManagment.clock[clockManagment.clock.activeComponents].clock))
		{ // The message's component has an entry higher than the local one
			cerr << "message component clock value is higher" << endl;
			clockManagment.expandClock();
		}
	}
	return clockManagment.clock.satisfiesDeliveryConditions(PCMsg, { PCMsgIncrComponent }, ut->clockEntries[idMsg]);
}

void Node::IncrementPC()
{
	clockManagment.clock.incrementEntries( { clockManagment.incrComponent }, ut->clockEntries[id]);
	if (clockManagment.incrComponent == clockManagment.clock.activeComponents - 1) // keep track when incremented the last component
		clockManagment.timeIncrLastComponent = simTime();
}

bool Node::deliverMsg(unsigned int idMsg, unsigned int seqMsg, DCS v, simtime_t rcvTime, unsigned int MsgIncrComponent)
{
	stat.nbDeliveries++;
	if (!control->canDeliver(idMsg, seqMsg, id))
		stat.falseDeliveredMsgHashFail++;

	dep d(idMsg, seqMsg, rcvTime, MsgIncrComponent, v);
	delivered.push_back(d);
	clockManagment.clock.incrementEntries( { MsgIncrComponent }, ut->clockEntries[idMsg]);
	return control->recvMessage(idMsg, seqMsg, id, true);
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
			if (PC_test(it->id, it->PC, it->incrComponent))
			{
				hasDeliveredMessage = true;
				deliverMsg(it->id, it->seq, it->PC, it->recvtime, it->incrComponent);
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
		clockManagment.reduceLocalActiveComponents();
	}
	scheduleAt(simTime() + SimTime(1, SIMTIME_S), &clockManagment.componentSizeCheckTimer);
}

AckComponent* Node::createAckComponent()
{
	AckComponent * m = new AckComponent();
	m->setSourceId(id);
	m->setComponent(clockManagment.clock[clockManagment.clock.activeComponents - 1].clock);
	m->setAckComponent(clockManagment.clock.activeComponents);
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

void Node::expandClockCheck()
{
	if (!clockManagment.expandDecision(nbReceivedLastSecond()))
		return;

	if (clockManagment.nbLocalActiveComponents < clockManagment.clock.activeComponents) // a diminué le nombre de composants incr et l'ack n'a pas encore eu lieu
		clockManagment.increaseLocalActiveComponents();
	else
	{
		clockManagment.expandClock();
		// To ensure that this node increments the newly added component such that other processes increment the number of active components
		clockManagment.incrComponent = clockManagment.clock.activeComponents - 1;
	}
}

void Node::RecvAckComponent(AckComponent* m)
{
	bool reply = clockManagment.acknowledgesComponentDeactivation(m->getAckComponent(), m->getComponent());
	reply |= !componentUsefulToPendingMessage(m->getAckComponent(), m->getComponent());
	cerr << "Node " << id << " replies to ackComponent: " << reply << " activeComp "
			<< clockManagment.clock.activeComponents << " localActiveComp " << clockManagment.nbLocalActiveComponents
			<< endl;

	clockManagment.ackData.beginAckRound();
	send(createAckRep(reply, m->getSourceId(), m->getAckComponent()), outGate);
}

bool Node::componentUsefulToPendingMessage(unsigned int componentIndex, ProbabilisticClock component)
{
	for (const dep& pendm : pendingMsg)
	{
		if (pendm.PC.size() >= componentIndex)
		{
			if (!(component == pendm.PC[componentIndex - 1].clock))
				return true;
		}
	}
	return false;
}

AckRep* Node::createAckRep(bool reply, unsigned int destination, unsigned int component)
{
	AckRep* mrep = new AckRep();
	mrep->setAck(reply);
	mrep->setIdDest(destination);
	mrep->setSourceId(id);
	mrep->setAckComponent(component);
	return mrep;
}

void Node::RecvAckRep(AckRep* m)
{
	clockManagment.ackData.getReply(m->getAck());
	if (clockManagment.ackData.gotReplies() == ut->nbNodes)
	{
		if (clockManagment.ackData.positiveAck == ut->nbNodes)
			clockManagment.reduceClock();
		cerr << "NODE " << id << " Send deleteCompoenent " << clockManagment.acknowledgementDecision(ut->nbNodes)
				<< endl;
		send(createDeleteComponent(clockManagment.acknowledgementDecision(ut->nbNodes), id, m->getAckComponent()),
				outGate);
	}
	else
		cerr << "NODE " << id << " recv ackPositifs: " << clockManagment.ackData.positiveAck << ", ackNegatifs: "
				<< clockManagment.ackData.negativeAck << endl;
}

DeleteComponent* Node::createDeleteComponent(bool decision, unsigned int sourceId, unsigned int componentIndex)
{
	DeleteComponent* msg = new DeleteComponent();
	msg->setAckComponent(componentIndex);
	msg->setSourceId(id);
	msg->setAck(decision);
	return msg;
}

void Node::RecvDeleteComponent(DeleteComponent* m)
{
	clockManagment.ackData.reset();
	if (m->getAck())
	{
		cerr << "NODE " << id << " RecvDeleteComponent reply " << m->getAckComponent() << endl;
		if (clockManagment.clock.activeComponents == m->getAckComponent())
		{
			if (clockManagment.nbLocalActiveComponents == clockManagment.clock.activeComponents)
				clockManagment.nbLocalActiveComponents--;
			clockManagment.clock.activeComponents--;
		}
	}
	else
		clockManagment.incrComponent = rand() % clockManagment.clock.size();
	iterativeDelivery();
}

void Node::clearDelivered()
{
	vector<dep>::iterator it = delivered.begin();
	while (it != delivered.end() && it->recvtime < simTime() - SimTime(1, SIMTIME_S))
		it++;
	delivered.erase(delivered.begin(), it);
}

