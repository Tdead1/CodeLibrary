#include <iostream>
#include <conio.h>
#include <windows.h>

#include "Utility.h"

int main()
{
	std::vector<Unit*> units;
	units.resize(MAXUNITAMOUNT * sizeof(Unit*));
	Utility ai = Utility(&units);

	// Fills up unit list.
	for(int i = 0; i < MAXUNITAMOUNT; i++)
	{
		ai.AddUnit();
	}
	std::cout << units.size() << "\n";
	
	
	while (true)
	{
		ai.Update();
		std::cout << ai.duration << "\n";
		if (GetAsyncKeyState(VK_ESCAPE))
		{
			ai.ResetSystem();
			break;
		}
	}

	std::cout << units.size() << "\n";
	return 0;
}