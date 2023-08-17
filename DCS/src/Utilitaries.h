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

using namespace omnetpp;
using namespace std;


class Utilitaries : public cSimpleModule
{
  public:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    int combinaisons(int p, int n);
    vector<vector<int>> comb(int N, int K);
    unsigned long long  computeNbCombinaisons(int n, int k);
    vector<vector<unsigned int>> EvenCombinations(int N, int k, int M);



    vector<vector<unsigned int>>  clockEntries;
    int nbCombinaisons;

    vector<int> channelRandNumber;
    std::default_random_engine generator, generatorChannelDelay, generatorSendDistribution;
#define CHANNELDELAY 100000. //100000
    std::normal_distribution<double>* distributionChannelDelayPair = new std::normal_distribution<double>(CHANNELDELAY,20000.); // 20000
    std::normal_distribution<double>* distributionChannelDelayImpair = new std::normal_distribution<double>(CHANNELDELAY,20000.); // 20000

    std::normal_distribution<double>* sendDistribution = new std::normal_distribution<double>(0.,10000.);

#define LOAD_MULTIPLIER 10
    int LOAD_AMPLITUDE=100;
    double PEAKSPERDELAY;
    int nbNodes;
    int clockLength;
    int load=20;
//    int baseload=20;
    int targetload = 0;
    map<int,vector<int>> m;
    double delaySend; // en ms

};

#endif
