#pragma once

#include <vector>
#include "NodePrioQueue.h"
#include "types.h"
#include <malloc.h>

class NodeHeap
{
public:
	NodeHeap(size_t initialSize);
	
	Node* newNode(uint32 id, BlockPos pos,
		PathFindingBlockState state, Node* predecessor, uint32 gCost);
	inline Node* get(size_t index);
private:
	std::vector<Node> nodes;
};