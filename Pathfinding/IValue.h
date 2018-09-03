#pragma once
namespace rle
{

	//! The interface value to be used for items in the binary heap.
	class IValue
	{
	public:
		virtual float getValue() = 0;
		virtual float getSecondaryValue() = 0;
		virtual bool isLess(IValue *tile) = 0;
		virtual	bool isMore(IValue *tile) = 0;
		virtual	bool isEqual(IValue *tile) = 0;
		float value = -1.f;
		int index = -1;
	};
}