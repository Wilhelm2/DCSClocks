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
//    scheduleAt(simTime(), broadcastTimer);
//    std::cerr<< "Node " << id << " broadcast msg at " << (simTime()+ *(new SimTime( (((float)id)/nbNodes * 2000), SIMTIME_MS)))<<endl;
	if (DeliveryControl == Delivery::DCS)
		scheduleAt(SimTime(1001, SIMTIME_MS), &componentSizeCheckTimer);
//    scheduleAt(simTime()+ *(new SimTime( (((float)id)/nbNodes * 5000)+10, SIMTIME_MS)), broadcastTimer); // modification pour que les noeuds commencent progressivement à envoyer des msg ( pas tous au même moment à l'init) +++ AJOUTE 10 ms pour laisser le temps aux msg Init de faire le parcours
	ut = dynamic_cast<Utilitaries*>(getModuleByPath("DynamicClockSet.ut"));
	control = dynamic_cast<DeliveryController*>(getModuleByPath("DynamicClockSet.control"));
	memset(stat.nbMsgCombinaisons, 0, sizeof(stat.nbMsgCombinaisons));
	memset(stat.msgSize, 0, sizeof(stat.msgSize));

	Init * m = new Init();
	m->setSourceId(id);
	cModule::GateIterator i(this);
	while (!i.end() && (*i)->getType() == cGate::INPUT)
		i++;
	outGate = (*i);
	send(m, outGate);

	DEBUGRECEPTIONTIME.open("data/DEBUGRECEPTIONTIME.dat", std::ios::out);
}

void Node::handleMessage(cMessage *msg)
{
	if (msg == &componentSizeCheckTimer)
		PeriodicComponentCheck();
	else
	{
		if (dynamic_cast<BroadcastNotify*>(msg))
			createAppMsg();
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
	if ((simTime() - lastExpand < 0.5) || (simTime() - lastReduce) < 0.5 || currentACKRound)
		return false;

	if (nbReceivedLastSecond() > 10 && ut->clockLength * nbActiveComponents < 20)
		return true;
	if (nbReceivedLastSecond() > 25 && ut->clockLength * nbActiveComponents < 50)
		return true;
	if (nbReceivedLastSecond() > 33 && ut->clockLength * nbActiveComponents < 100)
		return true;
	if (nbReceivedLastSecond() > 45 && ut->clockLength * nbActiveComponents < 150)
		return true;
	if (nbReceivedLastSecond() > 53 && ut->clockLength * nbActiveComponents < 200)
		return true;
	if (nbReceivedLastSecond() > 65 && ut->clockLength * nbActiveComponents < 300)
		return true;
	if (nbReceivedLastSecond() > 75 && ut->clockLength * nbActiveComponents < 400)
		return true;
	if (nbReceivedLastSecond() > 80 && ut->clockLength * nbActiveComponents < 500)
		return true;
	return false;
}

bool Node::reduceDecision()
{
	if ((simTime() - lastExpand < 0.5) || (simTime() - lastReduce) < 0.5 || currentACKRound || nbActiveComponents == 1)
		return false;
	if (simTime() > 49 && simTime() < 100)
		return false; // POUR DES EXPÉRIENCES !!

	if (nbReceivedLastSecond() < 10 && ut->clockLength * nbActiveComponents > 20)
		return true;
	if (nbReceivedLastSecond() < 22 && ut->clockLength * nbActiveComponents > 50)
		return true;
	if (nbReceivedLastSecond() < 33 && ut->clockLength * nbActiveComponents > 100)
		return true;
	if (nbReceivedLastSecond() < 40 && ut->clockLength * nbActiveComponents > 150)
		return true;
	if (nbReceivedLastSecond() < 45 && ut->clockLength * nbActiveComponents > 200)
		return true;
	if (nbReceivedLastSecond() < 65 && ut->clockLength * nbActiveComponents > 300)
		return true;
	if (nbReceivedLastSecond() < 75 && ut->clockLength * nbActiveComponents > 400)
		return true;
	if (nbReceivedLastSecond() < 80 && ut->clockLength * nbActiveComponents > 500)
		return true;
	return false;
}

void Node::PeriodicComponentCheck()
{
	// id == 0 test pour que ce soit que le noeud 0 qui fasse le check, afin d'éviter de devoir implémenter les acks concurrents
	//  if(failedHashTestPerSecond < LIMITFAILEDHASHTESTREDUCE && id == 0 && nbSentComponents>1) // lance un round de ack pour réduire le nombre de composants
//    std::cerr<< "NODE " <<id << " TEST TO REDUCE NBCOMPONENTS nbHashsTestsPerSecond()" << nbHashsTestsPerSecond() << " nbReceivedLastSecond() " << nbReceivedLastSecond() << " nbsentcomponents " << nbActiveComponents<< " lastexpand " << lastExpand<<endl;

	if (reduceDecision())
//    if( nbReceivedLastSecond() / LIMIT_LOAD_DOWN < nbActiveComponents -1 && ( simTime() - lastExpand > 0.5) && (simTime() - lastReduce) > 0.5  )
//    if( nbReceivedLastSecond() / LIMIT_LOAD_DOWN < nbActiveComponents -1 && ( simTime() - lastExpand > 1) && (simTime() - lastReduce) > 2  )
//    if(nbHashsTestsPerSecond() / nbReceivedLastSecond() < DOWNLIMIT_NB_HASHTESTS_SECOND && nbActiveComponents > 1 && ( simTime() - lastExpand > 1) )
	{
		if (id == 0)
		{
			AckComponent * m = new AckComponent();
			m->setSourceId(id);

			m->setComponent(clock[nbActiveComponents - 1].clock);
			m->setAckComponent(nbActiveComponents);
			send(m, outGate);
			cerr << "NODE " << id << " SENT MESSAGE TO DISABLE COMPONENT " << nbActiveComponents << endl;
			cerr << "\t\t OBSERVED LOAD : " << nbReceivedLastSecond() << " LIMIT LOADDOWN " << LIMIT_LOAD_DOWN
					<< " rapport " << nbReceivedLastSecond() / LIMIT_LOAD_DOWN << " nbactiveComponents "
					<< nbActiveComponents << endl;
		}
		nbLocalActiveComponents = nbActiveComponents - 1;
		incrComponent = rand() % nbLocalActiveComponents;
		cerr << "PROCESS " << id << " REAFFECTS ITSELF TO COMPONENT " << incrComponent << endl;
		lastReduce = simTime();
	}
	scheduleAt(simTime() + SimTime(1001, SIMTIME_MS), &componentSizeCheckTimer);
}

int Node::calculateDelay(int idTarget)
{
	int delay;
	if ((ut->channelRandNumber[id] + ut->channelRandNumber[idTarget]) % 2 == 0)
		delay = (*ut->distributionChannelDelayPair)(ut->generatorChannelDelay);
	else
		delay = (*ut->distributionChannelDelayImpair)(ut->generatorChannelDelay);
	if (delay < 0)
		delay = 0;
	return delay;
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
	bool reply = true;

	if (m->getAckComponent() > clock.size())
	{
		cerr << "Requests to delete component not in local DCS!" << endl;
		exit(1);
	}
	if (incrComponent == m->getAckComponent() - 1 || timeIncrLastComponent > simTime() - SimTime(300, SIMTIME_MS))
	{
		reply = false;
		if (incrComponent == m->getAckComponent() - 1)
			cerr << "Reply no because increments this component" << endl;
		else
			cerr << "Reply no because incremented less than 0.3s ago" << endl;
	}
	if (!(m->getComponent() == clock[m->getAckComponent() - 1].clock)) // components are not equal
		reply = false;

	for (const dep& pendm : pendingMsg)
	{
		if (pendm.PC.size() >= m->getAckComponent())
		{
			if (!(m->getComponent() == pendm.PC[m->getAckComponent() - 1].clock)) // components are not equal
				reply = false;
		}
	}
	cerr << "Node " << id << " reply to ackComponent: " << reply << " activeComp " << nbActiveComponents
			<< " localActiveComp " << nbLocalActiveComponents << endl;

	currentACKRound = true; // blocks DCS dynamics
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
	if (m->getAck())
		ackPositifs++;
	else
		ackNegatifs++;
	if (ackPositifs + ackNegatifs == ut->nbNodes - 1) // -1 car n'attends pas de réponse de soi-même
	{ // tous les noeuds ont acquittés le composant, peut envoyer le message delete
		if (ackPositifs == ut->nbNodes - 1)
		{
			msg->setAck(true);
			nbActiveComponents--;
		}
		else
			msg->setAck(false);
		cerr << "NODE " << id << " Send deleteCompoenent " << msg->getAck() << endl;
		send(msg, outGate);
		ackPositifs = 0;
		ackNegatifs = 0;
	}
	else
		cerr << "NODE " << id << " recv ackPositifs: " << ackPositifs << ", ackNegatifs: " << ackNegatifs << endl;
}

void Node::RecvDeleteComponent(DeleteComponent* m)
{
	currentACKRound = false;
	if (m->getAck())
	{
		cerr << "NODE " << id << " RECEIVE DISABLE COMPONENT " << m->getAckComponent() << endl;
		if (nbActiveComponents == m->getAckComponent())
		{
			if (nbLocalActiveComponents == nbActiveComponents)
				nbLocalActiveComponents--;
			nbActiveComponents--;
			if (nbLocalActiveComponents > nbActiveComponents)
			{
				cerr << "NBlocalActive supérieur à nbActive " << nbLocalActiveComponents << " > " << nbActiveComponents
						<< endl;
			}
		}
	}
	else
	{
//        cerr<< "ROUND FAILLLLLLL"<<endl;
// ne fonctionne pas        incrComponent = rand() % PC.size();
	}
	iterativeDelivery();
	return;
}

bool Node::deliverMsg(int idMsg, int seqMsg, DCS v, simtime_t rcvTime, int MsgIncrComponent)
{
	stat.nbDeliveries++;
	if (!control->canDeliver(idMsg, seqMsg, id))
		stat.falseDeliveredMsgHashFail++;

	dep d(idMsg, seqMsg, rcvTime, MsgIncrComponent, v);
	delivered.push_back(d);
	clock.incrementEntries( { MsgIncrComponent }, ut->clockEntries[idMsg]);

	deliveriesTime.push_back(simTime());
	return control->recvMessage(idMsg, seqMsg, id, true);
}

int Node::nbDeliveriesLastSecond()
{
	vector<simtime_t>::iterator it = deliveriesTime.begin();
	while (it != deliveriesTime.end() && *it < (simTime() - measureTime))
		it++;
	deliveriesTime.erase(deliveriesTime.begin(), it);
	return deliveriesTime.size();
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
	bool deliveredbool = true;
	while (deliveredbool)
	{
		deliveredbool = false;
		for (vector<dep>::iterator it = pendingMsg.begin(); it != pendingMsg.end();
		/*fait dans la boucle à cause du erase*/)
		{
			if (PC_test(it->id, it->PC, it->incrComponent))
			{
				deliveredbool = true;
				deliverMsg(it->id, it->seq, it->PC, it->recvtime, it->incrComponent);
				it = pendingMsg.erase(it);
			}
			else
			{
				it++;
			}
		}
	}
}

AppMsg* Node::createAppMsg()
{
	expandClockCheck();
	control->sendMessage(id, seq);
	control->recvMessage(id, seq, id, true);

	stat.nbBroadcasts++;
	stat.localActiveComponentsWhenBroadcast += nbLocalActiveComponents;
	stat.controlDataSize += nbActiveComponents * sizeof(ut->clockLength) * sizeof(unsigned int);

	seq++;
	IncrementPC();
	AppMsg* m = createAppMsg();

	dep d(id, seq, simTime(), incrComponent, clock);
	delivered.push_back(d);
	send(m, outGate);
	assert(nbActiveComponents < incrComponent); // Verifies that does not increment an inactive component
	clearDelivered();
	return m;
}

AppMsg* Node::makeAppMsg()
{
	AppMsg *m = new AppMsg("broadcast");
	m->setSeq(seq);
	m->setSourceId(id);
	m->setIncrComponent(incrComponent);
	m->setPC(clock);
	return m;
}

void Node::IncrementPC()
{
	clock.incrementEntries( { incrComponent }, ut->clockEntries[id]);
	if (incrComponent == nbActiveComponents - 1) // keep track when incremented the last component
		timeIncrLastComponent = simTime();
}

void Node::expandClockCheck()
{
	if (nbReceivedLastSecond() == 0)
		return;
	if (DeliveryControl != Delivery::DCS)
		return;

	if (!expandDecision())
//    if(nbReceivedLastSecond() / LIMIT_LOAD_UP < nbActiveComponents)
		return;

	if (nbLocalActiveComponents < nbActiveComponents) // a diminué le nombre de composants incr et l'ack n'a pas encore eu lieu
	{
		nbLocalActiveComponents++;
		incrComponent = rand() % nbLocalActiveComponents;
		cerr << "NODE " << id << " AFFECTS ITSELF TO COMPONENT " << incrComponent << " without incr clock " << endl;
		lastExpand = simTime();
		return;
	}
	expandClockForce();
	incrComponent = nbActiveComponents - 1; // fait cette optimisation pour être sûr que le noeud qui augmente son nombre de composants envoyés incr bien le composant, sinon les autres noeuds ne vont peut-être pas prendre en compte cette augmentation
}

void Node::expandClockForce()
{
	if (currentACKRound)
	{
		cerr << "CURRENT ACKROUND" << endl;
	}
	lastExpand = simTime();
	nbActiveComponents++;
	nbLocalActiveComponents++;
	incrComponent = rand() % nbLocalActiveComponents;
	if (nbActiveComponents - 1 < clock.size()) // dans ce cas a déjà un composant local non utilisé
		return;
	clock.Add();
	cerr << "EXPAND THE CLOCK ! Node " << id << " NOW INCREMENTS COMPONENT " << incrComponent
			<< " number of components " << clock.size() << endl;
}

//retourne true si pense que c'est ordonné et false sinon
bool Node::PC_test(int idMsg, const DCS &PCMsg, int PCMsgIncrComponent)
{
	if (PCMsg.size() > clock.size())
	{ // adds a component to the clock
		if (currentACKRound)
			return false; // does not expand clock during ackRound
		expandClockForce();
	}
	else if (nbActiveComponents < PCMsg.size())
	{ // checks whether needs to activate a component
		if (!(PCMsg[nbActiveComponents].clock <= clock[nbActiveComponents].clock))
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

