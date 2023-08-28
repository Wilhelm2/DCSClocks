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

#ifndef __DYNAMICCLOCKSET_CENTRALCOMMUNICATIONNODE_H_
#define __DYNAMICCLOCKSET_CENTRALCOMMUNICATIONNODE_H_

#include <omnetpp.h>
#include <algorithm>
#include <iostream>
#include <string>
#include <regex>
#include <functional>
#include "Messages/AppMsg_m.h"
#include "Messages/AckComponent_m.h"
#include "Messages/AckRep_m.h"
#include "Messages/AckRoundDecision_m.h"
#include "Messages/broadcastNotify_m.h"
#include "Messages/Init_m.h"
#include "DeliveryController.h"
#include "SimulationParameters.h"

using namespace omnetpp;
using namespace std;

// Node that is in charge of the communication between nodes.
// Nodes send messages to the centralCommunicationNode which determines the communication delays for each message and sends it to the right destination(s).
// Finally, this node also determines which node has to broadcast a message and when.
class CentralCommunicationNode: public cSimpleModule
{
protected:
	virtual void initialize();
	virtual void handleMessage(cMessage *msg);
	// Informs the next node to broadcast a message
	void handleBroadcastNotify();
	void handleInit(Init* m);
	void broadcastMessage(cMessage* m);
	unsigned int getSourceId(cMessage* msg);
	unsigned int getTargetId(cMessage* msg);
	void setTargetId(cMessage* msg, unsigned int targetId);

	unsigned int computeDelay(unsigned int sourceId, unsigned int targetId);
	void incrementDelayIntervals(unsigned int entry);
	void handleAckRep(AckRep* m);

	// Reference to the simulation parameters
	SimulationParameters* ut;

	// Vector containing the gates to communicate with the other nodes. gateToTarget[i] contains the gate to communicate with Nodes[i]
	vector<cGate*> gateToTarget;

	// Number of messages sent on pair channels
	unsigned int nbSendPair = 0;
	// Number of messages sent on unpair channlels
	unsigned int nbSendImpair = 0;
	// Saves the communication delays
	unsigned int delayIntervals[500];

	// id of next node that will broadcast a message
	unsigned int nextBroadcastNode = 0;
	// Timer to schedule the next broadcast
	cMessage broadcastTimer;
};

#endif
