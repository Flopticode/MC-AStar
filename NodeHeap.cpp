#include "NodeHeap.h"

NodeHeap::NodeHeap(size_t initialSize)
{
	nodes.reserve(initialSize);
}

Node* NodeHeap::newNode(uint32 id, BlockPos pos,
	PathFindingBlockState state, Node* predecessor, uint32 gCost)
{
	if (nodes.size() == nodes.capacity())
		nodes.reserve(nodes.size() * 2);

	nodes.push_back(Node(id, pos, state, predecessor, gCost));
	return get(nodes.size() - 1);
}
inline Node* NodeHeap::get(size_t index)
{
	return &nodes[index];
}