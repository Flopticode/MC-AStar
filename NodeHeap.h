#pragma once

#include <vector>
#include "NodePrioQueue.h"
#include "types.h"
#include <malloc.h>

class NodeHeap
{
public:
	NodeHeap(size_t initialSize);
	
	uint32 newNode(BlockPos pos, PathFindingBlockState state,
		uint32 predecessor, uint32 gCost);
	Node* get(size_t index);
private:
	std::vector<Node> nodes;
	uint32 idCntr = 0;
};