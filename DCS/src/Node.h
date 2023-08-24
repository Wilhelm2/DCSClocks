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
#include "Utilitaries.h"
#include "Stats.h"
#include "structures.h"
#include "Clock/ProbabilisticClock.h"
#include "Clock/DCS.h"
#include "DCSManagement.h"

using namespace omnetpp;
using namespace std;

typedef struct s_stats_Node
{
	unsigned int falseDeliveredMsg = 0;
	int nbDeliveries = 0;
	int controlDataSize = 0;

	int nbBroadcasts = 0;
	int localActiveComponentsWhenBroadcast = 0;
} stats_Node;

class Node: public cSimpleModule
{
public:
	virtual void initialize() override;
	void setOutGate();
	void sendInitMessage();
	virtual void handleMessage(cMessage *msg) override;

	void sendAppMsg();
	AppMsg* createAppMsg();
	void RecvAppMsg(AppMsg*m);
	bool PC_test(unsigned int id, const DCS &PC, unsigned int PCMsgIncrComponent);
	bool deliverMsg(unsigned int idMsg, unsigned int seqMsg, DCS v, simtime_t rcvTime, unsigned int MsgIncrComponent);
	void iterativeDelivery();

	void PeriodicComponentCheck();
	AckComponent* createAckComponent();
	unsigned int nbReceivedLastSecond();
	void expandClockCheck();

	void RecvAckComponent(AckComponent*m);
	bool componentUsefulToPendingMessage(unsigned int componentIndex, ProbabilisticClock component);
	AckRep* createAckRep(bool reply, unsigned int destination, unsigned int component);
	void RecvAckRep(AckRep* m);
	AckRoundDecision* createDeleteComponent(bool decision, unsigned int sourceId, unsigned int componentIndex);
	void RecvAckRoundDecision(AckRoundDecision* m);

	void clearDelivered();

	DeliveryController* control;
	Utilitaries* ut;
	unsigned int seq = 0;
	unsigned int id = idCounter++;
	vector<dep> pendingMsg;
	vector<dep> delivered;
	vector<simtime_t> receivedTime;

	cGate* outGate;

	DCSManagement clockManagment;

	enum class Delivery
	{
		NOTHING, PCcomparision, DCS
	};
	Delivery DeliveryControl = Delivery::DCS;

	stats_Node stat;

	static unsigned int idCounter;

	std::ofstream DEBUGRECEPTIONTIME;
};

#endif
