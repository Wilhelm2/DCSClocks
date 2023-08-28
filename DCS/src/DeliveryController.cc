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

#include "DeliveryController.h"

void DeliveryController::initialize(int stage)
{
	cSimpleModule::initialize(stage);
	params = dynamic_cast<SimulationParameters*>(getModuleByPath("DCS.ut"));
	processDependencies.resize(params->nbNodes);
	for (vector<TotalDependencies>::iterator it = processDependencies.begin(); it != processDependencies.end(); it++)
		*it = TotalDependencies(params->nbNodes);
	processBroadcastedMessages.resize(params->nbNodes);
}

void DeliveryController::notifySendMessage(unsigned int idSender, unsigned int seq)
{
	msg t;
	t.seq = seq;
	processDependencies[idSender][idSender]++;
	t.dependencies = processDependencies[idSender];
	t.psHasDelivered.assign(params->nbNodes, false);
	t.sendTime = simTime();
	t.deliveredAtTime.assign(params->nbNodes, 0);
	processBroadcastedMessages[idSender].push_back(t);
//    std::cout << "DeliveryController: MSG ADDED from " << idSrc <<" seq " << t->seq << " || "; printclock(t->clock);
}

vector<msg>::iterator DeliveryController::searchMessage(idMsg idM)
{
	if (processBroadcastedMessages.size() > idM.id)
	{
		for (vector<msg>::iterator it = processBroadcastedMessages[idM.id].begin();
				it != processBroadcastedMessages[idM.id].end(); ++it)
		{
			if (it->seq == idM.seq)
				return it;
		}
	}
	cerr << "Message " << idM.id << "," << idM.seq << " not registered" << endl;
	exit(1);
}

bool DeliveryController::notifyDeliverMessage(idMsg idM, unsigned int idDest)
{
	vector<msg>::iterator m = searchMessage(idM);
	bool correctDelivery = canCausallyDeliverMessage(idM, idDest);

	if (!correctDelivery) // process wants to deliver message out of causal order
	{
//        printDeliveryError("Out of causal order delivery", idM, idDest, m.dependencies, processDependencies[idDest]);
		for (unsigned int i = 0; i < params->nbNodes; i++)
			processDependencies[idDest][i] = max(m->dependencies[i], processDependencies[idDest][i]);
	}
	else
	{
		processDependencies[idDest][idM.id] = max(m->dependencies[idM.id], processDependencies[idDest][idM.id]); // increments local clock of idDest
		if (m->psHasDelivered[idDest])
		{
			printDeliveryError("Multiple delivery", idM, idDest, m->dependencies, processDependencies[idDest]);
			exit(0);
		}
	}

	m->deliveredAtNbPs++;
	m->psHasDelivered[idDest] = true;
	m->deliveredAtTime[idDest] = simTime();
	if (m->deliveredAtNbPs == params->nbNodes)
		deleteMessage(idM);
	return correctDelivery;
}

void DeliveryController::printDeliveryError(string errorReason, idMsg idM, unsigned int destProcess,
		TotalDependencies messageDependencies, TotalDependencies processDependencies)
{
	cerr << "DeliveryController " << errorReason << " idMsg " << idM.id << "," << idM.seq << " destination "
			<< destProcess << endl;
	cerr << "Process clock: ";
	processDependencies.printErr();
	cerr << "Message clock: ";
	messageDependencies.printErr();
}

bool DeliveryController::canCausallyDeliverMessage(idMsg idM, unsigned int idDest)
{
	vector<msg>::iterator m = searchMessage(idM);
	return (processDependencies[idDest].SatisfiesDeliveryConditions(m->dependencies, idM.id));
}

void DeliveryController::deleteMessage(idMsg idM)
{
	vector<msg>::iterator it;
	for (it = processBroadcastedMessages[idM.id].begin(); it != processBroadcastedMessages[idM.id].end(); ++it)
		if (it->seq == idM.seq)
			break;

	if (it == processBroadcastedMessages[idM.id].end())
	{
		cerr << "Message not found" << endl;
		exit(1);
	}
	processBroadcastedMessages[idM.id].erase(it);
	return;
}

bool DeliveryController::isDependency(idMsg idM, idMsg idDep)
{
	vector<msg>::iterator m = searchMessage(idM);
	return m->dependencies[idDep.id] >= idDep.seq;
}

