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

DCSManagement::DCSManagement()
{
}

DCSManagement::~DCSManagement()
{
}

bool DCSManagement::expandDecision(unsigned int nbReceivedMessages)
{
	if ((simTime() - lastExpand < 0.5) || (simTime() - lastReduce) < 0.5 || ackData.inAckRound)
		return false;

	if (nbReceivedMessages > 10 && clock[0].clock.size() * clock.activeComponents < 20)
		return true;
	if (nbReceivedMessages > 25 && clock[0].clock.size() * clock.activeComponents < 50)
		return true;
	if (nbReceivedMessages > 33 && clock[0].clock.size() * clock.activeComponents < 100)
		return true;
	if (nbReceivedMessages > 45 && clock[0].clock.size() * clock.activeComponents < 150)
		return true;
	if (nbReceivedMessages > 53 && clock[0].clock.size() * clock.activeComponents < 200)
		return true;
	if (nbReceivedMessages > 65 && clock[0].clock.size() * clock.activeComponents < 300)
		return true;
	if (nbReceivedMessages > 75 && clock[0].clock.size() * clock.activeComponents < 400)
		return true;
	if (nbReceivedMessages > 80 && clock[0].clock.size() * clock.activeComponents < 500)
		return true;
	return false;
}

bool DCSManagement::reduceDecision(unsigned int nbReceivedMessages)
{
	if ((simTime() - lastExpand < 0.5) || (simTime() - lastReduce) < 0.5 || ackData.inAckRound
			|| clock.activeComponents == 1)
		return false;
	if (simTime() > 49 && simTime() < 100)
		return false; // POUR DES EXPÉRIENCES !!

	if (nbReceivedMessages < 10 && clock[0].clock.size() * clock.activeComponents > 20)
		return true;
	if (nbReceivedMessages < 22 && clock[0].clock.size() * clock.activeComponents > 50)
		return true;
	if (nbReceivedMessages < 33 && clock[0].clock.size() * clock.activeComponents > 100)
		return true;
	if (nbReceivedMessages < 40 && clock[0].clock.size() * clock.activeComponents > 150)
		return true;
	if (nbReceivedMessages < 45 && clock[0].clock.size() * clock.activeComponents > 200)
		return true;
	if (nbReceivedMessages < 65 && clock[0].clock.size() * clock.activeComponents > 300)
		return true;
	if (nbReceivedMessages < 75 && clock[0].clock.size() * clock.activeComponents > 400)
		return true;
	if (nbReceivedMessages < 80 && clock[0].clock.size() * clock.activeComponents > 500)
		return true;
	return false;
}

void DCSManagement::reduceLocalActiveComponents()
{
	nbLocalActiveComponents = clock.activeComponents - 1;
	incrComponent = rand() % nbLocalActiveComponents;
	lastReduce = simTime();
}

void DCSManagement::increaseLocalActiveComponents()
{
	nbLocalActiveComponents++;
	incrComponent = rand() % nbLocalActiveComponents;
	lastExpand = simTime();
}

void DCSManagement::expandClock()
{
	increaseLocalActiveComponents();
	clock.ActivateComponent(incrComponent);
	clock.activeComponents++;
	if (clock.activeComponents > clock.size())
		clock.Add();
}

void DCSManagement::reduceClock()
{
	clock.activeComponents--;
	clock.DeactivateComponent(clock.activeComponents);
}

bool DCSManagement::acknowledgesComponentDeactivation(unsigned int componentIndex, ProbabilisticClock component)
{
	assert(componentIndex > clock.size() && "Requests to delete component not in local DCS!");
	bool reply = true;

	if (incrComponent == componentIndex - 1)
	{
		reply = false;
		cerr << "Replies no because increments this component" << endl;
	}
	if (timeIncrLastComponent > simTime() - SimTime(300, SIMTIME_MS))
	{
		reply = false;
		cerr << "Replies no because incremented the component less than 0.3s ago" << endl;
	}

	if (!(component == clock[componentIndex - 1].clock))
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
