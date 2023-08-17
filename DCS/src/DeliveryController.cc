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

#include "DeliveryController.h"

Define_Module(DeliveryController);

void DeliveryController::initialize() {
	cSimpleModule::initialize();
	ut = dynamic_cast<Utilitaries*>(getModuleByPath("DynamicClockSet.ut"));
	Horloges.resize(ut->nbNodes);

	// initialise l'horloge pour chaque mobile
	for (vector<vector<int>>::iterator it = Horloges.begin();
			it != Horloges.end(); ++it)
		it->assign(ut->nbNodes, 0);

	messages.resize(ut->nbNodes);
	std::cout << this->getFullName() << endl;
	//testModule();
}

void DeliveryController::sendMessage(int idSrc, int seq) {
	Horloges[idSrc][idSrc]++; // incrémente l'horloge de la source
	msg *t = new msg();
	t->seq = seq;
	t->nbDeliv = 0; // initialise à 1 car le noeud source le délivre directement MIS À 0 CAR NE DELIVRE PLUS À L'ENVOI
	t->horloge = Horloges[idSrc];
	t->booldeliver.assign(ut->nbNodes, false);
	//t->booldeliver.at(idSrc) = true; // le noeud délivre directement son propre message MODIFICATION LE MOBILE NE DELIVRE PAS DIRECTEMENT LE MSG
	t->sendTime = simTime();
	t->totalTimeDeliver = 0;
	messages[idSrc].push_back(t);
	//    std::cout << "BROADCASTCONTROLLER: MSG ADDED from MH " << idSrc <<" seq " << t->seq << " || "; printHorloge(t->horloge);
}

msg *DeliveryController::searchMessage(vector<msg*> vecMessages, int seq) {
	for (vector<msg*>::iterator it = vecMessages.begin();
			it != vecMessages.end(); ++it) {
		//std::cout << "msg with seq " << (*it)->seq << endl;
		if ((*it)->seq == seq)
			return *it;
	}
	return nullptr;
}

void DeliveryController::deleteMessage(int idSrc, int seq) {
//        std::cout<< "BROADCASTCONTROLLER: DELETE MESSAGE (" << idSrc <<"," <<seq<<")"<<endl;

	for (vector<msg*>::iterator it = messages.at(idSrc).begin();
			it != messages.at(idSrc).end(); ++it)
		if ((*it)->seq == seq) {
			totalTimeDelivered += (*it)->totalTimeDeliver / ut->nbNodes;
			msg* t = *it;
			messages[idSrc].erase(it, it + 1);
			delete t;
			return;
		}
	return;
}

bool DeliveryController::recvMessage(int idSrc, int seq, int idDest,
		bool decision) {
	bool orderedDeliver = true;
	msg *m;
	if ((m = searchMessage(messages[idSrc], seq)) == nullptr) {
		std::cerr
				<< "recvMessage: Message non contenu dans le vecteur des messages du Host idSrc "
				<< idSrc << " seq " << seq << " idDest " << idDest << endl;
		throw "Message non contenu dans le vecteur des messages du Host idSrc";
	}

	//DEBUG
	//std::cout << "decision " << (decision ? "true":"false")<< endl;
	//std::cout << "H_"<< idSrc<< "," << seq<< " " ; printHorloge(m->horloge);
	//std::cout << "H["<< idDest<<"]  " ;printHorloge(HorlogesH.at(idDest));

	//cout<< "Horloge du message " ;
	// compare les deux horloges
	for (int i = 0; i < ut->nbNodes; i++) {
		//        cout <<  m->horloge[i]<< ",";
		// message ne respecte pas encore la cohérence causale
		if (!(m->horloge[i] <= Horloges[idDest][i] || // H_m[i] <= H[i]
				(m->horloge[i] == (Horloges[idDest][i] + 1) && i == idSrc))) // H_m[i] == H[i]+1
		{
			if (decision) // le message a été délivré
			{
				std::cerr << "Source " << idSrc << " DEST " << idDest << " seq "
						<< seq << endl;
				std::cout << "Horloge message ";
				printHorloge(m->horloge);
				std::cout << "Horloge cible   ";
				printHorloge(Horloges[idDest]);
				std::cerr << "Message délivré et ne respecte pas l'ordre causal"
						<< simTime() << endl;
				for (int j = 0; j < ut->nbNodes; j++) {
					if (m->horloge[j] > Horloges[idDest][j] && j != idDest)
						std::cerr << "Msg from " << j << " seq"
								<< Horloges[idDest][j] << " to "
								<< m->horloge[j] << endl;
				}
				//                falseDeliv += (i == idSrc ? m->horloge[i] - Horloges[idDest][i] - 1 : m->horloge[i] - Horloges[idDest][i]);
				orderedDeliver = false;
				//                throw "Message false delivered";
			}
		}
		Horloges[idDest][i] =
				m->horloge[i] > Horloges[idDest][i] ?
						m->horloge[i] : Horloges[idDest][i]; // fait le max entre les 2 vecteurs
	}
	//    cout<<endl;

	if (!decision) // le message respecte l'ordre causale mais a été marqué comme ne le respectant pas
	{
		std::cerr
				<< "BROADCASTCONTROLLER: Message non délivré mais repecte l'ordre causal DEST "
				<< idDest << " SRC " << idSrc << endl;
		printHorloge(m->horloge);
		printHorloge(Horloges[idDest]);
		std::cerr << "Message non délivré mais respecte l'ordre causal" << endl;
		exit(1);
	}
	if (m->booldeliver[idDest]) {
		std::cerr << "BROADCASTCONTROLLER: Délivraison multiple SRC " << idSrc
				<< " seq " << seq << " to " << idDest << endl;
		printHorloge(m->horloge);
		printHorloge(Horloges[idDest]);
		std::cerr << "Délivraison multiple" << endl;
		multipleDeliv++;
		exit(1);
	}

	m->nbDeliv++;
	m->booldeliver[idDest] = true;
	m->totalTimeDeliver += simTime() - m->sendTime;

	if (m->nbDeliv == ut->nbNodes) // delete le message car reçu par tous les mobiles ET stations
		deleteMessage(idSrc, seq);
	// DEBUG
	//if(decision)
	//    std::cout << "H["<< idDest<<"]  " ;printHorloge(HorlogesH.at(idDest));
	return orderedDeliver;
}

bool DeliveryController::canDeliver(int idSrc, int seq, int idDest) {
	msg *m;
	if ((m = searchMessage(messages[idSrc], seq)) == nullptr) {
		std::cerr
				<< "recvMessage: Message non contenu dans le vecteur des messages du Host idSrc "
				<< idSrc << " seq " << seq << " idDest " << idDest << endl;
		throw "Message non contenu dans le vecteur des messages du Host idSrc";
	}
	for (int i = 0; i < ut->nbNodes; i++) {
		//        cout <<  m->horloge[i]<< ",";
		// message ne respecte pas encore la cohérence causale
		if (!(m->horloge[i] <= Horloges[idDest][i] || // H_m[i] <= H[i]
				(m->horloge[i] == (Horloges[idDest][i] + 1) && i == idSrc))) // H_m[i] == H[i]+1
		{
			return false;
		}
	}
	return true;
}

void DeliveryController::printHorloge(vector<int> v) {
	for (unsigned int i = 0; i < v.size(); i++)
		std::cout << v[i] << "\t";
	std::cout << endl;
}

void DeliveryController::printHorlogeErr(vector<int> v) {
	for (unsigned int i = 0; i < v.size(); i++)
		std::cerr << v[i] << "\t";
	std::cerr << endl;
}

bool DeliveryController::isDependency(int idMsg, int seqMsg, int idDep,
		int seqDep) {
	msg *m;
	if ((m = searchMessage(messages[idMsg], seqMsg)) == nullptr) {
		std::cerr
				<< "recvMessage: Message non contenu dans le vecteur des messages du Host idSrc "
				<< idMsg << " seq " << seqMsg << endl;
		throw "Message non contenu dans le vecteur des messages du Host idSrc";
	}
	return m->horloge[idDep] >= seqDep;
}

void DeliveryController::MissingDependencies(int idMsg, int seqMsg,
		int idDest) {
	msg *m;
	if ((m = searchMessage(messages[idMsg], seqMsg)) == nullptr) {
		std::cerr
				<< "recvMessage: Message non contenu dans le vecteur des messages du Host idSrc "
				<< idMsg << " seq " << seqMsg << endl;
		throw "Message non contenu dans le vecteur des messages du Host idSrc";
	}
	std::cout << simTime() << "MissingDependencies by node " << idDest
			<< " for delivery of " << idMsg << "," << seqMsg << endl;
	for (int i = 0; i < ut->nbNodes; i++) {
		if ((m->horloge[i] > Horloges[idDest][i] && i != idMsg) || // H_m[i] <= H[i]
				(m->horloge[i] > (Horloges[idDest][i] + 1) && i == idMsg))
			std::cout << " entry " << i << " msg:" << m->horloge[i] << " dest "
					<< Horloges[idDest][i] << endl;
	}

}

void DeliveryController::compareHashVectors(vector<int> v1, int id, int seq) {
	msg *m;
	if ((m = searchMessage(messages[id], seq)) == nullptr) {
		std::cerr
				<< "recvMessage: Message non contenu dans le vecteur des messages du Host idSrc "
				<< id << " seq " << seq << endl;
		throw "Message non contenu dans le vecteur des messages du Host idSrc";
	}

	for (int i = 0; i < v1.size(); i++) {
		if (v1[i] != m->horloge[i]) //&& i != id)
			std::cout << "entry " << i << " v1:" << v1[i] << " msg:"
					<< m->horloge[i] << endl;
	}

}

void DeliveryController::compareMessagesDependencies(int idMsg1, int seqMsg1,
		int idMsg2, int seqMsg2) {
	msg *m1, *m2;
	if ((m1 = searchMessage(messages[idMsg1], seqMsg1)) == nullptr) {
		std::cerr
				<< "recvMessage: Message non contenu dans le vecteur des messages du Host idSrc "
				<< idMsg1 << " seq " << seqMsg1 << endl;
		throw "Message non contenu dans le vecteur des messages du Host idSrc";
	}

	if ((m2 = searchMessage(messages[idMsg2], seqMsg2)) == nullptr) {
		std::cerr
				<< "recvMessage: Message non contenu dans le vecteur des messages du Host idSrc "
				<< idMsg2 << " seq " << seqMsg2 << endl;
		throw "Message non contenu dans le vecteur des messages du Host idSrc";
	}

	std::cout << simTime() << "Comparision of dependencies of messages "
			<< idMsg1 << "," << seqMsg1 << " | " << idMsg2 << "," << seqMsg2
			<< endl;

	for (int i = 0; i < ut->nbNodes; i++) {
		if (m1->horloge[i] != m2->horloge[i])
			std::cout << " entry " << i << " m1:" << m1->horloge[i] << " m2: "
					<< m2->horloge[i] << endl;
	}

}

// test le module avec un jeu d'instructions vérifié à la main
void DeliveryController::testModule(void) {
	//0
	sendMessage(0, 0);
	sendMessage(0, 1);
	//1
	sendMessage(1, 0);
	recvMessage(0, 0, 1, true);
	//2
	sendMessage(2, 0);
	//0
	recvMessage(1, 0, 0, true);
	sendMessage(0, 2);
	recvMessage(2, 0, 0, true);

	//1
	recvMessage(0, 2, 1, false);
	sendMessage(1, 1);

	//2
	recvMessage(0, 2, 2, false);
	recvMessage(0, 1, 2, false);
	sendMessage(2, 1);
	recvMessage(1, 0, 2, true);
	recvMessage(0, 0, 2, true);
	recvMessage(0, 1, 2, true);
	recvMessage(0, 2, 2, true);
	recvMessage(1, 1, 2, true);
	sendMessage(2, 2);

	//0
	recvMessage(2, 1, 0, true);
	recvMessage(1, 1, 0, true);
	recvMessage(2, 2, 0, true);
	sendMessage(0, 3);

	//1
	recvMessage(2, 1, 1, false);
	recvMessage(0, 1, 1, true);
	recvMessage(0, 2, 1, true);
	recvMessage(2, 0, 1, true);
	recvMessage(2, 1, 1, true);
	sendMessage(1, 2);
	recvMessage(0, 3, 1, false);
	recvMessage(2, 2, 1, true);
	recvMessage(0, 3, 1, true);

	//2
	recvMessage(1, 2, 2, true);
	recvMessage(0, 3, 2, true);

	//1
	recvMessage(1, 2, 0, true);
	std::cout << "Réussite des tests" << endl;
}

