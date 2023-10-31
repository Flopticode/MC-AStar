#pragma once

#include <vector>
#include <list>
#include "MinecraftTypes.h"

typedef struct _Node
{
	uint32 id = 0;
	_Node* predecessor;
	BlockPos pos;
	PathFindingBlockState state;
	uint32 gCost;

	_Node()
		:id(0), predecessor(nullptr), pos(), state(0), gCost(0)
	{

	}
	_Node(uint32 id, BlockPos pos, PathFindingBlockState state, _Node* predecessor, uint32 gCost)
		:id(id), pos(pos), state(state), predecessor(predecessor), gCost(gCost)
	{

	}

	bool operator==(_Node other)
	{
		return other.pos == pos;
	}

} Node;

typedef struct _PrioQueueEntry
{
	Node* node;
	uint32 priority;

	_PrioQueueEntry(Node* node, uint32 priority)
		:node(node), priority(priority)
	{

	}
	_PrioQueueEntry()
		:node(nullptr), priority(0)
	{

	}

	bool operator<(_PrioQueueEntry other)
	{
		if (other.priority < priority)
			return true;
		else if (other.priority > priority)
			return false;
		return other.node->id < node->id;
	}
	bool operator>(_PrioQueueEntry other)
	{
		if (other.priority > priority)
			return true;
		else if (other.priority < priority)
			return false;
		return other.node->id > node->id;
	}
	bool operator==(_PrioQueueEntry other)
	{
		return node == other.node && priority == other.priority;
	}
} PrioQueueEntry;

class HashMap
{
public:
	void addNode(BlockPos pos, PrioQueueEntry* entry)
	{
		buckets[pos.hash() % buckets.size()].push_back(entry);
		numElements++;
	}
	PrioQueueEntry* get(BlockPos pos)
	{
		auto bucket = buckets[pos.hash() % buckets.size()];
		auto endIter = bucket.end();
		auto iter = bucket.begin();
		while (iter != endIter)
		{
			PrioQueueEntry* curEntry = *iter;
			if (curEntry->node->pos == pos)
				return curEntry;
			iter++;
		}
		return nullptr;
	}
	void remove(BlockPos pos)
	{
		auto bucket = buckets[pos.hash() % buckets.size()];
		auto endIter = bucket.end();
		auto iter = bucket.begin();
		while (iter != endIter)
		{
			PrioQueueEntry* curEntry = *iter;
			if (curEntry->node->pos == pos)
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
			buckets.push_back(std::list<PrioQueueEntry*>());
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
	std::vector<std::list<PrioQueueEntry*>> buckets;
	size_t numElements = 0;
};