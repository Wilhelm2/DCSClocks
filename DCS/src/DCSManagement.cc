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

#include "DCSManagement.h"

bool DCSManagement::expandDecision(unsigned int nbReceivedMessages)
{
	if ((simTime() - lastExpand < 0.5) || (simTime() - lastReduce) < 0.5 || ackData.inAckRound)
		return false;

	unsigned int clockSize = clock[0].clock.size() * clock.activeComponents;
	if (nbReceivedMessages > 10 && clockSize < 20)
		return true;
	if (nbReceivedMessages > 25 && clockSize < 50)
		return true;
	if (nbReceivedMessages > 33 && clockSize < 100)
		return true;
	if (nbReceivedMessages > 45 && clockSize < 150)
		return true;
	if (nbReceivedMessages > 53 && clockSize < 200)
		return true;
	if (nbReceivedMessages > 65 && clockSize < 300)
		return true;
	if (nbReceivedMessages > 75 && clockSize < 400)
		return true;
	if (nbReceivedMessages > 80 && clockSize < 500)
		return true;
	return false;
}

bool DCSManagement::reduceDecision(unsigned int nbReceivedMessages)
{
	if ((simTime() - lastExpand < 0.5) || (simTime() - lastReduce) < 0.5 || ackData.inAckRound
			|| clock.activeComponents == 1)
		return false;

	unsigned int clockSize = clock[0].clock.size() * clock.activeComponents;
	if (nbReceivedMessages < 10 && clockSize > 20)
		return true;
	if (nbReceivedMessages < 22 && clockSize > 50)
		return true;
	if (nbReceivedMessages < 33 && clockSize > 100)
		return true;
	if (nbReceivedMessages < 40 && clockSize > 150)
		return true;
	if (nbReceivedMessages < 45 && clockSize > 200)
		return true;
	if (nbReceivedMessages < 65 && clockSize > 300)
		return true;
	if (nbReceivedMessages < 75 && clockSize > 400)
		return true;
	if (nbReceivedMessages < 80 && clockSize > 500)
		return true;
	return false;
}

void DCSManagement::reduceLocalActiveComponents(unsigned int nodeId)
{
	nbLocalActiveComponents = clock.activeComponents - 1;
	reaffectIncrementedComponent(nodeId);
	lastReduce = simTime();
}

void DCSManagement::increaseLocalActiveComponents(unsigned int nodeId)
{
	nbLocalActiveComponents++;
	reaffectIncrementedComponent(nodeId);
	cerr << "chose component " << incrComponent << endl;
	lastExpand = simTime();
}

void DCSManagement::reaffectIncrementedComponent(unsigned int nodeId)
{
//	incrComponent = rand() % nbLocalActiveComponents;
	incrComponent = nodeId % nbLocalActiveComponents;
}

void DCSManagement::expandClock(unsigned int nodeId)
{
	cerr << "Node " << nodeId << "increases active components to " << clock.activeComponents << endl;
	if (nbLocalActiveComponents == clock.size())
	{
		clock.Add();
		clock.ActivateComponent(clock.size() - 1);
		clock.activeComponents++;
	}
	else if (nbLocalActiveComponents == clock.activeComponents)
	{
		clock.ActivateComponent(clock.size() - 1);
		clock.activeComponents++;
	}
	increaseLocalActiveComponents(nodeId);
	// To ensure that this node increments the newly added component such that other processes increment the number of active components
	//clockManagment.incrComponent = clockManagment.clock.activeComponents - 1;
}

void DCSManagement::reduceClock()
{
	clock.activeComponents--;
	clock.DeactivateComponent(clock.activeComponents);
}

bool DCSManagement::acknowledgesComponentDeactivation(unsigned int componentIndex, const ProbabilisticClock& component)
{
	assert(componentIndex <= clock.size() && "Requests to delete component not in local DCS!");
	bool reply = true;

	if (incrComponent == componentIndex)
	{
		reply = false;
		cerr << "Replies no because increments this component" << endl;
	}
	if (timeIncrLastComponent > simTime() - SimTime(300, SIMTIME_MS))
	{
		reply = false;
		cerr << "Replies no because incremented the component less than 0.3s ago" << endl;
	}

	if (!(component == clock[componentIndex].clock))
	{
		reply = false;
		cerr << "Replies no because components are not equal" << endl;
	}
	return reply;
}

bool DCSManagement::acknowledgementDecision(unsigned int nbNodes)
{
	if (ackData.positiveAck == nbNodes)
		return true;
	else
		return false;
}

void DCSManagement::prepareComparison(const DCS& PCMsg, unsigned int idReceiver)
{
	unsigned int newLocalActiveComponents = clock.prepareComparison(PCMsg);
	bool hasActivatedComponents = newLocalActiveComponents != nbLocalActiveComponents;
	nbLocalActiveComponents = newLocalActiveComponents;
	if (hasActivatedComponents)
		reaffectIncrementedComponent(idReceiver);
}

void DCSManagement::IncrementPC(vector<unsigned int> clockEntries, unsigned int componentIndex)
{
	clock.incrementEntries( { componentIndex }, clockEntries);
	if (componentIndex == clock.activeComponents - 1)
		timeIncrLastComponent = simTime(); // keeps track of when incremented the last component
}

void DCSManagement::getAckRoundDecision(bool decision, unsigned int componentIndex, unsigned int nodeId)
{
	if (decision)
	{
		cerr << "Node " << nodeId << " RecvDeleteComponent reply " << componentIndex << endl;
		if (clock.activeComponents == componentIndex + 1)
		{
			if (nbLocalActiveComponents == clock.activeComponents)
				nbLocalActiveComponents--;
			clock.activeComponents--;
		}
	}
	reaffectIncrementedComponent(nodeId);
	ackData.reset();
}

