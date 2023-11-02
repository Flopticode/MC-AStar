#pragma once

#include "MinecraftTypes.h"
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <chrono>
#include <iostream>
#include <map>
#include "HashMap.h"
#include "Heap.h"

#define BLOCK_QUANT 900000
/*
class PriorityQueue
{
private:
	static bool compareEntries(PrioQueueEntry e1, PrioQueueEntry e2)
	{
		return e1 < e2;
	}

public:
	void add(Node* node, uint32 prio)
	{
		PrioQueueEntry* entry = newEntry(node, prio);
		entryHeap[0][size()-1] = *entry;
		entryMap[node->pos] = entry; //entryMap.addNode(node->pos, entry);
		std::push_heap(getHeapEntry(0), getHeapEntry(size()), &compareEntries);
	}
	void update(Node* node, uint32 newPrio)
	{
		auto index = indexOf(node);
		getEntry(index)->priority = newPrio;
		std::push_heap(getHeapEntry(0), getHeapEntry(index + 1), &compareEntries);
	}
	Node* pop()
	{
		if (size() == 0)
			return nullptr;

		auto frontNode = heap.pop()->node;
		//entryMap.remove(frontNode->pos);
		entryMap.erase(frontNode->pos);
		numentries--;

		return frontNode;
	}
	bool contains(Node* node)
	{
		return entryMap.count(node->pos) > 0;//entryMap.get(node->pos) != nullptr;
	}
	bool empty()
	{
		return size() == 0;
	}
	size_t size()
	{
		return numentries;
	}
	void clear()
	{
		numentries = 0;
		entryMap.clear();
	}

	PriorityQueue(size_t initialSize)
		:heap(Heap(maxnumEntries))
	{
		//entryMap.clearAndSetNumBuckets(initialSize);

		nodes = std::vector<Node*>();
		nodes.push_back(new Node[BLOCK_QUANT]());
		maxnumNodes = BLOCK_QUANT;

		entries = std::vector<PrioQueueEntry*>();
		entries.push_back(new PrioQueueEntry[BLOCK_QUANT]());
		maxnumEntries = BLOCK_QUANT;
	}
	~PriorityQueue()
	{
		for (auto& n : nodes)
			delete n;
		for (auto& e : entries)
			delete e;
		delete entryHeap;
	}

	Node* newNode(uint32 id, BlockPos pos, PathFindingBlockState state, Node* predecessor, uint32 gCost)
	{
		if (numnodes >= maxnumNodes - 100)
		{
			nodes.push_back(new Node[BLOCK_QUANT]());
			maxnumNodes += BLOCK_QUANT;
		}

		Node* newNodePtr = getNode(numnodes++);

		*newNodePtr = Node(id, pos, state, predecessor, gCost);
		return newNodePtr;
	}
	Node* getNode(size_t index)
	{
		return &nodes[index / BLOCK_QUANT][index % BLOCK_QUANT];
	}
	
	PrioQueueEntry* newEntry(Node* node, uint32 prio)
	{
		if (numentries >= maxnumEntries - 100)
		{
			entries.push_back(new PrioQueueEntry[BLOCK_QUANT]());
			maxnumEntries += BLOCK_QUANT;
		}

		PrioQueueEntry* newEntryPtr = getEntry(numentries++);
		*newEntryPtr = PrioQueueEntry(node, prio);
		return newEntryPtr;
	}
	PrioQueueEntry* getEntry(size_t index)
	{
		return &entries[index / BLOCK_QUANT][index % BLOCK_QUANT];
	}

private:
	//HashMap entryMap = HashMap();
	std::map<BlockPos, PrioQueueEntry*> entryMap;
	Heap heap;

	size_t numnodes = 0;
	size_t maxnumNodes;
	std::vector<Node*> nodes;

	size_t numentries = 0;
	size_t maxnumEntries;
	std::vector<PrioQueueEntry*> entries;

	PrioQueueEntry (*entryHeap)[BLOCK_QUANT] = new PrioQueueEntry[1][BLOCK_QUANT];
};
*/