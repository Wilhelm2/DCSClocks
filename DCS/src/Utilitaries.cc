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
	delaySend = par("delaySend");
	PEAKSPERDELAY = par("PEAKSPERDELAY");

	for (int i = 0; i < nbNodes; i++)
		channelRandNumber.push_back(rand() % 2);

	float messagesPerSecond = 1000. * nbNodes / delaySend; // 1000 vient du fait que delaySend == 5000 et pas 5
	float channelDelay = CHANNELDELAY / 1000000;
//    int k = min(4.,ceil(std::log1p(1)*clockLength/(messagesPerSecond*channelDelay))); // limite à x sinon l'ensemble des possibilités calculés dans comb sera trop grand
	int k = 2;
	std::cerr << "Number of entries to increment " << k << endl;
	clockEntries = EvenCombinations(nbNodes, k, clockLength); // comb(clockLength, k);
	nbCombinaisons = computeNbCombinaisons(clockLength, k);
	cerr << "nbcombinaisons possible par la clock  " << nbCombinaisons << endl;
//    std::shuffle(clockEntries.begin(), clockEntries.end(),  std::default_random_engine {} );
	for (int i = 0; i < nbNodes; i++)
	{
		cerr << "Node " << i << " entries ";
		for (int j : clockEntries[i % nbCombinaisons])
			cerr << j << " ";
		cerr << endl;
	}

	// Étape 1: La charge cible est : delaySend/LOAD_MULTIPLIER
	// Étape 2: Différence à appliquer : delaySend - delaySend/LOAD_MULTIPLIER
	// Étape 3: Le pas vaut : (delaySend - delaySend/LOAD_MULTIPLIER) / DURÉE DE VARIATION
	//delayIncrement = (delaySend - delaySend/LOAD_MULTIPLIER ) / 50;

}

// le nombre de combinaisons de p parmis n
int Utilitaries::combinaisons(int p, int n)
{
	int res = 1;
	for (int i = p; i <= n; i++)
		res *= i;
	return res;
}

vector<vector<int>> Utilitaries::comb(int N, int K)
{
	std::string bitmask(K, 1); // K leading 1's
	bitmask.resize(N, 0); // N-K trailing 0's
	vector<vector<int>> res;

	do
	{
		vector<int> v;
		for (int i = 0; i < N; ++i) // [0..N-1] integers
		{
			if (bitmask[i])
				v.push_back(i);
		}
		sort(v.begin(), v.end()); // tri les entrées
		res.push_back(v);
	} while (std::prev_permutation(bitmask.begin(), bitmask.end())); // permute bitmask
	return res;
}

unsigned long long Utilitaries::computeNbCombinaisons(int n, int k)
{
	if (nbNodes >= clockLength)
		return clockLength;
	long long ret = 1;
	for (int i = k + 1; i <= n; i++)
		ret *= i;
	for (int i = 2; i <= k; i++)
		ret /= i;
	return ret;
}

// N = nbNodes, k = nb entries to increment when broadcasting a message, M = size of clock
vector<vector<unsigned int>> Utilitaries::EvenCombinations(int N, int k, int M)
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

	/*    vector<unsigned int> verif; verif.resize(M);
	 for(vector<unsigned int> v : combinations)
	 {
	 for(int i :v)
	 verif[i]++;
	 }
	 for(int i:verif)
	 cerr<< i << ",";
	 cerr<<endl;
	 cerr<<"sum of verif " << std::accumulate(verif.begin(), verif.end(), 0)<<endl;
	 cerr<<"size of vector " << combinations.size()<<endl;
	 cerr<<"nbIncrements "<< nbIncrements<<endl;
	 exit(0);
	 */
	return combinations;
}

