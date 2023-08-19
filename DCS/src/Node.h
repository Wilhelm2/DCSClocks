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
#include "Messages/DeleteComponent_m.h"
#include "DeliveryController.h"
#include "Messages/broadcastNotify_m.h"
#include "Utilitaries.h"
#include "Stats.h"
#include "structures.h"
#include "Clock/ProbabilisticClock.h"
#include "Clock/DCS.h"
using namespace omnetpp;
using namespace std;

typedef struct s_stats_Node
{
	int falseDeliveredMsgHashSuccess = 0;
	int falseDeliveredMsgHashFail = 0;
	int rightDeliveredMsgHashSuccess = 0;
	int rightDeliveredMsgHashFail = 0;

	int nbDeliveries = 0;
	int controlDataSize = 0;
	int falseDetectedDependencies = 0;
	int nbHashs = 0;
	int msgSize[2000];
	int nbMsgCombinaisons[LIMIT_HASHS + 1];

	int nbBroadcasts = 0;
	int localActiveComponentsWhenBroadcast = 0;
} stats_Node;

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

class Node: public cSimpleModule
{
public:
	virtual void initialize() override;
	void setOutGate();
	void sendInitMessage();
	virtual void handleMessage(cMessage *msg) override;

	AppMsg* sendAppMsg();
	AppMsg* createAppMsg();
	void RecvAppMsg(AppMsg*m);
	bool PC_test(unsigned int id, const DCS &PC, unsigned int PCMsgIncrComponent);
	void IncrementPC();
	bool deliverMsg(unsigned int idMsg, unsigned int seqMsg, DCS v, simtime_t rcvTime, unsigned int MsgIncrComponent);
	void iterativeDelivery();

	void PeriodicComponentCheck();
	bool expandDecision();
	bool reduceDecision();
	int nbReceivedLastSecond();
	void expandClockCheck();
	void expandClockForce();

	void RecvAckComponent(AckComponent*m);
	AckRep* createAckRep(bool reply, unsigned int destination, unsigned int component);
	void RecvAckRep(AckRep* m);
	void RecvDeleteComponent(DeleteComponent* m);

	void clearDelivered();

	cMessage componentSizeCheckTimer;
	DeliveryController* control;
	Utilitaries* ut;
	unsigned int seq = 0;
	unsigned int id = idCounter++;

	DCS clock;
	unsigned int incrComponent = 0; // Incremented component when broadcasting a message

	cGate* outGate;

	vector<dep> pendingMsg; // msg qui ont pas passé le test PCtest

	vector<dep> delivered;

	enum class Delivery
	{
		NOTHING, PCcomparision, DCS
	};
	Delivery DeliveryControl = Delivery::PCcomparision;

	simtime_t lastExpand = *(new SimTime(-1, SIMTIME_S));
	simtime_t lastReduce = *(new SimTime(0, SIMTIME_S));
#define LIMIT_LOAD_UP 20 // Toutes les tranches de charge de x msg ajoute un composant
#define LIMIT_LOAD_DOWN 10 // Toutes les tranches de charge de x msg retire un composant
	simtime_t measureTime = *(new SimTime(500, SIMTIME_MS)); //*(new SimTime(500, SIMTIME_MS)) ; // temps de mesure (moyenne de hashs ces combien measureTime dernieres secondes)

	struct ackRoundData ackData;
	simtime_t timeIncrLastComponent = 0;

	unsigned int nbLocalActiveComponents = 1;

	stats_Node stat;

	vector<simtime_t> receivedTime;

	static unsigned int idCounter;

	std::ofstream DEBUGRECEPTIONTIME;
};

#endif
