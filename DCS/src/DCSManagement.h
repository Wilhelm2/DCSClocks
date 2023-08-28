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

// Contains the variables used during acknowledge rounds
struct ackRoundData
{
	// Number of positive acks received during that round
	unsigned int positiveAck = 0;
	// Number of negative acks received during that round
	unsigned int negativeAck = 0;
	// Whether the node is in an acknowledge round or not
	bool inAckRound = false;

	// Initializes the structure at the beginning of an ack round
	void beginAckRound()
	{
		positiveAck = 1; // for the node which launched the round
		negativeAck = 0;
		inAckRound = true;
	}

	// Resets the structure at the end of an ack round
	void reset()
	{
		positiveAck = negativeAck = 0;
		inAckRound = false;
	}

	// Registers an ack reply
	void getReply(bool reply)
	{
		if (reply)
			positiveAck++;
		else
			negativeAck++;
	}

	// Returns the total number of received replies
	unsigned int gotReplies()
	{
		return positiveAck + negativeAck;
	}
};

// Implements the management of a DCS clock
class DCSManagement
{
public:
	// Returns whether the clock should be expanded or not
	bool expandDecision(unsigned int nbReceivedMessages);
	// Returns whether the clock should be reduced or not
	bool reduceDecision(unsigned int nbReceivedMessages);
	// Reduces the number of local active components
	void reduceLocalActiveComponents(unsigned int nodeId);
	// Increases the number of local active components
	void increaseLocalActiveComponents(unsigned int nodeId);
	// Reaffect the component incremented by the node when broadcasting a message
	void reaffectIncrementedComponent(unsigned int nodeId);
	// Expands the clock
	void expandClock(unsigned int nodeId);
	// Reduces the clock / Deactivates a component
	void reduceClock();
	// Returns whether the node acknowledges or not the deactivation of the component componentId
	bool acknowledgesComponentDeactivation(unsigned int componentId, const ProbabilisticClock& component);
	// Returns whether the component has to be deactivated or not
	bool acknowledgementDecision(unsigned int nbNodes);
	// Prepares the DCS clock to the comparison with the DCS clock PCMsg
	void prepareComparison(const DCS &PCMsg, unsigned int idReceiver);
	// Increments the entries of the DCS clock
	void IncrementPC(vector<unsigned int> clockEntries, unsigned int componentIndex);
	// Gets the acknowledge round decision. Only called by the ack round initiator
	void getAckRoundDecision(bool decision, unsigned int componentIndex, unsigned int nodeId);

	// The node's DCS clock
	DCS clock;
	// Number of the clock's local active components
	unsigned int nbLocalActiveComponents = 1;
	// Index of the incremented component when broadcasting a message
	unsigned int incrComponent = 0;
	// Timer to check whether the clock's size should be reduced
	cMessage componentSizeCheckTimer;
	// Last time at which the clock was expanded
	simtime_t lastExpand = SimTime(-1, SIMTIME_S);
	// Last time at which the clock was reduced
	simtime_t lastReduce = SimTime(0, SIMTIME_S);
	// Average time measurement of message load when deciding whether expand or reduce the clock
	simtime_t measureTime = SimTime(500, SIMTIME_MS);
	// Last time the node increment the last component
	simtime_t timeIncrLastComponent = 0;
	// Structure containing the data related to ack rounds
	struct ackRoundData ackData;
};

#endif /* DCSMANAGEMENT_H_ */
