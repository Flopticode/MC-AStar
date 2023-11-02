#pragma once

#include <vector>
#include <list>
#include <unordered_map>
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

	bool operator==(const _Node& other)
	{
		return other.pos == pos;
	}

} Node;

class HashMap
{
public:
	HashMap(size_t numBuckets)
	{
		clearAndSetNumBuckets(numBuckets);
	}
	void addNode(Node* node, uint32 priority)
	{
		priorities[node->pos] = priority;
		buckets[priority % buckets.size()].push_back(node);
		buckets[priority % buckets.size()].sort([this](Node* n1, Node* n2) {
			return priorities[n1->pos] < priorities[n2->pos];
		}); // TODO this is inefficient. change this.
	}
	void update(Node* node, uint32 newPriority)
	{
		uint32 oldPrio = priorities[node->pos];
		if (oldPrio == newPriority)
			return;

		buckets[oldPrio % buckets.size()].remove(node);
		priorities[node->pos] = newPriority;
		buckets[newPriority % buckets.size()].push_back(node);
		buckets[newPriority % buckets.size()].sort([this](Node* n1, Node* n2) {
			return priorities[n1->pos] < priorities[n2->pos];
		}); // TODO this is inefficient. change this.
	}
	bool contains(Node* node)
	{
		return priorities.count(node->pos) > 0;
	}
	Node* pop()
	{
		for (auto& bucket : buckets)
		{
			if (!bucket.empty())
			{
				auto ret = bucket.front();
				bucket.pop_front();
				priorities.erase(ret->pos);
				return ret;
			}
		}
	}
	void clearAndSetNumBuckets(size_t num)
	{
		buckets.clear();
		buckets.reserve(num);
		buckets.resize(num);
	}
	void clear()
	{
		auto numBuckets = buckets.size();
		for (size_t i = 0; i < numBuckets; i++)
		{
			buckets[numBuckets].clear();
		}
		priorities.clear();
	}
	size_t size()
	{
		return priorities.size();
	}
	bool empty()
	{
		return size() == 0;
	}

private:
	std::vector<std::list<Node*>> buckets;
	std::unordered_map<BlockPos, uint32> priorities;
};