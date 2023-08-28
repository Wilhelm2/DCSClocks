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

#ifndef __DYNAMICCLOCKSET_DELIVERYDeliveryController_H_
#define __DYNAMICCLOCKSET_DELIVERYDeliveryController_H_

#include <string.h>
#include <omnetpp.h>
#include <vector>
#include <stdexcept>

#include "SimulationParameters.h"
#include "structures.h"
#include "TotalDependencies.h"

using namespace std;
using namespace omnetpp;

// Contains the information of a message relevant to ensure that every node delivers exactly once and in causal order
typedef struct s_msg
{
	// id not required since struct belongs to a vector associated with the message's sending process
	// The message's sequence number
	unsigned int seq;
	// The message's dependencies
	TotalDependencies dependencies;
	// Number of nodes that delivered the message
	unsigned int deliveredAtNbPs = 0;
	// Array to control multiple deliveries
	vector<bool> psHasDelivered;
	// The time at which the sender of the message broadcasted it
	simtime_t sendTime;
	//The time at which nodes delivered the message
	vector<simtime_t> deliveredAtTime;
} msg;

// Controls that every nodes delivers every message exactly once and in causal order.
class DeliveryController: public cSimpleModule
{
public:
	virtual void initialize(int stage);
	// For the sender of a message to signal the boradcast
	void notifySendMessage(unsigned int idSender, unsigned int seq);
	vector<msg>::iterator searchMessage(idMsg idM);
	// For the receivers of a message to signal their delivery of the message
	bool notifyDeliverMessage(idMsg idM, unsigned int idDest);
	// Prints the delivery error *errorReason* when the message is delivered out of causal order or multiple times
	void printDeliveryError(string errorReason, idMsg idM, unsigned int destProcess,
			TotalDependencies messageDependencies, TotalDependencies processDependencies);
	// Determines whether the node idDest can causally deliver the message or not
	bool canCausallyDeliverMessage(idMsg idM, unsigned int idDest);
	void deleteMessage(idMsg idM);
	bool isDependency(idMsg idM, idMsg idDep);

	// Entry i contains the messages broadcasted by node i that have not yet been delivered by all nodes
	vector<vector<msg>> processBroadcastedMessages;
	// Entry i contains the dependencies of the next message that node i will broadcast
	vector<TotalDependencies> processDependencies;
	// Reference to the simulation parameters
	SimulationParameters* params;
};

Define_Module(DeliveryController);

#endif
