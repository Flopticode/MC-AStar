#pragma once

#include "MinecraftTypes.h"
#include <vector>
#include <list>
#include "HashMap.h"
#include <stdexcept>

class HashList
{
public:
	void addNode(BlockPos pos)
	{
		buckets[pos.hash() % buckets.size()].push_back(pos);
		numElements++;
	}
	bool contains(BlockPos pos)
	{
		auto bucket = buckets[pos.hash() % buckets.size()];
		auto endIter = bucket.end();
		auto iter = bucket.begin();
		while (iter != endIter)
		{
			BlockPos curEntry = *iter;
			if (curEntry == pos)
				return true;
			iter++;
		}
		return false;
	}
	void remove(BlockPos pos)
	{
		auto bucket = buckets[pos.hash() % buckets.size()];
		auto endIter = bucket.end();
		auto iter = bucket.begin();
		while (iter != endIter)
		{
			BlockPos curEntry = *iter;
			if (curEntry == pos)
			{
				bucket.erase(iter);
				numElements--;
				return;
			}
			iter++;
		}
		throw std::invalid_argument("No such position.");
	}
	void clearAndSetNumBuckets(size_t num)
	{
		buckets.clear();
		buckets.reserve(num);
		for (size_t i = 0; i < num; i++)
		{
			buckets.push_back(std::list<BlockPos>());
		}
	}
	void clear()
	{
		auto numBuckets = buckets.size();
		for (size_t i = 0; i < numBuckets; i++)
		{
			buckets[numBuckets].clear();
		}
	}
	size_t size()
	{
		return numElements;
	}

private:
	std::vector<std::list<BlockPos>> buckets;
	size_t numElements = 0;
};