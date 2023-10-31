#include "AStar.h"
#include "PathFindingBlockStateUtils.h"
#include "PrioQueue.h"
#include <queue>
#include <iostream>
#include <chrono>
#include "HashList.h"

Path* makePath(Node* node);

uint32 getCost(PathFindingBlockState state)
{
	return
		PFBSUtils::getBreakDelay(state);
}

void expandNode(uint32& idCntr, BlockPos start, BlockPos end, PathFindingWorld* world,
	PriorityQueue& openlist,
	HashList& closedlist, Node* currentNode)
{
	BlockPos curPos = currentNode->pos;
	BlockPos successorPositions[] = {
		BlockPos(curPos.x,	curPos.y + 1, curPos.z),
		BlockPos(curPos.x,	curPos.y - 1, curPos.z),
		BlockPos(curPos.x,	curPos.y, curPos.z + 1),
		BlockPos(curPos.x,	curPos.y, curPos.z - 1),
		BlockPos(curPos.x + 1,	curPos.y, curPos.z),
		BlockPos(curPos.x - 1,	curPos.y, curPos.z)
	};

	for (uint8 i = 0; i < 6; i++)
	{
		

		BlockPos successorPos = successorPositions[i];

		if (closedlist.contains(successorPos))
			continue;
		
		auto successor = openlist.newNode(idCntr++,
			successorPos,
			world->getBlockState(successorPos),
			nullptr,
			(uint32)start.dist(successorPos));

		if (PFBSUtils::isInvalid(successor->state))
			continue;

		auto tentative_g = currentNode->gCost + getCost(successor->state);

		bool openlistContainsSuccessor = openlist.contains(successor);
		if (openlistContainsSuccessor && tentative_g >= successor->gCost)
			continue;

		successor->predecessor = currentNode;
		successor->gCost = tentative_g;

		uint32 fCost = (uint32)(tentative_g + successor->pos.dist(end));

		if (openlistContainsSuccessor)
			openlist.update(successor, fCost);
		else
			openlist.add(successor, fCost);

		
	}
}

Path* AStar::calculatePath(PathFindingWorld* world, BlockPos start, BlockPos end)
{
	auto initialSize = end.dist(start) * 80;

	auto openlist = PriorityQueue(initialSize);
	auto closedlist = HashList();
	closedlist.clearAndSetNumBuckets(initialSize);

	uint32 idCntr = 0;

	openlist.add(openlist.newNode(idCntr++, start, world->getBlockState(start), nullptr, 0), 0);

	do
	{
		
		auto curNode = openlist.pop();
		
		if (curNode->pos == end)
		{
			std::cout << closedlist.size() << " nodes were evaluated, " << openlist.size() << " were open." << std::endl;
			return makePath(curNode);
		}

		closedlist.addNode(curNode->pos);

		expandNode(idCntr, start, end, world, openlist, closedlist, curNode);

	} while (!openlist.empty());
	
	return nullptr;
}

Path* makePath(Node* node)
{
	auto path = std::list<BlockPos>();
	while (node->predecessor != nullptr)
	{
		path.push_back(node->pos);
		node = node->predecessor;
	}
	path.reverse();
	return new Path(path);
}