#pragma once
#include "IValue.h"

#include <vector>
#include <iostream>
namespace rle
{
	//! The binary heap will arange for fast sorted storage for the open list in the pathfinder.
	template <class T, class = typename std::enable_if<std::is_base_of<IValue, T>::value>::type>
	class BinaryHeap
	{
	public:
		BinaryHeap() {};
		~BinaryHeap() {};
		//! The list of items in the heap.
		std::vector<T*> items;

		//! Adds an item to the heap.
		void addItem(T *item)
		{
			items.push_back(item);
			item->index = (int)items.size() - 1;
			sortUp(item);
			return;
		}

		//! removes the first item from the heap (lowest value).
		T *removeFirst()
		{
			T* first = items[0];
			items.erase(items.begin());
			for (int i = 0; i < items.size(); i++)
			{
				items[i]->index -= 1;
			}
			sortDown(first);
			first->index = -1;
			return first;
		}

		//! Sorts the array upwards (from low to high).
		void sortUp(T *item)
		{
			// Using while true because it's more readable.
			while (true)
			{
				int parentIndex = (item->index - 1) / 2;

				if (item->isLess(items[parentIndex]))
				{
					swap(item->index, parentIndex);
				}
				else return;
			}
		}

		//! Sorts the array down (from item id 0 to last).
		void sortDown(T *item)
		{
			int swapIndex = 0;
			int index_left = item->index * 2 + 1;
			int index_right = item->index * 2 + 2;
			while (true)
			{
				// If indexleft is valid...
				if (index_left < (int)items.size())
				{
					// If indexright is invalid
					if (index_right < (int)items.size())
					{
						if (items[index_left]->isLess(items[item->index]))
						{
							swapIndex = index_left;
							// if indexright is less than indexleft...
							if (items[index_right]->isLess(items[index_left]))
							{
								swapIndex = index_right;
							}
							swap(item->index, swapIndex);
						}
						else return;
					}
					// Swap the loweset with the highest, or not.
					else if (items[item->index]->isLess(items[swapIndex]))
					{
						swap(swapIndex, item->index);
					}
					else return;
				}
				else
					return;
			}
		}

		//! Swaps two items at the specified ID's.
		void swap(int ID1, int ID2)
		{
			T* itemTemp = items[ID1];
			items[ID1] = items[ID2];
			items[ID2] = itemTemp;
			items[ID1]->index = ID1;
			items[ID2]->index = ID2;
		}

		// Checks if the heap contains the specified item.
		bool contains(T* item)
		{
			if (item->index == -1)
				return false;
			else
				return true;
		}
	};
}