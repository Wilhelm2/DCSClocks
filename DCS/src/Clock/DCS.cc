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

#include "DCS.h"

DCS::DCS()
{
	Add();
}

DCS::DCS(unsigned int componentSize)
{
	this->componentSize = componentSize;
	Add();
}

DCS::~DCS()
{
}

component DCS::operator[](unsigned int i) const
{
	return clock[i];
}

void DCS::incrementEntries(const vector<unsigned int>& S_incr, const vector<unsigned int>& entries)
{
	for (unsigned int ck : S_incr)
	{
		for (unsigned int i : entries)
		{
			if (ck >= clock.size())
			{
				cerr << "clock smaller than ck" << endl;
				exit(1);
			}
			clock[ck].clock[i]++;
		}
	}
}

bool DCS::satisfiesDeliveryConditions(const DCS& compareClock, const vector<unsigned int>& S_incr,
		const vector<unsigned int>& entries) const
{
	assert(size() >= compareClock.size());
	for (unsigned int i = 0; i < compareClock.size(); i++)
	{
		if (std::find(S_incr.begin(), S_incr.end(), i) != S_incr.end())
		{ // i\in S_incr
			if (!clock[i].clock.satisfiesDeliveryCondition(compareClock.getComponent(i).clock, entries))
				return false;
		}
		else
		{ // i \notin S_incrs
			if (!clock[i].clock.satisfiesDeliveryCondition(compareClock.getComponent(i).clock, { }))
				return false;
		}
	}
	return true;
}

unsigned int DCS::size() const
{
	return clock.size();
}

component DCS::getComponent(unsigned int k) const
{
	return clock[k];
}

unsigned int DCS::prepareComparison(DCS clockCompare)
{
	// Step 1
	while (clock.size() < clockCompare.size())
		Add();

	// Step 2
	for (unsigned int i = 0; i < clockCompare.size(); i++)
	{
		if (!(clockCompare.clock[i].clock <= clock[i].clock) && !clock[i].active)
		{
			ActivateComponent(i);
			activeComponents = max(activeComponents, i + 1);
		}
	}

	// Ensures that activated components have lower ids than inactive ones
	for (unsigned int i = 0; i < activeComponents; i++)
		ActivateComponent(i);
	return activeComponents;
}

void DCS::Add()
{
	component c = { ProbabilisticClock(componentSize), false };
	clock.push_back(c);
}

void DCS::ActivateComponent(unsigned int i)
{
	clock[i].active = true;
}

void DCS::DeactivateComponent(unsigned int i)
{
	clock[i].active = false;
}

void DCS::print()
{
	for (component PC : clock)
		PC.clock.printClock();
}
