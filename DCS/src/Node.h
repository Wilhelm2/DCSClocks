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
using namespace omnetpp;
using namespace std;

typedef struct s_stats_Node {
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

class Node: public cSimpleModule {
public:
	virtual void initialize() override;
	virtual void handleMessage(cMessage *msg) override;
	void PeriodicComponentCheck();
	int calculateDelay(int idTarget);
	AppMsg* createAppMsg();
	void IncrementPC();
	void expandClockCheck();
	void expandClockForce();
	bool AchourErrorDetection(int idMsg, const vector<vector<int>> &PCMsg,
			int MsgincrComponent);
	bool PC_test(int id, const vector<ProbabilisticClock> &PC,
			int PCMsgIncrComponent);
	void iterativeDelivery();
	void iterativeDeliveryPCcomparision();
	bool searchDep(int id, int seq);
	bool deliverMsg(int idMsg, int seqMsg, const vector<ProbabilisticClock> v,
			simtime_t rcvTime, int MsgIncrComponent);
	int nbDeliveriesLastSecond();
	int nbReceivedLastSecond();
	void RecvAppMsg(AppMsg*m);
	void RecvAckComponent(AckComponent*m);
	void RecvAckRep(AckRep* m);
	void RecvDeleteComponent(DeleteComponent* m);

	void clearDelivered();
	void recoveryAdd(vector<int> copieModele, dep d, vector<int> mPC);
	simtime_t maxtime(simtime_t t1, simtime_t t2);
	bool expandDecision();
	bool reduceDecision();

	cMessage componentSizeCheckTimer;
	unsigned int seq = 0;
	unsigned int id = idCounter++;
	vector<ProbabilisticClock> PC;
	unsigned int incrComponent = 0; // composant de PC incrémenté lorsque broadcast un message

	DeliveryController* control;
	Utilitaries* ut;

	cGate* outGate;

	vector<tuple<dep, vector<ProbabilisticClock>>> pendingMsg; // msg qui ont pas passé le test PCtest

	vector<tuple<dep, vector<ProbabilisticClock> > > delivered;

	vector<int> vectorClockDelivered; // pour de meilleures performances

	enum Delivery {
		NOTHING, PCcomparision, ACHOUR, RECOVERY
	};
	int DeliveryControl = RECOVERY;

	hash<string> hasher;

	int failedHashTestPerSecond = 0; // to count the number of failed hash computation per second to decide when to expand the vector clock
	simtime_t lastExpand = *(new SimTime(-1, SIMTIME_S));
	simtime_t lastReduce = *(new SimTime(0, SIMTIME_S));
#define LIMITFAILEDHASHTESTEXPAND 10
#define LIMITFAILEDHASHTESTREDUCE 1
#define UPLIMIT_NB_HASHTESTS_SECOND 20
#define DOWNLIMIT_NB_HASHTESTS_SECOND 10
#define LIMIT_LOAD_UP 20 // Toutes les tranches de charge de x msg ajoute un composant
#define LIMIT_LOAD_DOWN 10 // Toutes les tranches de charge de x msg retire un composant
	simtime_t measureTime = *(new SimTime(500, SIMTIME_MS)); //*(new SimTime(500, SIMTIME_MS)) ; // temps de mesure (moyenne de hashs ces combien measureTime dernieres secondes)

	int ackPositifs = 0;
	int ackNegatifs = 0;
	simtime_t timeIncrLastComponent = 0;
	bool currentACKRound = false; // pour bloquer l'expand de l'horloge lors d'un round ack (car durant le round se réaffecte à un composant inférieur)

	int nbActiveComponents = 1;
	int nbLocalActiveComponents = 1;

	stats_Node stat;

	vector<simtime_t> deliveriesTime;
	vector<simtime_t> receivedTime;

	static int idCounter;

	std::ofstream DEBUGRECEPTIONTIME;

};

#endif
