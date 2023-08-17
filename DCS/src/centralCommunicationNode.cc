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

void CentralCommunicationNode::initialize() {
	ut = dynamic_cast<Utilitaries*>(getModuleByPath("DynamicClockSet.ut"));
	memset(delayIntervals, 0, sizeof(delayIntervals));
	scheduleAt(simTime() + 1. / ut->load, broadcastTimer);
	gateToTarget.resize(ut->nbNodes);
}

void CentralCommunicationNode::handleMessage(cMessage *msg) {
	if (msg == broadcastTimer) {
		cout << "ORDER NODE " << nodeToBroadcast << " to broadcast a message "
				<< endl;
		send(new BroadcastNotify(), gateToTarget[nodeToBroadcast]);

		nodeToBroadcast = (nodeToBroadcast + 1) % ut->nbNodes;
		scheduleAt(simTime() + 1. / ut->load, broadcastTimer);
	} else if (Init* m = dynamic_cast<Init*>(msg)) {
		std::string output = std::regex_replace(
				m->getArrivalGate()->getFullName(),
				std::regex("[^0-9]*([0-9]+).*"), std::string("$1"));
		gateToTarget[m->getSourceId()] = this->gate("neighbour$o",
				stoi(output));
		delete msg;
	} else if (AppMsg* m = dynamic_cast<AppMsg*>(msg)) {
		for (unsigned int i = 0; i < gateToTarget.size(); i++) {
			if (i == m->getSourceId()) // ne renvoie pas le msg à la source
				continue;
			int delay;
			if ((ut->channelRandNumber[m->getSourceId()]
					+ ut->channelRandNumber[i]) % 2 == 0) {
				delay = (*ut->distributionChannelDelayPair)(
						ut->generatorChannelDelay);
				//                stat->nbPairSend ++;
				nbSendPair++;
			} else {
				delay = (*ut->distributionChannelDelayImpair)(
						ut->generatorChannelDelay);
				//                stat->nbImpairSend++;
				nbSendImpair++;
			}
			if (delay < 0)
				delay = 0;
			incrementDelayIntervals((int) (delay / 10000));
			m->setDelay(delay);
			send(m->dup(), gateToTarget[i]);
		}
		delete msg;
	} else if (AckComponent* m = dynamic_cast<AckComponent*>(msg)) {
		for (unsigned int i = 0; i < gateToTarget.size(); i++) {
			if (i == m->getSourceId()) // ne renvoie pas le msg à la source
				continue;
			int delay;
			if ((ut->channelRandNumber[m->getSourceId()]
					+ ut->channelRandNumber[i]) % 2 == 0) {
				delay = (*ut->distributionChannelDelayPair)(
						ut->generatorChannelDelay);
				nbSendPair++;
				//                stat->nbPairSend ++;
			} else {
				delay = (*ut->distributionChannelDelayImpair)(
						ut->generatorChannelDelay);
				nbSendImpair++;
				//                stat->nbImpairSend++;
			}
			if (delay < 0)
				delay = 0;
			incrementDelayIntervals((int) (delay / 10000));

			send(m->dup(), gateToTarget[i]);
		}
		delete msg;
	} else if (DeleteComponent* m = dynamic_cast<DeleteComponent*>(msg)) {
		for (unsigned int i = 0; i < gateToTarget.size(); i++) {
			if (i == m->getSourceId()) // ne renvoie pas le msg à la source
				continue;
			int delay;
			if ((ut->channelRandNumber[m->getSourceId()]
					+ ut->channelRandNumber[i]) % 2 == 0) {
				nbSendPair++;
				delay = (*ut->distributionChannelDelayPair)(
						ut->generatorChannelDelay);
				//                stat->nbPairSend ++;
			} else {
				delay = (*ut->distributionChannelDelayImpair)(
						ut->generatorChannelDelay);
				nbSendImpair++;
				//                stat->nbImpairSend++;
			}
			if (delay < 0)
				delay = 0;
			incrementDelayIntervals((int) (delay / 10000));

			send(m->dup(), gateToTarget[i]);
		}
		delete msg;
	}

	else if (AckRep* m = dynamic_cast<AckRep*>(msg)) {
		for (unsigned int i = 0; i < gateToTarget.size(); i++) {
			int delay;
			if ((ut->channelRandNumber[m->getSourceId()]
					+ ut->channelRandNumber[i]) % 2 == 0) {
				nbSendPair++;
				delay = (*ut->distributionChannelDelayPair)(
						ut->generatorChannelDelay);
				//                stat->nbPairSend ++;
			} else {
				delay = (*ut->distributionChannelDelayImpair)(
						ut->generatorChannelDelay);
				nbSendImpair++;
				//                stat->nbImpairSend++;
			}
			if (delay < 0)
				delay = 0;
			incrementDelayIntervals((int) (delay / 10000));
			if (i == m->getIdDest()) {
				send(m->dup(), gateToTarget[i]);
				break;
			}
		}
		delete msg;
	} else {
		cerr << "Error message not found" << msg << endl;
		exit(1);
	}
}

void CentralCommunicationNode::incrementDelayIntervals(unsigned int entry) {
	if (entry < (sizeof(delayIntervals) / sizeof(int)))
		delayIntervals[entry]++;
	else {
		cerr << "INCREMENTS delayIntervals OUT OF BOUND :" << entry
				<< " bound: " << sizeof(delayIntervals) / sizeof(int) << endl;
		throw "INCREMENTS delayIntervals OUT OF BOUND";
	}
}
