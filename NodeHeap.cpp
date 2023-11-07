#include "NodeHeap.h"

NodeHeap::NodeHeap(size_t initialSize)
{
	nodes.reserve(initialSize);
}

uint32 NodeHeap::newNode(BlockPos pos,
	PathFindingBlockState state, uint32 predecessorID, uint32 gCost)
{
	if (nodes.size() == nodes.capacity())
		nodes.reserve(nodes.size() * 2);

	nodes.push_back(Node(idCntr++, pos, state, predecessorID, gCost));
	return nodes.size() - 1;
}
Node* NodeHeap::get(size_t index)
{
	return &nodes[index];
}