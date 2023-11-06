#include "NodePrioQueue.h"

bool _Node::operator==(const _Node& other)
{
	return other.pos == pos;
}

_Node::_Node()
	:id(0), predecessor(nullptr), pos(), state(0), gCost(0)
{

}

_Node::_Node(uint32 id, BlockPos pos, PathFindingBlockState state, _Node* predecessor, uint32 gCost)
	:id(id), pos(pos), state(state), predecessor(predecessor), gCost(gCost)
{

}

NodePrioQueue::NodePrioQueue(size_t numBuckets)
{
	if (numBuckets == 0)
		numBuckets = 1;

	clearAndSetNumBuckets(numBuckets);
}

void NodePrioQueue::addNode(Node* node, uint32 priority)
{
	priorities[node->pos] = priority;
	buckets[priority % buckets.size()].push_back(node);
	buckets[priority % buckets.size()].sort([this](Node* n1, Node* n2) {
		return priorities[n1->pos] < priorities[n2->pos];
		}); // TODO this is inefficient. change this.
}

void NodePrioQueue::update(Node* node, uint32 newPriority)
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

bool NodePrioQueue::contains(Node* node)
{
	return priorities.count(node->pos) > 0;
}

Node* NodePrioQueue::pop()
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
	return nullptr;
}
void NodePrioQueue::clearAndSetNumBuckets(size_t num)
{
	if (num == 0)
		num = 1;

	buckets.clear();
	buckets.reserve(num);
	buckets.resize(num);
}
void NodePrioQueue::clear()
{
	auto numBuckets = buckets.size();
	for (size_t i = 0; i < numBuckets; i++)
		buckets[numBuckets].clear();
	priorities.clear();
}
size_t NodePrioQueue::size()
{
	return priorities.size();
}
bool NodePrioQueue::empty()
{
	return size() == 0;
}
