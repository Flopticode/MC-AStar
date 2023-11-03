#pragma once

#include "MinecraftTypes.h"
#include <vector>
#include <list>

class BlockPosHashList
{
public:
	void addNode(BlockPos pos);
	bool contains(BlockPos pos);
	void remove(BlockPos pos);
	void clearAndSetNumBuckets(size_t num);
	void clear();
	size_t size();

private:
	std::vector<std::list<BlockPos>> buckets;
	size_t numElements = 0;
};