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
	ut = dynamic_cast<SimulationParameters*>(getModuleByPath("DCS.ut"));
	for (unsigned int i = 0; i < ut->nbNodes; i++)
		nodes.push_back(dynamic_cast<Node*>(getModuleByPath(("DCS.Nodes[" + to_string(i) + "]").c_str())));

	openFiles();

	scheduleAt(simTime() + SimTime(1, SIMTIME_S), &timer);
}

void Stats::openFiles()
{
	string pathToFiles = "simulations/data/";
	aloneNumbers.open(pathToFiles + "aloneNumbers.dat", std::ios::out);
	messageLoadFile.open(pathToFiles + "messageLoadFile.dat", std::ios::out);
	clockSizeFile.open(pathToFiles + "clockSizeFile.dat", std::ios::out);
	deliveriesFile.open(pathToFiles + "deliveriesFile.dat", std::ios::out);
}

void Stats::handleMessage(cMessage *msg)
{
	std::cerr << "Time: " << simTime() << endl;
	scheduleAt(simTime() + SimTime(1, SIMTIME_S), msg);

	WriteMessageLoad();
	WriteClockSize();
	WriteFalseDeliveries();
//	printErrNodeStats();
//	printErrNbDeliveredMessages();
//	printErrPendingMessages();
	printErrIncrementedComponents();
	if (simTime() == 850)
	{
		WriteAloneNumbers();
		exit(0);
	}
}

void Stats::WriteMessageLoad()
{
	messageLoadFile << simTime() << " " << ut->loadVector[ut->indexLoad] << endl;
}

void Stats::WriteClockSize()
{
	unsigned int maxClockSize = 1;
	unsigned int maxActiveComponents = 1;
	float nbBroadcasts = 0;
	unsigned int activeComponentsWhenBroadcast = 0;
	for (Node*n : nodes)
	{
		maxClockSize = max(maxClockSize, n->clockManagment.clock.size());
		maxActiveComponents = max(maxActiveComponents, n->clockManagment.clock.activeComponents);
		nbBroadcasts += n->stat.nbBroadcasts;
		n->stat.nbBroadcasts = 0;
		activeComponentsWhenBroadcast += n->stat.localActiveComponentsWhenBroadcast;
		n->stat.localActiveComponentsWhenBroadcast = 0;
	}
	float avgComponents = (nbBroadcasts > 0 ? activeComponentsWhenBroadcast / nbBroadcasts : 0);
	clockSizeFile << simTime() << " " << maxClockSize << " " << maxActiveComponents * ut->clockLength << " "
			<< maxActiveComponents << " " << avgComponents * ut->clockLength << endl;
}

void Stats::WriteFalseDeliveries()
{
	unsigned int falseDeliveredMsg = 0;
	for (Node* n : nodes)
		falseDeliveredMsg += n->stat.falseDeliveredMsg;

	deliveriesFile << simTime() << " " << falseDeliveredMsg << endl;
	cerr << "falseDeliveredMsg " << falseDeliveredMsg << endl;
}

void Stats::WriteAloneNumbers()
{
	unsigned int nbBroadcastedMsg = 0;
	unsigned int nbDeliveredMsg = 0;
	float controlDataSize = 0;
	float nbFalseDeliveries = 0;

	for (Node* n : nodes)
	{
		nbBroadcastedMsg += n->seq;
		nbDeliveredMsg += n->stat.nbDeliveries;
		controlDataSize += n->stat.controlDataSize;
		nbFalseDeliveries += n->stat.falseDeliveredMsg;
	}
	if (nbDeliveredMsg == 0)
		aloneNumbers << "0 0" << endl;
	else
		aloneNumbers << controlDataSize / nbBroadcastedMsg << " " << nbFalseDeliveries / nbDeliveredMsg << endl;
	std::cerr << "Number of false delivered messages " << nbFalseDeliveries << endl;
	std::cerr << "false delivery rate" << nbFalseDeliveries / (nbDeliveredMsg == 0 ? 1 : nbDeliveredMsg) << endl;
}

void Stats::printErrPendingMessages()
{
	cerr << "Number of pending messages per node" << endl;
	for (Node* n : nodes)
		cerr << n->pendingMsg.size() << "\t";
	cerr << endl;
}

void Stats::printErrNbDeliveredMessages()
{
	cerr << "Number of delivered messages per node" << endl;
	for (Node* n : nodes)
		cerr << n->stat.nbDeliveries << endl;
	cerr << endl;
}

void Stats::printErrNodeStats()
{
	for (Node*n : nodes)
		cerr << "node " << n->id << " pendingmsg " << n->pendingMsg.size() << " delivered " << n->delivered.size()
				<< " receivedtime " << n->receivedTime.size() << "clock components " << n->clockManagment.clock.size()
				<< " lcomponent " << n->clockManagment.nbLocalActiveComponents << " ccomponent "
				<< n->clockManagment.clock.activeComponents << endl;
}

void Stats::printErrIncrementedComponents()
{
	vector<unsigned int> countIncrComponents;
	unsigned int maxComponents = 0;
	for (Node*n : nodes)
		maxComponents = max(maxComponents, n->clockManagment.clock.size());

	countIncrComponents.resize(maxComponents, 0);
	for (Node* n : nodes)
		countIncrComponents[n->clockManagment.incrComponent]++;
	for (unsigned int i = 0; i < countIncrComponents.size(); i++)
		cerr << i << ":" << countIncrComponents[i] << endl;
}
