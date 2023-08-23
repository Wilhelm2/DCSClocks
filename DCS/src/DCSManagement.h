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

#ifndef DCSMANAGEMENT_H_
#define DCSMANAGEMENT_H_
#include <omnetpp.h>
#include <vector>
#include "Clock/DCS.h"

using namespace omnetpp;
using namespace std;

struct ackRoundData
{
	unsigned int positiveAck = 0;
	unsigned int negativeAck = 0;
	bool inAckRound = false;

	void beginAckRound()
	{
		positiveAck = 1; // for the node which launched the round
		negativeAck = 0;
		inAckRound = true;
	}

	void reset()
	{
		positiveAck = negativeAck = 0;
		inAckRound = false;
	}

	void getReply(bool reply)
	{
		if (reply)
			positiveAck++;
		else
			negativeAck++;
	}
	unsigned int gotReplies()
	{
		return positiveAck + negativeAck;
	}
};

class DCSManagement
{
public:
	DCSManagement();
	virtual ~DCSManagement();
	bool expandDecision(unsigned int nbReceivedMessages);
	bool reduceDecision(unsigned int nbReceivedMessages);
	void reduceLocalActiveComponents();
	void increaseLocalActiveComponents();
	void expandClock();
	void reduceClock();
	bool acknowledgesComponentDeactivation(unsigned int componentId, ProbabilisticClock component);
	bool acknowledgementDecision(unsigned int nbNodes);
	void IncrementPC(vector<unsigned int> clockEntries);

	DCS clock;
	unsigned int nbLocalActiveComponents = 1;
	unsigned int incrComponent = 0; // Incremented component when broadcasting a message
	cMessage componentSizeCheckTimer;
	simtime_t lastExpand = SimTime(-1, SIMTIME_S);
	simtime_t lastReduce = SimTime(0, SIMTIME_S);
	simtime_t measureTime = SimTime(500, SIMTIME_MS); // Average time measurment of message load when deciding whether expand or reduce the clock
	simtime_t timeIncrLastComponent = 0;
	struct ackRoundData ackData;

};

#endif /* DCSMANAGEMENT_H_ */
