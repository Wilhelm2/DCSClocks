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

// Takes the statistics, writes them into files and prints them on the error display
class Stats: public cSimpleModule
{
private:
	virtual void initialize();
	void openFiles();
	virtual void handleMessage(cMessage *msg);

	// File containing final simulation numbers
	std::ofstream aloneNumbers;
	// File containing the system's message load second per second
	std::ofstream messageLoadFile;
	// File containing information about the DCS clock sizes second per second
	std::ofstream clockSizeFile;
	// File containing the number of messages delivered out of causal order second per second
	std::ofstream deliveriesFile;

	// References to the system's nodes
	vector<Node*> nodes;
	// Reference to the simulation parameters
	SimulationParameters* ut;
	// Timer to take statistics
	cMessage timer;

public:

	// Writes the final simulation numbers to a file
	void WriteAloneNumbers();
	// Writes the control data to a file
	void WriteControleDataSize();
	// Writes the message load to a file
	void WriteMessageLoad();
	// Writes the clock sizes to a file
	void WriteClockSize();
	// Writes the number of false delivered messages to a file
	void WriteFalseDeliveries();

	// Prints on the error display the number of pending messages for each node
	void printErrPendingMessages();
	// Prints on the error display the number of delivered messaes for each node
	void printErrNbDeliveredMessages();
	// Prints on the error display some stats about each node
	void printErrNodeStats();
	// Prints on the error display how many nodes increment every component of the system's DCS clock
	void printErrIncrementedComponents();

};

#endif
