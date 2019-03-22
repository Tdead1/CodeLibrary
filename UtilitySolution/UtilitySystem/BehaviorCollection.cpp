#include "BehaviorCollection.h"
#include "Units.h"

// List all available behaviors here:

Flee::Flee()
{
	health.value = 0.0f;
	dexterity.value = 1.0f;

	stats.push_back(&health);
	stats.push_back(&dexterity);
}
void Flee::Logic(Unit * owner)
{
	return;
}



MeleeAttack::MeleeAttack()
{
	meleeDamage.value = 1.0f;
}
void MeleeAttack::Logic(Unit * owner)
{
	return;
}



RangedAttack::RangedAttack()
{
	rangedDamage.value = 1.0f;
}
void RangedAttack::Logic(Unit * owner)
{
	return;
}


