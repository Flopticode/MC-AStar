#pragma once

#include <vector>
#include <list>
#include <unordered_map>
#include "MinecraftTypes.h"
#include "BlockPosPrioMap.h"
#include "Node.h"

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
	BlockPosPrioMap priorities;

	void insertIntoBucket(std::list<Node*>& bucket, Node* node, uint32 prio);
};