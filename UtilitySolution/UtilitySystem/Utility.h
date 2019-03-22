#pragma once

#define MAXUNITAMOUNT 120000

#include <chrono>

#include "Units.h"
#include "Behaviors.h"

class Utility
{
public:
	//! This list will be bound to all units in the Utility system.
	Utility(std::vector<Unit*>* units);
	
	
	Unit* AddUnit();
	bool RemoveUnit(Unit* unit);
	void ResetSystem();
	void Update();
	
	std::vector<Unit*>* unitList;
	Behaviors behaviors;

	double duration;
private:
	unsigned long int idCounter = 0;
	Unit* rawUnitArray;
};