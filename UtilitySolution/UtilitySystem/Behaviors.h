#pragma once
#include "BehaviorCollection.h"

// Holds a list of all possible behaviors.
class Behaviors
{
public:
	Idle idle;

	std::vector<Behavior*> list;

	Behaviors()
	{
		list =
		{
			&idle
			//...
		};

	};
};
