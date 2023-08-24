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

#include "centralCommunicationNode.h"

Define_Module(CentralCommunicationNode);

void CentralCommunicationNode::initialize()
{
	ut = dynamic_cast<Utilitaries*>(getModuleByPath("DCS.ut"));
	memset(delayIntervals, 0, sizeof(delayIntervals));
	gateToTarget.resize(ut->nbNodes);

	scheduleAt(simTime() + 1. / ut->load, &broadcastTimer);
}

void CentralCommunicationNode::handleMessage(cMessage *msg)
{
	if (msg == &broadcastTimer)
		handleBroadcastNotify();
	else if (msg->isSelfMessage()) // delay passed, send to target
		send(msg, gateToTarget[getTargetId(msg)]);
	else
	{
		if (Init* m = dynamic_cast<Init*>(msg))
			handleInit(m);
		else if (AckRep* m = dynamic_cast<AckRep*>(msg))
			handleAckRep(m);
		else
			broadcastMessage(msg);
	}
}

unsigned int CentralCommunicationNode::getSourceId(cMessage* msg)
{
	unsigned int sourceId;
	if (AppMsg* m = dynamic_cast<AppMsg*>(msg))
		sourceId = m->getSourceId();
	else if (AckComponent* m = dynamic_cast<AckComponent*>(msg))
		sourceId = m->getSourceId();
	else if (AckRoundDecision* m = dynamic_cast<AckRoundDecision*>(msg))
		sourceId = m->getSourceId();
	else if (AckRep* m = dynamic_cast<AckRep*>(msg))
		sourceId = m->getSourceId();
	else if (Init* m = dynamic_cast<Init*>(msg))
		sourceId = m->getSourceId();
	else
	{
		cerr << "Message type not found" << msg << endl;
		exit(1);
	}
	return sourceId;
}

unsigned int CentralCommunicationNode::getTargetId(cMessage* msg)
{
	unsigned int targetId;
	if (AppMsg* m = dynamic_cast<AppMsg*>(msg))
		targetId = m->getTargetId();
	else if (AckComponent* m = dynamic_cast<AckComponent*>(msg))
		targetId = m->getTargetId();
	else if (AckRoundDecision * m = dynamic_cast<AckRoundDecision*>(msg))
		targetId = m->getTargetId();
	else if (AckRep* m = dynamic_cast<AckRep*>(msg))
		targetId = m->getTargetId();
	else if (Init* m = dynamic_cast<Init*>(msg))
		targetId = m->getTargetId();
	else
	{
		cerr << "Message type not found" << msg << endl;
		exit(1);
	}
	return targetId;
}

void CentralCommunicationNode::setTargetId(cMessage* msg, unsigned int targetId)
{
	if (AppMsg* m = dynamic_cast<AppMsg*>(msg))
		m->setTargetId(targetId);
	else if (AckComponent* m = dynamic_cast<AckComponent*>(msg))
		m->setTargetId(targetId);
	else if (AckRoundDecision * m = dynamic_cast<AckRoundDecision*>(msg))
		m->setTargetId(targetId);
	else if (AckRep* m = dynamic_cast<AckRep*>(msg))
		m->setTargetId(targetId);
	else if (Init* m = dynamic_cast<Init*>(msg))
		m->setTargetId(targetId);
	else
	{
		cerr << "Message type not found" << msg << endl;
		exit(1);
	}
}

void CentralCommunicationNode::handleBroadcastNotify()
{
	cout << "Order node " << nodeToBroadcast << " to broadcast a message " << endl;
	send(new BroadcastNotify(), gateToTarget[nodeToBroadcast]);
	nodeToBroadcast = (nodeToBroadcast + 1) % ut->nbNodes;
	scheduleAt(simTime() + 1. / ut->load, &broadcastTimer);
}

void CentralCommunicationNode::handleInit(Init* m)
{
	std::string output = std::regex_replace(m->getArrivalGate()->getFullName(), std::regex("[^0-9]*([0-9]+).*"),
			std::string("$1"));
	gateToTarget[m->getSourceId()] = this->gate("neighbour$o", stoi(output));
	delete m;
}

void CentralCommunicationNode::broadcastMessage(cMessage* m)
{
	for (unsigned int i = 0; i < gateToTarget.size(); i++)
	{
		if (i == getSourceId(m)) // don't send message back to source
			continue;
		cMessage* copy = m->dup();
		setTargetId(copy, i);
		scheduleAt(simTime() + SimTime(computeDelay(getSourceId(m), i), SIMTIME_US), copy);
	}
	delete m;
}

unsigned int CentralCommunicationNode::computeDelay(unsigned int sourceId, unsigned int targetId)
{
	double delay;
	if ((ut->channelRandNumber[sourceId] + ut->channelRandNumber[targetId]) % 2 == 0)
	{
		delay = (*ut->distributionChannelDelayPair)(ut->generatorChannelDelay);
		nbSendPair++;
	}
	else
	{
		delay = (*ut->distributionChannelDelayImpair)(ut->generatorChannelDelay);
		nbSendImpair++;
	}
//	if (delay / 10000 > 40000)
//		cerr << "sourceid " << sourceId << " targetid " << targetId << " ut " << ut << " delay " << delay << endl;
//	cerr << (*ut->distributionChannelDelayPair)(ut->generatorChannelDelay) << endl;

	incrementDelayIntervals((delay / 10000));
	return max(delay, 0.);
}

void CentralCommunicationNode::handleAckRep(AckRep* m)
{
	scheduleAt(SimTime(computeDelay(m->getSourceId(), m->getIdDest()), SIMTIME_US), m);
}

void CentralCommunicationNode::incrementDelayIntervals(unsigned int entry)
{
	if (entry < (sizeof(delayIntervals) / sizeof(int)))
		delayIntervals[entry]++;
	else
	{
		cerr << "INCREMENTS delayIntervals OUT OF BOUND :" << entry << " bound: "
				<< sizeof(delayIntervals) / sizeof(int) << endl;
		exit(1);
	}
}
