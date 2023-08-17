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
#include "Messages/DeleteComponent_m.h"
#include "Messages/broadcastNotify_m.h"
//#include "depReq_m.h"
//#include "depRsp_m.h"
#include "Messages/Init_m.h"
#include "DeliveryController.h"
#include "Utilitaries.h"

using namespace omnetpp;
using namespace std;

class CentralCommunicationNode: public cSimpleModule {
protected:
	virtual void initialize();
	virtual void handleMessage(cMessage *msg);
	void incrementDelayIntervals(unsigned int entry);

	vector<cGate*> gateToTarget;
	Utilitaries* ut;

	int nbSendPair = 0;
	int nbSendImpair = 0;
	// pour les délais
	int delayIntervals[200];

	int nodeToBroadcast = 0; // id of next node that will broadcast a message
	cMessage* broadcastTimer = new cMessage(); // cMessage pour déterminer quand va broadcast le prochain message

};

#endif
