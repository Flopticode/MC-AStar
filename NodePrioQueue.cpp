#include "NodePrioQueue.h"

NodePrioQueue::NodePrioQueue(size_t numBuckets)
	:priorities(BlockPosPrioMap(numBuckets))
{
	if (numBuckets == 0)
		numBuckets = 1;
	clearAndSetNumBuckets(numBuckets);
}

void NodePrioQueue::addNode(Node* node, uint32 priority)
{
	priorities.put(node->getPos(), priority);
	insertIntoBucket(buckets[priority % buckets.size()], node, priority);
}

void NodePrioQueue::update(Node* node, uint32 newPriority)
{
	uint32 oldPrio = priorities.get(node->getPos());
	if (oldPrio == newPriority)
		return;

	auto& bucket = buckets[oldPrio % buckets.size()];
	bucket.remove(node);
	priorities.put(node->getPos(), newPriority);
	insertIntoBucket(bucket, node, newPriority);
}

bool NodePrioQueue::contains(Node* node)
{
	return priorities.contains(node->getPos());
}

Node* NodePrioQueue::pop()
{
	for (auto& bucket : buckets)
	{
		if (!bucket.empty())
		{
			auto ret = bucket.front();
			bucket.pop_front();
			priorities.erase(ret->getPos());
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

void NodePrioQueue::insertIntoBucket(std::list<Node*>& bucket, Node* node, uint32 prio)
{
	auto iter = bucket.begin();
	while (iter != bucket.end())
	{
		if (priorities.get((*iter)->getPos()) > prio)
		{
			bucket.insert(iter, node);
			break;
		}
		iter++;
	}
	bucket.push_back(node);
}
