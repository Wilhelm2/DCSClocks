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

#include "Stats.h"

Define_Module(Stats);

void Stats::initialize()
{
	char tmp[40];
	ut = dynamic_cast<Utilitaries*>(getModuleByPath("DynamicClockSet.ut"));
	nextstep = ut->LOAD_AMPLITUDE;
	step = 200 * 2 / ut->LOAD_AMPLITUDE;
	for (int i = 0; i < ut->nbNodes; i++)
	{
		sprintf(tmp, "DynamicClockSet.Nodes[%d]", i);
		nodes.push_back(dynamic_cast<Node*>(getModuleByPath(tmp)));
	}
	control = dynamic_cast<DeliveryController*>(getModuleByPath("DynamicClockSet.control"));

	scheduleAt(simTime() + *(new SimTime(1, SIMTIME_S)), new cMessage());

	// calcul = ln(2)*R/X avec R taille de l'horloge et X nombre de messages concurrents
	// X = nbMessagesPerSecond * channelDelay
	float channelDelay = CHANNELDELAY / 1000000;
	std::cerr << "channelDelay " << channelDelay << endl;

	nbRecoveriesFile.open("data/nbRecoveriesFile.dat", std::ios::out);
	nbHashsFile.open("data/nbHashsFile.dat", std::ios::out);
	nbMsgHashsFile.open("data/nbMsgHashsFile.dat", std::ios::out);
	aloneNumbers.open("data/aloneNumbers.dat", std::ios::out);
	broadcastMessageTimeFile.open("data/broadcastMessageTimeFile.dat", std::ios::out);
	messageLoadFile.open("data/messageLoadFile.dat", std::ios::out);
	clockSizeFile.open("data/clockSizeFile.dat", std::ios::out);
	deliveriesFile.open("data/deliveriesFile.dat", std::ios::out);

	// test de la loi normale, que c'est bien distribué
	/*int distribution[200];
	 memset(distribution,0,sizeof(distribution));
	 for(int i=0;i<100000;i++){
	 int res=(*distributionChannelDelayImpair)(generatorChannelDelay)/10000 -10;
	 distribution[res]++;
	 }
	 for(int i:distribution)
	 cerr<<i<<"\t";
	 cerr<<endl;
	 exit(0);*/

}

void Stats::WriteMessageLoad()
{
	messageLoadFile << simTime() << " " << ut->load << endl; // division par 1000 car delaySend en ms
}

void Stats::WriteClockSize()
{
	int clockSize = 1;
	unsigned int activeComponents = 1;
	float maxActiveComponents = 0;
	float nbBroadcasts = 0;
	int nbActiveWhenBroadcast = 0;
	for (Node*n : nodes)
	{
		int nb = n->clock.size();
		clockSize = max(clockSize, nb);
		activeComponents = max(activeComponents, n->clock.activeComponents);
		maxActiveComponents += n->clock.activeComponents;
		nbBroadcasts += n->stat.nbBroadcasts;
		n->stat.nbBroadcasts = 0;
		nbActiveWhenBroadcast += n->stat.localActiveComponentsWhenBroadcast;
		n->stat.localActiveComponentsWhenBroadcast = 0;
	}
	float avgComponents = (nbBroadcasts > 0 ? nbActiveWhenBroadcast / nbBroadcasts : 0);
	clockSizeFile << simTime() << " " << clockSize << " " << activeComponents * ut->clockLength << " "
			<< maxActiveComponents / ut->nbNodes * ut->clockLength << " " << avgComponents * ut->clockLength << endl;
}

void Stats::WriteDeliveries()
{
	int falseDeliveredMsgHashSuccess = 0;
	int falseDeliveredMsgHashFail = 0;
	int rightDeliveredMsgHashSuccess = 0;
	int rightDeliveredMsgHashFail = 0;
	for (Node* n : nodes)
	{
		falseDeliveredMsgHashSuccess += n->stat.falseDeliveredMsgHashSuccess;
		falseDeliveredMsgHashFail += n->stat.falseDeliveredMsgHashFail;
		rightDeliveredMsgHashSuccess += n->stat.rightDeliveredMsgHashSuccess;
		rightDeliveredMsgHashFail += n->stat.rightDeliveredMsgHashFail;
	}
	deliveriesFile << simTime() << " " << falseDeliveredMsgHashSuccess << " " << falseDeliveredMsgHashFail << " "
			<< rightDeliveredMsgHashSuccess << " " << rightDeliveredMsgHashFail << endl;
	cerr << "falseDeliveredMsgHashSuccess " << falseDeliveredMsgHashSuccess << " falseDeliveredMsgHashFail "
			<< falseDeliveredMsgHashFail << " rightDeliveredMsgHashSuccess " << rightDeliveredMsgHashSuccess
			<< " rightDeliveredMsgHashFail " << rightDeliveredMsgHashFail << endl;
}

void Stats::WriteTotalNbHashs()
{
	float totalHashs = 0;
	int totalDeliveries = 0;
	for (Node* n : nodes)
	{
		nbHashsFile << n->stat.nbHashs << " ";
		totalHashs += n->stat.nbHashs;
		totalDeliveries += n->stat.nbDeliveries;
	}
	nbHashsFile << endl;
	nbHashsFile << totalHashs;
	nbHashsFile << totalHashs / totalDeliveries << endl;
}

void Stats::WriteAloneNumbers()
{
	int totalMessages = 0;
	int totalDeliveredMsg = 0;
	float totalControlDataSize = 0;
	float totalNbFalseDeliveries = 0;
	for (Node* n : nodes)
	{
		totalMessages += n->seq;
		totalDeliveredMsg += n->stat.nbDeliveries;
		totalControlDataSize += n->stat.controlDataSize;
		totalNbFalseDeliveries += n->stat.falseDeliveredMsgHashFail;
		totalNbFalseDeliveries += n->stat.falseDeliveredMsgHashSuccess;
	}
	if (totalDeliveredMsg == 0)
	{
		aloneNumbers << "0 0" << endl;
		return;
	}

	aloneNumbers << totalControlDataSize / totalMessages << " " << totalNbFalseDeliveries / totalDeliveredMsg << endl;
	std::cerr << "Number of false delivered messages " << totalNbFalseDeliveries << endl;
	std::cerr << "RATE of false delivered messages " << totalNbFalseDeliveries / totalDeliveredMsg << endl;

}

void Stats::clearDelivered()
{
	for (Node* n : nodes)
		n->clearDelivered();
}

#define MAXLOAD 200
void Stats::loadHandler()
{
	/*
	 cerr<<simTime()<< " ";
	 ut->load = 100;

	 if(simTime()==50) // fait du load balancing
	 {
	 for(int i =0; i < nodes.size();i++)
	 nodes[i]->incrComponent = 0; // la moitié des processus vont incr le composant 0
	 }

	 if(simTime()==70)
	 {
	 for(int i =0; i < nodes.size()/2;i++)
	 nodes[i]->incrComponent = rand() %nodes[i]->nbActiveComponents ;
	 }
	 if(simTime()==90)
	 {
	 for(int i =0; i < nodes.size();i++)
	 nodes[i]->incrComponent = rand() %nodes[i]->nbActiveComponents ;
	 }


	 return;

	 if(simTime()==300)
	 return;
	 */

	/*
	 if( simTime() == nextstep)
	 {
	 ut->LOAD_AMPLITUDE -= 15;
	 ut->load =20;
	 nextstep += ut->LOAD_AMPLITUDE;
	 step = (200.*2.)/ut->LOAD_AMPLITUDE;
	 }

	 cerr<< "nextstep " << nextstep << " utloadamplitude " << ut->LOAD_AMPLITUDE<<endl;
	 if(simTime() < nextstep -ut->LOAD_AMPLITUDE/2)
	 //    if(((int)simTime().dbl()) % ut->LOAD_AMPLITUDE < 0.5*ut->LOAD_AMPLITUDE) // phase ascendante
	 {
	 ut->load += step;
	 //        ut->load += ut->baseload*LOAD_MULTIPLIER*2/ut->LOAD_AMPLITUDE;
	 cerr<< "increase load to " << ut->load<<endl;
	 }
	 else
	 {
	 ut->load -= step;
	 ut->load = max(1,ut->load);
	 //        ut->load -= ut->baseload*LOAD_MULTIPLIER*2/ut->LOAD_AMPLITUDE;
	 cerr<< "decrease load to "<< ut->load<<endl;
	 }
	 */

	// load random
	if (fmod(simTime().dbl(), 10) == 0)
	{

		if (simTime() < 60)
			ut->targetload = rand() % 30;
		else if (simTime() < 80)
			ut->targetload = rand() % 100 + 100;
		else if (simTime() < 140)
			ut->targetload = rand() % 30;
		else if (simTime() < 160)
			ut->targetload = rand() % 100 + 100;
		else if (simTime() < 220)
			ut->targetload = rand() % 30;
		else if (simTime() < 240)
			ut->targetload = rand() % 100 + 100;
		else
			ut->targetload = rand() % 50;

		step = (ut->targetload - ut->load) / 10;
		cerr << "NEW TARGET LOAD FROM " << ut->load << " TO LOAD " << ut->targetload << endl;
//        ut->load = (ut->targetload - ut->baseload) / 10;
	}
	ut->load += step;

	cerr << " load is equal to " << ut->load << endl;
}

void Stats::handleMessage(cMessage *msg)
{

	std::cerr << "TIME " << simTime() << endl;
	scheduleAt(simTime() + *(new SimTime(1, SIMTIME_S)), msg);
//    for(int i=0;i<nbNodes;i++)
//        control->printHorlogeErr(control->Horloges[i]);

	clearDelivered();
	/*for(int j=0;j<nodes.size(); j++)
	 {
	 vector<int> res = nodes[j]->mostRecentMsg();
	 std::cerr << "DEPENDANCES " << j << " : ";
	 for(int i = 0 ; i<res.size();i++)
	 std::cerr << res[i] << "\t";
	 std:cerr << endl;
	 }*/

	int msgSize[2000];
	int nbMsgCombinaisons[LIMIT_HASHS + 1];
	memset(msgSize, 0, sizeof(msgSize));
	memset(nbMsgCombinaisons, 0, (LIMIT_HASHS + 1) * sizeof(int));
	cout << "MSGSIZE " << endl;
	for (int i = 0; i < 2000; i++)
	{
		for (Node* n : nodes)
		{
			msgSize[i] += n->stat.msgSize[i];
		}
		cout << msgSize[i] << "\t";
	}
	cout << endl;
	cout << "nbMsgCombinations " << endl;
	for (int i = 0; i < LIMIT_HASHS; i++)
	{
		for (Node* n : nodes)
		{
			nbMsgCombinaisons[i] += n->stat.nbMsgCombinaisons[i];
		}
		cout << nbMsgCombinaisons[i] << "\t";
	}
	cout << endl;

	cerr << "NOMBRE DE DELIVERY/NOEUD" << endl;
	for (Node* n : nodes)
		cerr << n->stat.nbDeliveries << "\t";
	cerr << endl;

	std::cerr << "pendingmsg size " << endl;
	for (Node* n : nodes)
	{
		if (n->pendingMsg.size() > 500)
			cerr << n->id << ":" << n->pendingMsg.size() << "\t";
		else
			cerr << n->pendingMsg.size() << "\t";
	}
	cerr << endl;

	WriteMessageLoad();
	WriteClockSize();
	WriteDeliveries();

	loadHandler();

	if (simTime() == 300)
	{
		WriteTotalNbHashs();
		WriteAloneNumbers();
		nodes[0]->clock.print();
		vector<int> countIncrComponents;
		countIncrComponents.resize(nodes[0]->clock.size(), 0);
		for (Node* n : nodes)
		{
			countIncrComponents[n->incrComponent]++;
		}
		for (int i = 0; i < countIncrComponents.size(); i++)
			cerr << i << ":" << countIncrComponents[i] << endl;
		exit(0);
	}

}
