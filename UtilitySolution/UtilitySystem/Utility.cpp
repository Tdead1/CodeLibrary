#include "Utility.h"

Utility::Utility(std::vector<Unit*>* units)
{
	unitList = units;
	rawUnitArray = new Unit[MAXUNITAMOUNT];
}

Unit * Utility::AddUnit()
{
	int counter = 0;
	for (auto i : *unitList)
	{
		if (i == nullptr)
		{
			Human x;
			//x.id = counter;
			rawUnitArray[counter] = x;
			i = &rawUnitArray[counter];
			return i;
		}
		counter++;
	}
	return nullptr;
}

bool Utility::RemoveUnit(Unit* unit)
{
	for (auto i : *unitList)
	{
		if (i == unit)
		{
			delete(i);
			i = nullptr;
			return true;
		}
	}
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

	//~14ms (1.200.000 elements in debug x86) for increasing and then decreasing an int.
	//Slower now.
	for (auto i : *unitList) 
	{
		if (i != nullptr)
		{
			//i->stats;
			//i->hp.SetStat(i->hp.GetStat() + 1.0f);
			//i->hp.SetStat(i->hp.GetStat() - 1.0f);
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

	duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() * 0.000001;
}
