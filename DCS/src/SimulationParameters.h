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

#ifndef __DYNAMICCLOCKSET_SimulationParameters_H_
#define __DYNAMICCLOCKSET_SimulationParameters_H_

#include <omnetpp.h>
#include <random>
#include <vector>
#include <algorithm>
#include <fstream>

using namespace omnetpp;
using namespace std;

// Contains the simulation parameters
class SimulationParameters: public cSimpleModule
{
public:
	virtual void initialize();
	virtual void handleMessage(cMessage *msg);
	// Computes the number of entries incremented by a process when broadcasting a message
	unsigned int computeNbIncrementedEntries();
	// Computes the entries incremented by each process when it broadcasts a message
	vector<vector<unsigned int>> EvenCombinations(unsigned int N, unsigned int k, unsigned int M);
	// Reads the simulation message load from a file. The message load may wary every second
	void readLoadFromFile();

	// Clock entries incremented by nodes when broadcasting a message
	vector<vector<unsigned int>> clockEntries;

	vector<int> channelRandNumber;
	std::default_random_engine generator, generatorChannelDelay, generatorSendDistribution;
#define CHANNELDELAY 100000. //100000
	std::normal_distribution<double>* distributionChannelDelayPair = new std::normal_distribution<double>(CHANNELDELAY,
			20000.); // 20000
	std::normal_distribution<double>* distributionChannelDelayImpair = new std::normal_distribution<double>(
	CHANNELDELAY, 20000.); // 20000

	std::normal_distribution<double>* sendDistribution = new std::normal_distribution<double>(0., 10000.);

	// Number of nodes inside the system
	unsigned int nbNodes;
	// Clock length
	unsigned int clockLength;

	// Contains the message load for each simulation second, ie loadVector[i] contains the message load at second i
	vector<unsigned int> loadVector;
	// Contains the index of loadVector to read (ie nodes will read the message load in loadVector[indexLoad]
	unsigned int indexLoad = 0;
	// Time to schedule the next load modification
	cMessage loadTimer;
};

#endif
