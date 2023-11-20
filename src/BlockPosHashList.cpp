#include "BlockPosHashList.h"
#include <stdexcept>

void BlockPosHashList::addNode(BlockPos pos)
{
	buckets[pos.hash() % buckets.size()].push_back(pos);
	numElements++;
}
bool BlockPosHashList::contains(BlockPos pos)
{
	auto& bucket = buckets[pos.hash() % buckets.size()];
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
void BlockPosHashList::remove(BlockPos pos)
{
	auto& bucket = buckets[pos.hash() % buckets.size()];
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
void BlockPosHashList::clearAndSetNumBuckets(size_t num)
{
	buckets.clear();
	buckets.reserve(num);
	for (size_t i = 0; i < num; i++)
	{
		buckets.push_back(std::list<BlockPos>());
	}
}
void BlockPosHashList::clear()
{
	auto numBuckets = buckets.size();
	for (size_t i = 0; i < numBuckets; i++)
	{
		buckets[numBuckets].clear();
	}
}
size_t BlockPosHashList::size()
{
	return numElements;
}
