#ifndef STRUCTURES_H_
#define STRUCTURES_H_

#include <vector>
#include <omnetpp.h>

#include "Clock/DCS.h"

using namespace std;
using namespace omnetpp;

typedef struct s_dep
{
	int id;
	int seq;
	simtime_t recvtime;
	int incrComponent;
	DCS PC;

	s_dep(unsigned int id, unsigned int seq, simtime_t recvtime, unsigned int incrComponent, DCS PC)
	{
		this->id = id;
		this->seq = seq;
		this->recvtime = recvtime;
		this->incrComponent = incrComponent;
		this->PC = PC;
	}
} dep;

#endif /* STRUCTURES_H_ */
