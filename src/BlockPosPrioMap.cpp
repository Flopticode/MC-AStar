#include "BlockPosPrioMap.h"
#include <stdexcept>

BlockPosPrioMap::BlockPosPrioMap(size_t numBuckets)
{
	buckets.reserve(numBuckets);
	buckets.resize(numBuckets);
	this->numElements = 0;
	this->numBuckets = numBuckets;
}

bool BlockPosPrioMap::contains(BlockPos pos)
{
	return otto.count(pos) > 0;
	for (auto& entry : buckets[pos.hash() % buckets.size()])
	{
		if (pos == entry.pos)
			return true;
	}
	return false;
}

uint32 BlockPosPrioMap::get(BlockPos pos)
{
	return otto[pos];
	for (auto& entry : buckets[pos.hash() % buckets.size()])
	{
		if (pos == entry.pos)
			return entry.prio;
	}
	throw std::invalid_argument("Position " + pos.toString() + " is not contained in this map.");
}

void BlockPosPrioMap::put(BlockPos pos, uint32 prio)
{
	otto[pos] = prio;
	return;
	BlockPosPrioMapEntry e = { pos, prio };
	buckets[pos.hash() % buckets.size()].push_back(e);
	numElements++;
}

void BlockPosPrioMap::erase(BlockPos pos)
{
	otto.erase(pos);
	return;
	auto& bucket = buckets[pos.hash() % buckets.size()];
	auto iter = bucket.begin();
	auto endIter = bucket.end();
	while (iter != endIter)
	{
		if ((*iter).pos == pos)
		{
			bucket.erase(++iter);
			numElements--;
			return;
		}
		iter++;
	}
	throw std::invalid_argument("The BlockPos " + pos.toString() + " is not contained in this map.");
}

void BlockPosPrioMap::clear()
{
	otto.clear();
	return;
	numElements = 0;
	buckets.clear();
	buckets.resize(numBuckets);
}

size_t BlockPosPrioMap::size()
{
	return otto.size();
	return numElements;
}