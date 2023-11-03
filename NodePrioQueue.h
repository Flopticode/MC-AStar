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

	_Node();
	_Node(uint32 id, BlockPos pos, PathFindingBlockState state,
		_Node* predecessor, uint32 gCost);

	bool operator==(const _Node& other);

} Node;

class NodePrioQueue
{
public:
	NodePrioQueue(size_t numBuckets);
	void addNode(Node* node, uint32 priority);
	void update(Node* node, uint32 newPriority);
	bool contains(Node* node);
	Node* pop();
	void clearAndSetNumBuckets(size_t num);
	void clear();
	size_t size();
	bool empty();

private:
	std::vector<std::list<Node*>> buckets;

	/* TODO implement a more efficient unordered map due to the CPU spikes to ~40us
	without hash collisions */
	std::unordered_map<BlockPos, uint32> priorities;
};