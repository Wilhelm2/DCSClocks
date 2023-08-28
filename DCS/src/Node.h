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

#ifndef __DYNAMICCLOCKSET_NODE_H_
#define __DYNAMICCLOCKSET_NODE_H_

#include <omnetpp.h>
#include <algorithm>
#include <iostream>
#include <string>
#include <regex>
#include <functional>
#include "Messages/AppMsg_m.h"
#include "Messages/AckComponent_m.h"
#include "Messages/AckRep_m.h"
#include "Messages/Init_m.h"
#include "Messages/AckRoundDecision_m.h"
#include "DeliveryController.h"
#include "Messages/broadcastNotify_m.h"
#include "Stats.h"
#include "structures.h"
#include "Clock/ProbabilisticClock.h"
#include "Clock/DCS.h"
#include "DCSManagement.h"
#include "SimulationParameters.h"

using namespace omnetpp;
using namespace std;

// Contains the statistics for nodes
typedef struct s_stats_Node
{
	// Number of messages delivered out of causal order
	unsigned int falseDeliveredMsg = 0;
	// Number of delivered messages
	unsigned int nbDeliveries = 0;
	// Size of control information attached to messages
	unsigned int controlDataSize = 0;
	// Number of done broadcasts
	unsigned int nbBroadcasts = 0;
	// Number of active components when broadcasting a message
	unsigned int localActiveComponentsWhenBroadcast = 0;
} stats_Node;

// Implements nodes
class Node: public cSimpleModule
{
public:
	virtual void initialize() override;
	// Initializes the output gate
	void setOutGate();
	// Sends the init message to the central node such that the central node can associated the gates with the right nodes
	void sendInitMessage();
	virtual void handleMessage(cMessage *msg) override;

	// Performs an application message broadcast
	void sendAppMsg();
	// Creates an application message
	AppMsg* createAppMsg();
	// Handles the reception of an application message
	void RecvAppMsg(AppMsg*m);
	// Tests whether a message can be delivered or not
	bool PC_test(unsigned int id, const DCS &PC, unsigned int PCMsgIncrComponent);
	// Delivers the message of id idMsg
	bool deliverMsg(unsigned int idMsg, unsigned int seqMsg, DCS msgClock, simtime_t rcvTime,
			unsigned int MsgIncrComponent);
	// Tries to deliver pending messages
	void iterativeDelivery();

	// Periodically checks whether the node can deactivate components of its DCS clock
	void PeriodicComponentCheck();
	// Creates an AckComponent message
	AckComponent* createAckComponent();
	// Returns the number of received messages in the last second
	unsigned int nbReceivedLastSecond();
	// Handles the reception of an ackComponent message
	void RecvAckComponent(AckComponent*m);
	// Tests if the component is useful for the delivery of a pending message
	bool componentUsefulToPendingMessage(unsigned int componentIndex, const ProbabilisticClock& component);
	// Creates an AckRep message
	AckRep* createAckRep(bool reply, unsigned int destination, unsigned int component);
	// Handles the reception of AckRep messages
	void RecvAckRep(AckRep* m);
	// Creates a DeleteComponent message
	AckRoundDecision* createDeleteComponent(bool decision, unsigned int sourceId, unsigned int componentIndex);
	// Handles the reception of an AckRoundDecision message
	void RecvAckRoundDecision(AckRoundDecision* m);
	//Removes of obsolete messages from delivered, determined on their reception time
	void clearDelivered();

	// Reference to the delivery controller module
	DeliveryController* control;
	// Reference to the simulation parameters
	SimulationParameters* ut;

	// Sequence number the node will give to the next message it broadcasts
	unsigned int seq = 0;
	// Counter to give unique ids to nodes
	static unsigned int idCounter;
	// The node's unique id
	unsigned int id = idCounter++;
	// Module which manages when to increase or reduce the size of the node's DCS clock
	DCSManagement clockManagment;

	// Contains pending messages
	vector<dep> pendingMsg;
	// Contains delivered messages
	vector<dep> delivered;
	// Contains the reception time of messages
	vector<simtime_t> receivedTime;

	cGate* outGate;

	enum class Delivery
	{
		NOTHING, PCcomparision, DCS
	};
	// Contains the delivery mode, ie whether to deliver messages without any control or using Probabilistic clocks, or using DCS clocks
	Delivery DeliveryControl = Delivery::DCS;

	stats_Node stat;
};

#endif
