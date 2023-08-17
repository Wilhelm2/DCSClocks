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

int Node::idCounter = 0;

void Node::initialize() {
	cSimpleModule::initialize();
//    scheduleAt(simTime(), broadcastTimer);
//    std::cerr<< "Node " << id << " broadcast msg at " << (simTime()+ *(new SimTime( (((float)id)/nbNodes * 2000), SIMTIME_MS)))<<endl;
	if (DeliveryControl == RECOVERY)
		scheduleAt(SimTime(1001, SIMTIME_MS), &componentSizeCheckTimer);
//    scheduleAt(simTime()+ *(new SimTime( (((float)id)/nbNodes * 5000)+10, SIMTIME_MS)), broadcastTimer); // modification pour que les noeuds commencent progressivement à envoyer des msg ( pas tous au même moment à l'init) +++ AJOUTE 10 ms pour laisser le temps aux msg Init de faire le parcours
	ut = dynamic_cast<Utilitaries*>(getModuleByPath("DynamicClockSet.ut"));
	PC.push_back(ProbabilisticClock(ut->clockLength));
	vectorClockDelivered.resize(ut->nbNodes);
	control = dynamic_cast<DeliveryController*>(getModuleByPath(
			"DynamicClockSet.control"));
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

void Node::handleMessage(cMessage *msg) {
	if (msg == &componentSizeCheckTimer) {
		PeriodicComponentCheck();
	} else if (BroadcastNotify* m = dynamic_cast<BroadcastNotify*>(msg)) {
		createAppMsg();
	} else if (AppMsg* m = dynamic_cast<AppMsg*>(msg)) {
		unsigned int delay = m->getDelay();
		//cerr<<"delay " << delay<<endl;
		if (delay != 0) {
			m->setDelay(0);
			scheduleAt(simTime() + SimTime(delay, SIMTIME_US), msg);
		} else {
			RecvAppMsg(m);
			delete m;
		}
	} else if (AckComponent* m = dynamic_cast<AckComponent*>(msg)) {
		unsigned int delay = m->getDelay();
		if (delay != 0) {
			m->setDelay(0);
			scheduleAt(simTime() + SimTime(delay, SIMTIME_US), msg);
		} else {
			RecvAckComponent(m);
			delete m;
		}
	} else if (AckRep* m = dynamic_cast<AckRep*>(msg)) {
		unsigned int delay = m->getDelay();
		if (delay != 0) {
			m->setDelay(0);
			scheduleAt(simTime() + SimTime(delay, SIMTIME_US), msg);
		} else {
			RecvAckRep(m);
			delete m;
		}
	} else if (DeleteComponent* m = dynamic_cast<DeleteComponent*>(msg)) {
		unsigned int delay = m->getDelay();
		if (delay != 0) {
			m->setDelay(0);
			scheduleAt(simTime() + SimTime(delay, SIMTIME_US), msg);
		} else {
			RecvDeleteComponent(m);
			delete m;
		}
	} else {
		//std::cout << "NODE " <<id << "  " << msg<<endl;
		std::cerr << "MESSAGE DE TYPE INCONNU REÇU !" << msg << endl;
		throw "error message type";
	}
}

simtime_t Node::maxtime(simtime_t t1, simtime_t t2) {
	if (t1 > t2)
		return t1;
	else
		return t2;
}

bool Node::expandDecision() {
	if ((simTime() - lastExpand < 0.5) || (simTime() - lastReduce) < 0.5
			|| currentACKRound)
		return false;

	if (nbReceivedLastSecond() > 10
			&& ut->clockLength * nbActiveComponents < 20)
		return true;
	if (nbReceivedLastSecond() > 25
			&& ut->clockLength * nbActiveComponents < 50)
		return true;
	if (nbReceivedLastSecond() > 33
			&& ut->clockLength * nbActiveComponents < 100)
		return true;
	if (nbReceivedLastSecond() > 45
			&& ut->clockLength * nbActiveComponents < 150)
		return true;
	if (nbReceivedLastSecond() > 53
			&& ut->clockLength * nbActiveComponents < 200)
		return true;
	if (nbReceivedLastSecond() > 65
			&& ut->clockLength * nbActiveComponents < 300)
		return true;
	if (nbReceivedLastSecond() > 75
			&& ut->clockLength * nbActiveComponents < 400)
		return true;
	if (nbReceivedLastSecond() > 80
			&& ut->clockLength * nbActiveComponents < 500)
		return true;
	return false;
}

bool Node::reduceDecision() {
	if ((simTime() - lastExpand < 0.5) || (simTime() - lastReduce) < 0.5
			|| currentACKRound || nbActiveComponents == 1)
		return false;
	if (simTime() > 49 && simTime() < 100)
		return false; // POUR DES EXPÉRIENCES !!

	if (nbReceivedLastSecond() < 10
			&& ut->clockLength * nbActiveComponents > 20)
		return true;
	if (nbReceivedLastSecond() < 22
			&& ut->clockLength * nbActiveComponents > 50)
		return true;
	if (nbReceivedLastSecond() < 33
			&& ut->clockLength * nbActiveComponents > 100)
		return true;
	if (nbReceivedLastSecond() < 40
			&& ut->clockLength * nbActiveComponents > 150)
		return true;
	if (nbReceivedLastSecond() < 45
			&& ut->clockLength * nbActiveComponents > 200)
		return true;
	if (nbReceivedLastSecond() < 65
			&& ut->clockLength * nbActiveComponents > 300)
		return true;
	if (nbReceivedLastSecond() < 75
			&& ut->clockLength * nbActiveComponents > 400)
		return true;
	if (nbReceivedLastSecond() < 80
			&& ut->clockLength * nbActiveComponents > 500)
		return true;
	return false;
}

void Node::PeriodicComponentCheck() {
	// id == 0 test pour que ce soit que le noeud 0 qui fasse le check, afin d'éviter de devoir implémenter les acks concurrents
	//  if(failedHashTestPerSecond < LIMITFAILEDHASHTESTREDUCE && id == 0 && nbSentComponents>1) // lance un round de ack pour réduire le nombre de composants
//    std::cerr<< "NODE " <<id << " TEST TO REDUCE NBCOMPONENTS nbHashsTestsPerSecond()" << nbHashsTestsPerSecond() << " nbReceivedLastSecond() " << nbReceivedLastSecond() << " nbsentcomponents " << nbActiveComponents<< " lastexpand " << lastExpand<<endl;

	if (reduceDecision())
//    if( nbReceivedLastSecond() / LIMIT_LOAD_DOWN < nbActiveComponents -1 && ( simTime() - lastExpand > 0.5) && (simTime() - lastReduce) > 0.5  )
//    if( nbReceivedLastSecond() / LIMIT_LOAD_DOWN < nbActiveComponents -1 && ( simTime() - lastExpand > 1) && (simTime() - lastReduce) > 2  )
//    if(nbHashsTestsPerSecond() / nbReceivedLastSecond() < DOWNLIMIT_NB_HASHTESTS_SECOND && nbActiveComponents > 1 && ( simTime() - lastExpand > 1) )
	{
		if (id == 0) {
			AckComponent * m = new AckComponent();
			m->setSourceId(id);
			m->setComponent(PC[nbActiveComponents - 1].clock);
			m->setAckComponent(nbActiveComponents);
			send(m, outGate);
			cerr << "NODE " << id << " SENT MESSAGE TO DISABLE COMPONENT "
					<< nbActiveComponents << endl;
			cerr << "\t\t OBSERVED LOAD : " << nbReceivedLastSecond()
					<< " LIMIT LOADDOWN " << LIMIT_LOAD_DOWN << " rapport "
					<< nbReceivedLastSecond() / LIMIT_LOAD_DOWN
					<< " nbactiveComponents " << nbActiveComponents << endl;
		}
		nbLocalActiveComponents = nbActiveComponents - 1;
		incrComponent = rand() % nbLocalActiveComponents;
		cerr << "PROCESS " << id << " REAFFECTS ITSELF TO COMPONENT "
				<< incrComponent << endl;
		lastReduce = simTime();
	}
	failedHashTestPerSecond = 0;
	scheduleAt(simTime() + SimTime(1001, SIMTIME_MS), &componentSizeCheckTimer);
}

int Node::calculateDelay(int idTarget) {
	int delay;
	if ((ut->channelRandNumber[id] + ut->channelRandNumber[idTarget]) % 2 == 0)
		delay = (*ut->distributionChannelDelayPair)(ut->generatorChannelDelay);
	else
		delay = (*ut->distributionChannelDelayImpair)(
				ut->generatorChannelDelay);
	if (delay < 0)
		delay = 0;
	return delay;
}

void Node::RecvAppMsg(AppMsg*m) {
	// recréé le vecteur contenant les différents composants
	vector<ProbabilisticClock> PCMsg;
	for (unsigned int i = 0; i * ut->clockLength < m->getPC().size(); i++) {
		ProbabilisticClock pc;
		pc.clock = vector<unsigned int>(
				m->getPC().begin() + i * ut->clockLength,
				m->getPC().begin() + (i + 1) * ut->clockLength);
		PCMsg.push_back(pc);
	}

	receivedTime.push_back(simTime());

	// DELIVERY AT RECEPTION WITHOUT CONTROLING
	if (DeliveryControl == NOTHING) {
		if (!control->canDeliver(m->getSourceId(), m->getSeq(), id)) {
			stat.falseDeliveredMsgHashSuccess++;
			std::cerr << "MESSAGE DELIVRÉ HORS ORDRE CAUSAL" << endl;
		}
		deliverMsg(m->getSourceId(), m->getSeq(), PCMsg, simTime(),
				m->getIncrComponent()); //delivre le message
		return;
	} else if (DeliveryControl == PCcomparision) {
		if (PC_test(m->getSourceId(), PCMsg, m->getIncrComponent())) {
			if (!control->canDeliver(m->getSourceId(), m->getSeq(), id))
				stat.falseDeliveredMsgHashFail++;
			deliverMsg(m->getSourceId(), m->getSeq(), PCMsg, simTime(),
					m->getIncrComponent()); //delivre le message
			iterativeDeliveryPCcomparision();
		} else {
			dep d(m->getSourceId(), m->getSeq(), simTime(),
					m->getIncrComponent());
			pendingMsg.push_back(make_tuple(d, PCMsg));
		}
		return;
	}

	if (PC_test(m->getSourceId(), PCMsg, m->getIncrComponent())) {
		if (!control->canDeliver(m->getSourceId(), m->getSeq(), id))
			stat.falseDeliveredMsgHashFail++;
		deliverMsg(m->getSourceId(), m->getSeq(), PCMsg, simTime(),
				m->getIncrComponent()); //delivre le message
		iterativeDelivery();
	} else {
		dep d(m->getSourceId(), m->getSeq(), simTime(), m->getIncrComponent());
		pendingMsg.push_back(make_tuple(d, PCMsg));
	}

}

void Node::RecvAckComponent(AckComponent* m) {
	AckRep* mrep = new AckRep();
	mrep->setAck(true);
	mrep->setIdDest(m->getSourceId());
	mrep->setSourceId(id);
	mrep->setAckComponent(m->getAckComponent());
	if (m->getAckComponent() > PC.size()) {
		cerr << "VEUT DELETE UN COMPOSANT QUE LE NOEUD N'A PAS !" << endl;
		throw "error";
	}
	if (incrComponent == m->getAckComponent() - 1
			|| timeIncrLastComponent > simTime() - SimTime(300, SIMTIME_MS)) {
		mrep->setAck(false);
		if (incrComponent == m->getAckComponent() - 1)
			cerr << "Node replies false because increments this component"
					<< endl;
		else
			cerr
					<< "Node replies false because lastIncrComponent less than 0.3s ago"
					<< endl;
	}
	for (int i = 0; i < ut->clockLength; i++) {
		if (PC[m->getAckComponent() - 1][i] != m->getComponent()[i])
			mrep->setAck(false);
	}
	for (tuple<dep, vector<ProbabilisticClock>> pendm : pendingMsg) {
		if (get<1>(pendm).size() >= m->getAckComponent()) // l'horloge du message contient le composant
				{
			for (int i = 0; i < ut->clockLength; i++) {
				if (get<1>(pendm)[m->getAckComponent() - 1][i]
						!= m->getComponent()[i]) // compare chaque entrée pour savoir si c'est la bonne
					mrep->setAck(false);
			}
		}
	}
	cerr << "NODE " << id << " REPLIES TO ACKCOMPONENT WITH " << mrep->getAck()
			<< " activeComp " << nbActiveComponents << " localActiveComp "
			<< nbLocalActiveComponents << endl;

	currentACKRound = true; // bloque l'expand de l'horloge
	send(mrep, outGate);
}

void Node::RecvAckRep(AckRep* m) {
	DeleteComponent* msg = new DeleteComponent();
	msg->setAckComponent(m->getAckComponent());
	msg->setSourceId(id);
	if (m->getAck())
		ackPositifs++;
	else
		ackNegatifs++;
	if (ackPositifs + ackNegatifs == ut->nbNodes - 1) // -1 car n'attends pas de réponse de soi-même
			{ // tous les noeuds ont acquittés le composant, peut envoyer le message delete
		if (ackPositifs == ut->nbNodes - 1) {
			msg->setAck(true);
			nbActiveComponents--;
		} else
			msg->setAck(false);
		cerr << "NODE " << id << " SEND DELETECOMPONENT " << msg->getAck()
				<< endl;
		send(msg, outGate);
		ackPositifs = 0;
		ackNegatifs = 0;
	} else
		cerr << "NODE " << id << " RECEIVED " << ackPositifs
				<< " positive replies and " << ackNegatifs
				<< " negative replies" << endl;
}

void Node::RecvDeleteComponent(DeleteComponent* m) {
	currentACKRound = false; // libère l'expand de l'horloge
	if (m->getAck()) {
		cerr << "NODE " << id << " RECEIVE DISABLE COMPONENT "
				<< m->getAckComponent() << endl;
		if (nbActiveComponents == m->getAckComponent()) {
			if (nbLocalActiveComponents == nbActiveComponents)
				nbLocalActiveComponents--;
			nbActiveComponents--;
			if (nbLocalActiveComponents > nbActiveComponents) {
				cerr << "NBlocalActive supérieur à nbActive "
						<< nbLocalActiveComponents << " > "
						<< nbActiveComponents << endl;
			}
		}
	} else {
//        cerr<< "ROUND FAILLLLLLL"<<endl;
// ne fonctionne pas        incrComponent = rand() % PC.size();
	}
	iterativeDelivery();
	return;

	// au lieu de supprimer le composant je l'enlève de l'horloge
//    cerr<<"NODE "<<id << " RECEIVE DELETECOMPONENT"<<endl;
//    // supprime le composant de l'horloge
//    PC.erase(PC.end()-1);
//    // supprime le composant de tous les messages en attente
//    for(vector<tuple<dep,vector<vector<int>>>>::iterator it=pendingMsg.begin(); it!=pendingMsg.end(); it++)
//        get<1>(*it).erase(get<1>(*it).end()-1);
//
//    // supprime le composant de tous les messages delivered
//    for(vector<tuple<dep,vector<vector<int>>>>::iterator it=delivered.begin(); it!=delivered.end(); it++)
//        get<1>(*it).erase(get<1>(*it).end()-1);

}

// cherche une dépendance dans DepNext \cup Dep
bool Node::searchDep(int idDep, int seqDep) {
	return vectorClockDelivered[idDep] >= seqDep;
}

bool Node::deliverMsg(int idMsg, int seqMsg, const vector<ProbabilisticClock> v,
		simtime_t rcvTime, int MsgIncrComponent) {
	stat.nbDeliveries++;

	dep d(idMsg, seqMsg, rcvTime, MsgIncrComponent);

	delivered.push_back(make_tuple(d, v));
	for (int i : ut->clockEntries[idMsg])
		PC[MsgIncrComponent][i]++;

	vectorClockDelivered[idMsg]++;
	deliveriesTime.push_back(simTime());
	return control->recvMessage(idMsg, seqMsg, id, true);
}

int Node::nbDeliveriesLastSecond() {
	vector<simtime_t>::iterator it = deliveriesTime.begin();
	while (it != deliveriesTime.end() && *it < (simTime() - measureTime))
		it++;
	deliveriesTime.erase(deliveriesTime.begin(), it);
	return deliveriesTime.size();
}

int Node::nbReceivedLastSecond() {
	vector<simtime_t>::iterator it = receivedTime.begin();
	while (it != receivedTime.end() && *it < (simTime() - measureTime))
		it++;
	receivedTime.erase(receivedTime.begin(), it);
	return receivedTime.size();
}

void Node::iterativeDelivery() //int idMsg, int seqMsg,const vector<int>& v, unsigned int mHash, simtime_t rcvTime)
{
	bool deliveredbool = true;
	while (deliveredbool) {
		deliveredbool = false;

		// maintenant la liste des messages pendants (dont l'horloge probabiliste disait qu'ils ne pouvaient pas être délivrés)
		for (vector<std::tuple<dep, vector<ProbabilisticClock>>>::iterator it =
				pendingMsg.begin(); it != pendingMsg.end();
		/*fait dans la boucle à cause du erase*/) {
			dep d = get<0>(*it);
			vector<ProbabilisticClock> vHorloge = get<1>(*it);
			if (PC_test(d.id, vHorloge, d.incrComponent)) // test de l'horloge probabiliste
					{
				deliveredbool = true;

				if (!control->canDeliver(d.id, d.seq, id))
					stat.falseDeliveredMsgHashFail++;
				deliverMsg(d.id, d.seq, vHorloge, d.recvtime, d.incrComponent); //delvire le message
				it = pendingMsg.erase(it); // apparemment it est décrémenté après avoir été passé à erase mais avant que erase s'exécute
			} else {
				it++;
			}
		}

	}
	return;
}

void Node::iterativeDeliveryPCcomparision() {
	// maintenant délivre d'autres messages si possible
	bool deliveredbool = true;
	while (deliveredbool) {
		deliveredbool = false;
		for (vector<std::tuple<dep, vector<ProbabilisticClock>>>::iterator it =
				pendingMsg.begin(); it != pendingMsg.end();
		/*fait dans la boucle à cause du erase*/) {
			dep d = get<0>(*it);
			vector<ProbabilisticClock> vHorloge = get<1>(*it);
			if (PC_test(d.id, vHorloge, d.incrComponent)) // test de l'horloge probabiliste
					{
				deliveredbool = true;
				if (!control->canDeliver(d.id, d.seq, id))
					stat.falseDeliveredMsgHashFail++;
				deliverMsg(d.id, d.seq, vHorloge, d.recvtime, d.incrComponent); //delvire le message
				it = pendingMsg.erase(it); // apparemment it est décrémenté après avoir été passé à erase mais avant que erase s'exécute
			} else
				it++;
		}

	}
	return;
}

AppMsg* Node::createAppMsg() {
	expandClockCheck();
	AppMsg *m = new AppMsg("broadcast");
	seq++;
	control->sendMessage(id, seq);
	control->recvMessage(id, seq, id, true);

	stat.nbBroadcasts++;
	stat.localActiveComponentsWhenBroadcast += nbLocalActiveComponents;

	m->setSeq(seq);
	m->setSourceId(id);
	IncrementPC();
	vector<unsigned int> transmitVector; // vecteur utilisé pour la transmission car des messages ne peuvent apparemment pas contenir des vecteurs vector<vector<int>>
	transmitVector.reserve(ut->clockLength * nbActiveComponents);
	for (int i = 0; i < nbActiveComponents; i++)
		transmitVector.insert(transmitVector.end(), PC[i].clock.begin(),
				PC[i].clock.end());
	m->setPC(transmitVector);
	m->setIncrComponent(incrComponent);
	if (nbActiveComponents < incrComponent) {
		cerr << "Node " << id
				<< " SEND A MESSAGE WITH INCRCOMPONENT < NBSENTCOMPONENT"
				<< endl;
		throw "error";
	}
	clearDelivered();

	stat.controlDataSize += nbActiveComponents * sizeof(PC[0])
			* sizeof(PC[0][0]); // taille ok faut juste faire attention si simu pas longtemps alors msg broadcast actuel baissent la moyenne
	dep d(id, seq, simTime(), incrComponent);
	delivered.push_back(make_tuple(d, PC));
	send(m, outGate);
	vectorClockDelivered[id]++; // CELUI-CI DOIT ÊTRE APRÈS LE CALCUL DU HASH
//    cout<< simTime()<< " Node " << id << " broadcasts message of seq " << d.seq << " nb activeC "<< nbActiveComponents<<endl;
	return m;
}

void Node::IncrementPC() {
	for (int i : ut->clockEntries[id])
		PC[incrComponent][i]++;
	if (incrComponent == nbActiveComponents - 1) // keep track when incremented the last component
		timeIncrLastComponent = simTime();
}

void Node::expandClockCheck() {
	if (nbReceivedLastSecond() == 0)
		return;
	if (DeliveryControl != RECOVERY)
		return;

	if (!expandDecision())
//    if(nbReceivedLastSecond() / LIMIT_LOAD_UP < nbActiveComponents)
		return;

	if (nbLocalActiveComponents < nbActiveComponents) // a diminué le nombre de composants incr et l'ack n'a pas encore eu lieu
			{
		nbLocalActiveComponents++;
		incrComponent = rand() % nbLocalActiveComponents;
		cerr << "NODE " << id << " AFFECTS ITSELF TO COMPONENT "
				<< incrComponent << " without incr clock " << endl;
		lastExpand = simTime();
		return;
	}
	expandClockForce();
	incrComponent = nbActiveComponents - 1; // fait cette optimisation pour être sûr que le noeud qui augmente son nombre de composants envoyés incr bien le composant, sinon les autres noeuds ne vont peut-être pas prendre en compte cette augmentation
}

void Node::expandClockForce() {
	if (DeliveryControl != RECOVERY)
		return;
	if (currentACKRound) {
		cerr << "CURRENT ACKROUND" << endl;
	}
	lastExpand = simTime();
	nbActiveComponents++;
	nbLocalActiveComponents++;
	incrComponent = rand() % nbLocalActiveComponents;
	if (nbActiveComponents - 1 < PC.size()) // dans ce cas a déjà un composant local non utilisé
		return;
	PC.push_back(ProbabilisticClock(ut->clockLength));
	cerr << "EXPAND THE CLOCK ! Node " << id << " NOW INCREMENTS COMPONENT "
			<< incrComponent << " number of components " << PC.size() << endl;
}

//retourne true si pense que c'est ordonné et false sinon
bool Node::PC_test(int idMsg, const vector<ProbabilisticClock> &PCMsg,
		int PCMsgIncrComponent) {
	if (PCMsg.size() > PC.size()) { // adds a component to the clock
		if (currentACKRound)
			return false; // va bufferiser le message puis à la fin du round va appeler iterativeDeliery()
		expandClockForce();
	} else if (nbActiveComponents < PCMsg.size()) // vérifie si doit réactiver un composant
			{
		for (unsigned int i = 0; i < ut->clockLength; i++) {
			if (PC[nbActiveComponents][i] < PCMsg[nbActiveComponents][i]) {
				cerr << "message component clock value is higher" << endl;
				expandClockForce();
				break;
			}
		}
	}

	for (unsigned int i = 0; i < PCMsg.size(); i++) // pour chaque composant
			{
		if (i == PCMsgIncrComponent) // le composant incr par l'émetteur du message
			continue;
		if (!(PCMsg[i] <= PC[i]))
			return false;
	}

	for (unsigned int i = 0; i < ut->clockLength; i++) // pour toutes les entrées de l'horloge
			{
		if (PC[PCMsgIncrComponent][i] < PCMsg[PCMsgIncrComponent][i]) { // vérifie si c'est une entrée dans f(idMsg)
			bool found = false;
			for (int j : ut->clockEntries[idMsg]) {
				if (i == j) {
					if (PC[PCMsgIncrComponent][i]
							== PCMsg[PCMsgIncrComponent][i] - 1)
						found = true;
					else
						return false;
				}
			}
			if (!found)
				return false;
		}
	}
	return true;
}

// passe la liste delivered et delete les messages anciens. Appelé par le module de statistiques
void Node::clearDelivered() {
	vector<std::tuple<dep, vector<ProbabilisticClock>>>::iterator it =
			delivered.begin();
	while (it != delivered.end()
			&& get<0>(*it).recvtime < simTime() - SimTime(1, SIMTIME_S))
		it++;
	delivered.erase(delivered.begin(), it);
}

