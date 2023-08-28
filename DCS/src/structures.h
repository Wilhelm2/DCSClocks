#ifndef STRUCTURES_H_
#define STRUCTURES_H_

#include <vector>
#include <omnetpp.h>

#include "Clock/DCS.h"

using namespace std;
using namespace omnetpp;

// Unique identificator of messages
typedef struct sIdMsg
{
	unsigned int id; // id of the node that sent the message
	unsigned int seq; // sequence number that the message m's sender attributed to m
} idMsg;

// Contains the relevant information about messages
typedef struct s_dep
{
	idMsg id; // The message's id
	simtime_t recvtime; // Time at which the message was received
	unsigned int incrComponent; // Component incremented by the sender of the message when broadcasting the message
	DCS PC; // The message's DCS clock

	s_dep(unsigned int id, unsigned int seq, simtime_t recvtime, unsigned int incrComponent, DCS PC)
	{
		this->id =
		{	id,seq};
		this->recvtime = recvtime;
		this->incrComponent = incrComponent;
		this->PC = PC;
	}
}dep;

#endif /* STRUCTURES_H_ */
