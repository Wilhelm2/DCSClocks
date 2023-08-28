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

#ifndef DCS_H_
#define DCS_H_

#include <vector>
#include <cassert>

#include "ProbabilisticClock.h"

// Component of a DCS clock
typedef struct s_component
{
	ProbabilisticClock clock; // The component's Probabilistic clock
	bool active; // Whether the component is active or not
} component;

// Implement a DCS clock
class DCS
{
public:
	DCS();
	DCS(unsigned int componentSize);
	virtual ~DCS() = default;
	component operator[](unsigned int i) const;
	// Increments the entries entries of the components contained in S_incr
	void incrementEntries(const vector<unsigned int>& S_incr, const vector<unsigned int>& entries);
	// Tests whether the clock satisfies the delivery conditions of the DCS clock compareClock
	bool satisfiesDeliveryConditions(const DCS& compareClock, const vector<unsigned int>& S_incr,
			const vector<unsigned int>& entries) const;
	unsigned int size() const;
	component getComponent(unsigned int k) const;
	// Prepares the comparison with the DCS clock clockCompare
	unsigned int prepareComparison(const DCS& clockCompare);
	// Adds a component to the DCS clock
	void Add();
	// Activates component i of the DCS clock
	void ActivateComponent(unsigned int i);
	// Deactivates component i of the DCS clock
	void DeactivateComponent(unsigned int i);
	void print();

	vector<component> clock; // Contains the clock
	unsigned int componentSize = 0; // Size of the DCS clock components
	unsigned int activeComponents = 1; // Number of active components

};

#endif /* DCS_H_ */
