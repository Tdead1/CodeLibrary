#pragma once
#include "Stats.h"

class Unit;

class Behavior
{
public:
	std::vector<IStat*> stats;
	virtual void Logic(Unit* owner) = 0;
};

struct Idle : public Behavior
{
	void Logic(Unit* owner) override {};
};

struct Flee : public Behavior
{
	MaxIdealFloat dexterity;
	MinIdealFloat health;

	Flee();
	void Logic(Unit* owner) override;
};

struct MeleeAttack : public Behavior
{
	MaxIdealFloat meleeDamage;

	MeleeAttack();
	void Logic(Unit* owner) override;
};

struct RangedAttack : public Behavior
{
	MaxIdealFloat rangedDamage;
	
	RangedAttack();
	void Logic(Unit* owner) override;
};