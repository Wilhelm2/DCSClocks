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

#ifndef __DYNAMICCLOCKSET_UTILITARIES_H_
#define __DYNAMICCLOCKSET_UTILITARIES_H_

#include <omnetpp.h>
#include <random>
#include <vector>
#include <algorithm>
#include <fstream>

using namespace omnetpp;
using namespace std;

typedef struct sIdMsg
{
	unsigned int id;
	unsigned int seq;
} idMsg;

class Utilitaries: public cSimpleModule
{
public:
	virtual void initialize();
	virtual void handleMessage(cMessage *msg);
	unsigned int computeNbIncrementedEntries();
	vector<vector<unsigned int>> EvenCombinations(unsigned int N, unsigned int k, unsigned int M);
	void readLoadFromFile();

	vector<vector<unsigned int>> clockEntries;

	vector<int> channelRandNumber;
	std::default_random_engine generator, generatorChannelDelay, generatorSendDistribution;
#define CHANNELDELAY 100000. //100000
	std::normal_distribution<double>* distributionChannelDelayPair = new std::normal_distribution<double>(CHANNELDELAY,
			20000.); // 20000
	std::normal_distribution<double>* distributionChannelDelayImpair = new std::normal_distribution<double>(
	CHANNELDELAY, 20000.); // 20000

	std::normal_distribution<double>* sendDistribution = new std::normal_distribution<double>(0., 10000.);

	double PEAKSPERDELAY;
	unsigned int nbNodes;
	unsigned int clockLength;

	vector<unsigned int> loadVector;
	unsigned int indexLoad = 0;
	cMessage loadTimer;
};

#endif
