#pragma once
#include "Behaviors.h"

class Unit
{
public:
	std::vector<IStat*> stats;

	int id;
};

struct Human : public Unit
{
	MaxIdealFloat strength;
	MaxIdealFloat dexterity;
	MaxIdealFloat intelligence;
	MaxIdealFloat health;

	Human()
	{
		strength.value = 4.0f;
		dexterity.value = 4.0f;
		intelligence.value = 4.0f;
		health.value = 5.0f;

		stats.push_back(&health);
		stats.push_back(&strength);
		stats.push_back(&dexterity);
		stats.push_back(&intelligence);
	}
};

struct Soldier : public Human
{
	MaxIdealFloat meleeDamage;

	Soldier()
	{
		strength.value = 5.0f;
		dexterity.value = 3.0f;
		intelligence.value = 3.0f;

		meleeDamage.value = 2.0f;
	}
};

struct Ranger : public Soldier
{
	MaxIdealFloat rangedDamage;
	ExactIdealFloat rangeFromTarget;

	Ranger()
	{
		rangedDamage.value = 3.0f;
		rangeFromTarget.value = 10.0f;

		stats.push_back(&rangeFromTarget);
	}
};

struct Building : public Unit
{
	MaxIdealFloat health;

	Building()
	{
		health.value = 50.0f;
		stats.push_back(&health);
	}
};