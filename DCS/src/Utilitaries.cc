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

#include "Utilitaries.h"

Define_Module(Utilitaries);

void Utilitaries::initialize()
{
	cSimpleModule::initialize();
	nbNodes = par("nbNodes");
	clockLength = par("clockLength");
	PEAKSPERDELAY = par("PEAKSPERDELAY");

	for (unsigned int i = 0; i < nbNodes; i++)
		channelRandNumber.push_back(rand() % 2);

	clockEntries = EvenCombinations(nbNodes, computeNbIncrementedEntries(), clockLength);
	readLoadFromFile();
	scheduleAt(simTime() + SimTime(1, SIMTIME_S), &loadTimer);
}

void Utilitaries::handleMessage(cMessage *msg)
{
	indexLoad++;
	scheduleAt(simTime() + SimTime(1, SIMTIME_S), &loadTimer);
}

unsigned int Utilitaries::computeNbIncrementedEntries()
{
	//	float messagesPerSecond = 1000. * nbNodes / delaySend; // 1000 vient du fait que delaySend == 5000 et pas 5
	//	float channelDelay = CHANNELDELAY / 1000000;
	//    int k = min(4.,ceil(std::log1p(1)*clockLength/(messagesPerSecond*channelDelay))); // limite à x sinon l'ensemble des possibilités calculés dans comb sera trop grand
	unsigned int k = 2;
	std::cerr << "Number of entries to increment " << k << endl;
	return k;
}

void Utilitaries::readLoadFromFile()
{
	std::ifstream myfile("simulations/loadFile.txt");
	std::string myline;
	std::getline(myfile, myline);
	unsigned int t = 0;
	while (myfile && myline[0] != 0)
	{
		cerr << myline << endl;
		loadVector.push_back(stod(myline));
		cerr << "t " << t++ << " load " << stod(myline) << endl;
		std::getline(myfile, myline);
	}
}

// N = nbNodes, k = nb entries to increment when broadcasting a message, M = size of clock
vector<vector<unsigned int>> Utilitaries::EvenCombinations(unsigned int N, unsigned int k, unsigned int M)
{
	unsigned int nbIncrements = N * k / M + 1; // number of times that each entry should be affected at maximum to a process (added +1 so that if nbIncrements = x.y then will not block)
	vector<unsigned int> vectorIncr;
	vectorIncr.resize(M, 0);
	vector<vector<unsigned int>> combinations;
	unsigned int entry;
	for (unsigned int i = 0; i < N; i++) // for each process
	{
		vector<unsigned int> res;
		for (unsigned int j = 0; j < k; j++) // for each entry
		{
			entry = rand() % M;
			while (std::find(res.begin(), res.end(), entry) != res.end() || vectorIncr[entry] >= nbIncrements)
				entry = (entry + 1) % M;
			res.push_back(entry);
			vectorIncr[entry]++;
		}
		combinations.push_back(res);
	}
	std::shuffle(combinations.begin(), combinations.end(), std::default_random_engine { });
	return combinations;
}

