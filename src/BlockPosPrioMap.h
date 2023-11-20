#pragma once

#include "MinecraftTypes.h"
#include <list>
#include <vector>
#include <unordered_map>

struct BlockPosPrioMapEntry
{
	BlockPos pos;
	uint32 prio;
};

class BlockPosPrioMap
{
public:
	BlockPosPrioMap(size_t numBuckets);
	bool contains(BlockPos pos);
	uint32 get(BlockPos pos);
	void put(BlockPos pos, uint32 prio);
	void erase(BlockPos pos);
	void clear();
	size_t size();

private:
	size_t numElements;
	size_t numBuckets;
	std::vector<std::list<BlockPosPrioMapEntry>> buckets;

	std::unordered_map<BlockPos, uint32> otto = std::unordered_map<BlockPos, uint32>();
};