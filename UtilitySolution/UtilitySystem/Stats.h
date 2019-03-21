#pragma once

class IStat
{
	// Input the ideal stat for this to happen, returns -1.0f if not the same type.
	virtual float GetPriority(IStat* idealstat) = 0;
};

template<class Type> 
class Stat : public IStat
{
public:
	Type value;
};


class ExactIdealFloat : public Stat<float>
{
	float GetPriority(IStat* idealstat) override
	{
		ExactIdealFloat* idealFloat = static_cast<ExactIdealFloat*>(idealstat);
		if (idealFloat == nullptr)
			return -1.0f;

		if (this->value >= idealFloat->value)
			return this->value / idealFloat->value;
		else
			return idealFloat->value / this->value;
	};
};

class MinIdealFloat : public Stat<float>
{
	float GetPriority(IStat* idealstat) override
	{
		MinIdealFloat* idealFloat = static_cast<MinIdealFloat*>(idealstat);
		if (idealFloat == nullptr)
			return -1.0f;

		if (this->value > idealFloat->value)
			return idealFloat->value / this->value;
		else
			return 1.0f;
	};
};

class MaxIdealFloat : public Stat<float>
{
	float GetPriority(IStat* idealstat) override
	{
		MaxIdealFloat* idealFloat = static_cast<MaxIdealFloat*>(idealstat);
		if (idealFloat == nullptr)
			return -1.0f;

		if (this->value < idealFloat->value)
		{		
			return this->value / idealFloat->value;
		}
		else
			return 1.0f;
	};
};

/* Ideal vector stats? Ideal Position? Ideal alignment Vector2(lawfulness, evilness)? Maybe even ideal Transform? */