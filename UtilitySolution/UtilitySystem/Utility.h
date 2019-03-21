#pragma once

#define MAXUNITAMOUNT 12000000

#include <vector>
#include <chrono>

struct Unit
{
	int id = 0;
	float hp = 0.0f;
	float sta = 0.0f;
};

struct Stat
{
	Stat()			{ val = 0.0f;	};
	Stat(float x)	{ val = x;		};
	float val;
};



class Utility
{
public:
	//! This list will be bound to all units in the Utility system.
	Utility(std::vector<Unit*>* units);

	std::vector<Unit*>* unitList;

	double duration;

	Unit* AddUnit();
	bool RemoveUnit(Unit* unit);
	void ResetSystem();
	void Update();
	
private:
	unsigned long int idCounter = 0;
	Unit* rawUnitList;
};