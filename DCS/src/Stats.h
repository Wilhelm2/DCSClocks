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

#ifndef __DYNAMICCLOCKSET_STATS_H_
#define __DYNAMICCLOCKSET_STATS_H_

#include <omnetpp.h>
#include <fstream>
#include "Node.h"
#include "structures.h"
#include "Clock/ProbabilisticClock.h"
using namespace omnetpp;

class Node;

class Stats: public cSimpleModule
{
protected:
	virtual void initialize();
	virtual void handleMessage(cMessage *msg);

	vector<Node*> nodes;
	DeliveryController* control;
	Utilitaries* ut;

public:

	void WriteTotalNbHashs();
	void WriteAloneNumbers();
	void WriteControleDataSize();
	void WriteMessageLoad();
	void WriteClockSize();
	void WriteDeliveries();

	std::ofstream aloneNumbers;

	std::ofstream broadcastMessageTimeFile;
	std::ofstream messageLoadFile;
	std::ofstream clockSizeFile;
	std::ofstream deliveriesFile;

	void incrementDelayIntervals(unsigned int entry);
	void incrementMsgSize(unsigned int entry);
	void incrementFalseDetected(unsigned int entry);
	void incrementPlaceOfFalseDetected(unsigned int entry);

	void loadHandler();
	int nextstep;
	float step = 0;

};

#endif
