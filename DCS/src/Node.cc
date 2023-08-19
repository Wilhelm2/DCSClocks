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
		scheduleAt(SimTime(1001, SIMTIME_MS), &componentSizeCheckTimer);
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
	if (msg == &componentSizeCheckTimer)
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

bool Node::expandDecision()
{
	if ((simTime() - lastExpand < 0.5) || (simTime() - lastReduce) < 0.5 || ackData.inAckRound)
		return false;

	if (nbReceivedLastSecond() > 10 && ut->clockLength * clock.activeComponents < 20)
		return true;
	if (nbReceivedLastSecond() > 25 && ut->clockLength * clock.activeComponents < 50)
		return true;
	if (nbReceivedLastSecond() > 33 && ut->clockLength * clock.activeComponents < 100)
		return true;
	if (nbReceivedLastSecond() > 45 && ut->clockLength * clock.activeComponents < 150)
		return true;
	if (nbReceivedLastSecond() > 53 && ut->clockLength * clock.activeComponents < 200)
		return true;
	if (nbReceivedLastSecond() > 65 && ut->clockLength * clock.activeComponents < 300)
		return true;
	if (nbReceivedLastSecond() > 75 && ut->clockLength * clock.activeComponents < 400)
		return true;
	if (nbReceivedLastSecond() > 80 && ut->clockLength * clock.activeComponents < 500)
		return true;
	return false;
}

bool Node::reduceDecision()
{
	if ((simTime() - lastExpand < 0.5) || (simTime() - lastReduce) < 0.5 || ackData.inAckRound
			|| clock.activeComponents == 1)
		return false;
	if (simTime() > 49 && simTime() < 100)
		return false; // POUR DES EXPÉRIENCES !!

	if (nbReceivedLastSecond() < 10 && ut->clockLength * clock.activeComponents > 20)
		return true;
	if (nbReceivedLastSecond() < 22 && ut->clockLength * clock.activeComponents > 50)
		return true;
	if (nbReceivedLastSecond() < 33 && ut->clockLength * clock.activeComponents > 100)
		return true;
	if (nbReceivedLastSecond() < 40 && ut->clockLength * clock.activeComponents > 150)
		return true;
	if (nbReceivedLastSecond() < 45 && ut->clockLength * clock.activeComponents > 200)
		return true;
	if (nbReceivedLastSecond() < 65 && ut->clockLength * clock.activeComponents > 300)
		return true;
	if (nbReceivedLastSecond() < 75 && ut->clockLength * clock.activeComponents > 400)
		return true;
	if (nbReceivedLastSecond() < 80 && ut->clockLength * clock.activeComponents > 500)
		return true;
	return false;
}

void Node::PeriodicComponentCheck()
{
	if (reduceDecision())
//    if( nbReceivedLastSecond() / LIMIT_LOAD_DOWN < clock.activeComponents -1 && ( simTime() - lastExpand > 0.5) && (simTime() - lastReduce) > 0.5  )
//    if( nbReceivedLastSecond() / LIMIT_LOAD_DOWN < clock.activeComponents -1 && ( simTime() - lastExpand > 1) && (simTime() - lastReduce) > 2  )
//    if(nbHashsTestsPerSecond() / nbReceivedLastSecond() < DOWNLIMIT_NB_HASHTESTS_SECOND && clock.activeComponents > 1 && ( simTime() - lastExpand > 1) )
	{
		if (id == 0) // only node 0 launches ack rounds to avoid implementing concurrent acks
		{
			AckComponent * m = new AckComponent();
			m->setSourceId(id);
			m->setComponent(clock[clock.activeComponents - 1].clock);
			m->setAckComponent(clock.activeComponents);
			send(m, outGate);
			cerr << "Node " << id << " AckComponent message, active componens : " << clock.activeComponents << endl;
			cerr << "Observed load : " << nbReceivedLastSecond() << endl;
			ackData.beginAckRound();
		}
		nbLocalActiveComponents = clock.activeComponents - 1;
		incrComponent = rand() % nbLocalActiveComponents;
		cerr << "Node " << id << " reaffects itself to component " << incrComponent << endl;
		lastReduce = simTime();
	}
	scheduleAt(simTime() + SimTime(1001, SIMTIME_MS), &componentSizeCheckTimer);
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

void Node::RecvAckComponent(AckComponent* m)
{
	assert(m->getAckComponent() > clock.size() && "Requests to delete component not in local DCS!");
	bool reply = true;

	if (incrComponent == m->getAckComponent() - 1)
	{
		reply = false;
		cerr << "Replies no because increments this component" << endl;
	}
	if (timeIncrLastComponent > simTime() - SimTime(300, SIMTIME_MS))
	{
		reply = false;
		cerr << "Replies no because incremented the component less than 0.3s ago" << endl;
	}

	if (!(m->getComponent() == clock[m->getAckComponent() - 1].clock))
	{
		reply = false;
		cerr << "Replies no because components are not equal" << endl;
	}

	for (const dep& pendm : pendingMsg)
	{
		if (pendm.PC.size() >= m->getAckComponent())
		{
			if (!(m->getComponent() == pendm.PC[m->getAckComponent() - 1].clock))
				reply = false;
		}
	}
	cerr << "Node " << id << " replies to ackComponent: " << reply << " activeComp " << clock.activeComponents
			<< " localActiveComp " << nbLocalActiveComponents << endl;

	ackData.beginAckRound();
	send(createAckRep(reply, m->getSourceId(), m->getAckComponent()), outGate);
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
	DeleteComponent* msg = new DeleteComponent();
	msg->setAckComponent(m->getAckComponent());
	msg->setSourceId(id);
	ackData.getReply(m->getAck());

	if (ackData.gotReplies() == ut->nbNodes)
	{ // got reply from all nodes
		if (ackData.positiveAck == ut->nbNodes)
		{
			msg->setAck(true);
			clock.activeComponents--;
		}
		else
			msg->setAck(false);
		cerr << "NODE " << id << " Send deleteCompoenent " << msg->getAck() << endl;
		send(msg, outGate);
	}
	else
		cerr << "NODE " << id << " recv ackPositifs: " << ackData.positiveAck << ", ackNegatifs: "
				<< ackData.negativeAck << endl;
}

void Node::RecvDeleteComponent(DeleteComponent* m)
{
	ackData.reset();
	if (m->getAck())
	{
		cerr << "NODE " << id << " RecvDeleteComponent reply " << m->getAckComponent() << endl;
		if (clock.activeComponents == m->getAckComponent())
		{
			if (nbLocalActiveComponents == clock.activeComponents)
				nbLocalActiveComponents--;
			clock.activeComponents--;
		}
	}
	else
		incrComponent = rand() % clock.size();
	iterativeDelivery();
}

bool Node::deliverMsg(unsigned int idMsg, unsigned int seqMsg, DCS v, simtime_t rcvTime, unsigned int MsgIncrComponent)
{
	stat.nbDeliveries++;
	if (!control->canDeliver(idMsg, seqMsg, id))
		stat.falseDeliveredMsgHashFail++;

	dep d(idMsg, seqMsg, rcvTime, MsgIncrComponent, v);
	delivered.push_back(d);
	clock.incrementEntries( { MsgIncrComponent }, ut->clockEntries[idMsg]);
	return control->recvMessage(idMsg, seqMsg, id, true);
}

int Node::nbReceivedLastSecond()
{
	vector<simtime_t>::iterator it = receivedTime.begin();
	while (it != receivedTime.end() && *it < (simTime() - measureTime))
		it++;
	receivedTime.erase(receivedTime.begin(), it);
	return receivedTime.size();
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

AppMsg* Node::sendAppMsg()
{
	if (DeliveryControl == Delivery::DCS)
		expandClockCheck();
	control->sendMessage(id, seq);
	control->recvMessage(id, seq, id, true);

	stat.nbBroadcasts++;
	stat.localActiveComponentsWhenBroadcast += nbLocalActiveComponents;
	stat.controlDataSize += clock.activeComponents * sizeof(ut->clockLength) * sizeof(unsigned int);

	seq++;
	IncrementPC();
	AppMsg* m = createAppMsg();

	dep d(id, seq, simTime(), incrComponent, clock);
	delivered.push_back(d);
	send(m, outGate);
	assert(clock.activeComponents < incrComponent); // Verifies that does not increment an inactive component
	clearDelivered();
	return m;
}

AppMsg* Node::createAppMsg()
{
	AppMsg* m = new AppMsg("broadcast");
	m->setSeq(seq);
	m->setSourceId(id);
	m->setIncrComponent(incrComponent);
	m->setPC(clock);
	return m;
}

void Node::IncrementPC()
{
	clock.incrementEntries( { incrComponent }, ut->clockEntries[id]);
	if (incrComponent == clock.activeComponents - 1) // keep track when incremented the last component
		timeIncrLastComponent = simTime();
}

void Node::expandClockCheck()
{
	if (nbReceivedLastSecond() == 0)
		return;

	if (!expandDecision())
//    if(nbReceivedLastSecond() / LIMIT_LOAD_UP < clock.activeComponents)
		return;

	if (nbLocalActiveComponents < clock.activeComponents) // a diminué le nombre de composants incr et l'ack n'a pas encore eu lieu
	{
		nbLocalActiveComponents++;
		incrComponent = rand() % nbLocalActiveComponents;
		cerr << "NODE " << id << " AFFECTS ITSELF TO COMPONENT " << incrComponent << " without incr clock " << endl;
		lastExpand = simTime();
		return;
	}
	expandClockForce();
	incrComponent = clock.activeComponents - 1; // fait cette optimisation pour être sûr que le noeud qui augmente son nombre de composants envoyés incr bien le composant, sinon les autres noeuds ne vont peut-être pas prendre en compte cette augmentation
}

void Node::expandClockForce()
{
	if (ackData.inAckRound)
	{
		cerr << "CURRENT ACKROUND" << endl;
	}
	lastExpand = simTime();
	clock.activeComponents++;
	nbLocalActiveComponents++;
	incrComponent = rand() % nbLocalActiveComponents;
	if (clock.activeComponents - 1 < clock.size()) // dans ce cas a déjà un composant local non utilisé
		return;
	clock.Add();
	cerr << "EXPAND THE CLOCK ! Node " << id << " NOW INCREMENTS COMPONENT " << incrComponent
			<< " number of components " << clock.size() << endl;
}

bool Node::PC_test(unsigned int idMsg, const DCS &PCMsg, unsigned int PCMsgIncrComponent)
{
	if (PCMsg.size() > clock.size())
	{ // adds a component to the clock
		if (ackData.inAckRound)
			return false; // does not expand clock during ackRound
		expandClockForce();
	}
	else if (clock.activeComponents < PCMsg.size())
	{ // checks whether needs to activate a component
		if (!(PCMsg[clock.activeComponents].clock <= clock[clock.activeComponents].clock))
		{ // The message's component has an entry higher than the local one
			cerr << "message component clock value is higher" << endl;
			expandClockForce();
		}
	}
	return clock.satisfiesDeliveryConditions(PCMsg, { PCMsgIncrComponent }, ut->clockEntries[idMsg]);
}

// Removes old messages from delivered
void Node::clearDelivered()
{
	vector<dep>::iterator it = delivered.begin();
	while (it != delivered.end() && it->recvtime < simTime() - SimTime(1, SIMTIME_S))
		it++;
	delivered.erase(delivered.begin(), it);
}

