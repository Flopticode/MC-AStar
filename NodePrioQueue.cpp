#include "NodePrioQueue.h"

bool _Node::operator==(const _Node& other) const
{
	return other.pos == pos;
}

_Node::_Node()
	:id(0), predecessor(0xFFFFFFFF), pos(), state(0), gCost(0)
{

}

_Node::_Node(uint32 id, BlockPos pos, PathFindingBlockState state, uint32 predecessor, uint32 gCost)
	:id(id), pos(pos), state(state), predecessor(predecessor), gCost(gCost)
{

}

NodePrioQueue::NodePrioQueue(size_t numBuckets)
	:priorities(BlockPosPrioMap(numBuckets))
{
	if (numBuckets == 0)
		numBuckets = 1;
	clearAndSetNumBuckets(numBuckets);
}

void NodePrioQueue::addNode(Node node, uint32 priority)
{
	priorities.put(node.pos, priority);
	insertIntoBucket(buckets[priority % buckets.size()], node, priority);
}

void NodePrioQueue::update(Node node, uint32 newPriority)
{
	uint32 oldPrio = priorities.get(node.pos);
	if (oldPrio == newPriority)
		return;

	auto& bucket = buckets[oldPrio % buckets.size()];
	bucket.remove(node);
	priorities.put(node.pos, newPriority);
	insertIntoBucket(bucket, node, newPriority);
}

bool NodePrioQueue::contains(Node node)
{
	return priorities.contains(node.pos);
}

Node NodePrioQueue::pop()
{
	for (auto& bucket : buckets)
	{
		if (!bucket.empty())
		{
			auto ret = bucket.front();
			bucket.pop_front();
			priorities.erase(ret.pos);
			return ret;
		}
	}
	return Node(0xFFFFFFFF, BlockPos(), 0, 0xFFFFFFFF, 0);
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

void NodePrioQueue::insertIntoBucket(std::list<Node>& bucket, Node node, uint32 prio)
{
	auto iter = bucket.begin();
	while (iter != bucket.end())
	{
		if (priorities.get((*iter).pos) > prio)
		{
			bucket.insert(iter, node);
			break;
		}
		iter++;
	}
	bucket.push_back(node);
}
