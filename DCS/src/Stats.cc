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
	ut = dynamic_cast<Utilitaries*>(getModuleByPath("DCS.ut"));
	control = dynamic_cast<DeliveryController*>(getModuleByPath("DCS.control"));
	for (unsigned int i = 0; i < ut->nbNodes; i++)
		nodes.push_back(dynamic_cast<Node*>(getModuleByPath(("DCS.Nodes[" + to_string(i) + "]").c_str())));

	scheduleAt(simTime() + *(new SimTime(1, SIMTIME_S)), new cMessage());

// calcul = ln(2)*R/X avec R taille de l'horloge et X nombre de messages concurrents
// X = nbMessagesPerSecond * channelDelay
	float channelDelay = CHANNELDELAY / 1000000;
	std::cerr << "channelDelay " << channelDelay << endl;

	aloneNumbers.open("Graphs/data/aloneNumbers.dat", std::ios::out);
	broadcastMessageTimeFile.open("Graphs/data/broadcastMessageTimeFile.dat", std::ios::out);
	messageLoadFile.open("Graphs/data/messageLoadFile.dat", std::ios::out);
	clockSizeFile.open("Graphs/data/clockSizeFile.dat", std::ios::out);
	deliveriesFile.open("Graphs/data/deliveriesFile.dat", std::ios::out);

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
	messageLoadFile << simTime() << " " << ut->loadVector[ut->indexLoad] << endl;
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
		int nb = n->clockManagment.clock.size();
		clockSize = max(clockSize, nb);
		activeComponents = max(activeComponents, n->clockManagment.clock.activeComponents);
		maxActiveComponents += n->clockManagment.clock.activeComponents;
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
	int falseDeliveredMsg = 0;
	for (Node* n : nodes)
		falseDeliveredMsg += n->stat.falseDeliveredMsg;

	deliveriesFile << simTime() << " " << falseDeliveredMsg << endl;
	cerr << "falseDeliveredMsg " << falseDeliveredMsg << endl;
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
		totalNbFalseDeliveries += n->stat.falseDeliveredMsg;
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

void Stats::handleMessage(cMessage *msg)
{

	std::cerr << "TIME " << simTime() << endl;
	scheduleAt(simTime() + *(new SimTime(1, SIMTIME_S)), msg);
//    for(int i=0;i<nbNodes;i++)
//        control->printHorlogeErr(control->Horloges[i]);

	/*for(int j=0;j<nodes.size(); j++)
	 {
	 vector<int> res = nodes[j]->mostRecentMsg();
	 std::cerr << "DEPENDANCES " << j << " : ";
	 for(int i = 0 ; i<res.size();i++)
	 std::cerr << res[i] << "\t";
	 std:cerr << endl;
	 }*/

	for (Node*n : nodes)
		cerr << "node " << n->id << " pendingmsg " << n->pendingMsg.size() << " delivered " << n->delivered.size()
				<< " receivedtime " << n->receivedTime.size() << "clock components " << n->clockManagment.clock.size()
				<< " lcomponent " << n->clockManagment.nbLocalActiveComponents << " ccomponent "
				<< n->clockManagment.clock.activeComponents << endl;

//	cerr << "NOMBRE DE DELIVERY/NOEUD" << endl;
//	for (Node* n : nodes)
//		cerr << n->stat.nbDeliveries << "\t";
//	cerr << endl;
//
//	std::cerr << "pendingmsg size " << endl;
//	for (Node* n : nodes)
//	{
//		if (n->pendingMsg.size() > 500)
//			cerr << n->id << ":" << n->pendingMsg.size() << "\t";
//		else
//			cerr << n->pendingMsg.size() << "\t";
//	}
//	cerr << endl;
	WriteMessageLoad();
	WriteClockSize();
	WriteDeliveries();

	if (simTime() == 300)
	{
		WriteAloneNumbers();
		nodes[0]->clockManagment.clock.print();
		vector<int> countIncrComponents;
		countIncrComponents.resize(nodes[0]->clockManagment.clock.size(), 0);
		for (Node* n : nodes)
		{
			countIncrComponents[n->clockManagment.incrComponent]++;
		}
		for (unsigned int i = 0; i < countIncrComponents.size(); i++)
			cerr << i << ":" << countIncrComponents[i] << endl;
		exit(0);
	}
}
