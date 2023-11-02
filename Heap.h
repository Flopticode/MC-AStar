#pragma once

#include <vector>
#include "HashMap.h"
#include <algorithm>

class Heap
{
public:
	Heap(size_t size)
	{
		entries.reserve(size);
	}
	void push(int* entry)
	{
		uint32 upperBound = entries.size();
		uint32 lowerBound = startIndex;
		uint32 middle;

		while (true)
		{
			middle = (upperBound - lowerBound) / 2;
			if (entry > entries[middle])
				lowerBound = middle + 1;
			else
				upperBound = middle;

			if (upperBound == lowerBound)
			{

			}
		}
	}
	int* pop()
	{
		auto ret = entries[startIndex];
		startIndex++;
		return ret;
	}
	size_t size()
	{
		return entries.size();
	}

private:
	std::vector<int*> entries;
	size_t startIndex = 0;
};
