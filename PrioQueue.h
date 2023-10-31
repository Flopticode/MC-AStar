#pragma once

#include "MinecraftTypes.h"
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <chrono>
#include <iostream>
#include <map>
#include "HashMap.h"

#define BLOCK_QUANT 900000

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
		entryMap.addNode(node->pos, entry);
		std::push_heap(getHeapEntry(0), getHeapEntry(size()), &compareEntries);
	}
	void update(Node* node, uint32 newPrio)
	{
		auto index = indexOf(node);
		getEntry(index)->priority = newPrio;
		std::push_heap(getHeapEntry(0), getHeapEntry(index + 1), &compareEntries);
	}
	int indexOf(PrioQueueEntry* entry)
	{
		int index = 1;
		while (true)
		{
			if (getEntry(index) == entry)
			{
				return index;
			}
			if (getEntry(index) > entry)
			{
				index = index * 2 + 1;
			}
			else
			{
				index = index * 2;
			}

			if (index >= size())
				return -1;
		}
	}
	uint32 indexOf(Node* node)
	{
		PrioQueueEntry* entry = entryMap.get(node->pos);
		auto index = indexOf(entry);
		if (index == -1)
			throw std::invalid_argument("This node is not in this heap");
		return index;
	}
	Node* pop()
	{
		if (size() == 0)
			return nullptr;

		auto frontNode = getHeapEntry(0)->node;
		entryMap.remove(frontNode->pos);
		std::pop_heap(getHeapEntry(0), getHeapEntry(size()), &compareEntries);
		numentries--;

		return frontNode;
	}
	bool contains(Node* node)
	{
		return entryMap.get(node->pos) != nullptr;
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
	{
		entryMap.clearAndSetNumBuckets(initialSize);

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
	PrioQueueEntry* getHeapEntry(size_t index)
	{
		return &entryHeap[0][index];
	}

private:
	HashMap entryMap = HashMap();

	size_t numnodes = 0;
	size_t maxnumNodes;
	std::vector<Node*> nodes;

	size_t numentries = 0;
	size_t maxnumEntries;
	std::vector<PrioQueueEntry*> entries;

	PrioQueueEntry (*entryHeap)[BLOCK_QUANT] = new PrioQueueEntry[1][BLOCK_QUANT];
};
