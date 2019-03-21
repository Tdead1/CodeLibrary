#include "Utility.h"

Utility::Utility(std::vector<Unit*>* units)
{
	unitList = units;
	rawUnitList = new Unit[MAXUNITAMOUNT];
}

Unit* Utility::AddUnit()
{
	int counter = 0;
	for (auto i : *unitList)
	{
		counter++;
		if (i == nullptr)
		{
			Unit x;
			x.id = counter;
			rawUnitList[counter] = x;
			i = &rawUnitList[counter];
			return i;
		}
	}
}

bool Utility::RemoveUnit(Unit* unit)
{
	for (auto i : *unitList)
	{ 
		if (i == unit)
		{
			delete(i);
			i = nullptr;
			// If it hits this, it has erased an element.
			return true;
		}
	}
	// If it hits this, it hasn't erased an element.
	return false;
}

void Utility::ResetSystem()
{
	for (int i = unitList->size() - 1; i >= 0; --i)
	{
		delete(unitList->at(i));
	}
	unitList->clear();
}

void Utility::Update()
{
	auto start = std::chrono::steady_clock::now();

	//13 - 16 ms for 1200000 elements in debug x86
	for (auto i : *unitList) 
	{
		if (i != nullptr)
		{
			i->hp += 1;
			i->hp -= 1;
		}
	}

	//~556 ms
	//for(int i = unitList->size() - 1; i >= 0; --i)
	//{
	//	unitList->at(i)->hp += 1;
	//	unitList->at(i)->hp -= 1;
	//}

	//~300 ms
	//auto last = unitList->end();
	//Unit* i;
	//for (auto it = unitList->begin(); it != last; ++it)
	//{
	//	i = *it;
	//	i->hp += 1;
	//	i->hp -= 1;
	//}



	auto end = std::chrono::steady_clock::now();

	duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}
