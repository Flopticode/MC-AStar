#include "AStar.h"
#include "PathFindingBlockStateUtils.h"
#include <queue>
#include <iostream>
#include <chrono>
#include "BlockPosHashList.h"
#include "WorldRenderer.h"
#include "NodePrioQueue.h"
#include "NodeHeap.h"
#include <unordered_set>
#include "DebugDefs.h"

Path* makePath(Node* node);

uint32 getCost(PathFindingBlockState state)
{
	return
		PFBSUtils::getBreakDelay(state);
}

void expandNode(NodeHeap& nodeHeap, WorldRenderer* wr, uint32& idCntr, BlockPos start, BlockPos end,
	PathFindingWorld* world,
	NodePrioQueue& openlist, std::unordered_set<BlockPos>& closedlist, Node* currentNode)
{
	BlockPos curPos = currentNode->pos;
	
	// TODO feels inefficient to always test all the neighbors.
	// is there a more efficient way?
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

		if (closedlist.count(successorPos) > 0)
			continue;
		
		auto successor = nodeHeap.newNode(idCntr++,
			successorPos,
			world->getBlockState(successorPos),
			nullptr,
			(uint32)start.dist(successorPos));

		if (PFBSUtils::isInvalid(successor->state))
			continue;

		auto cost = getCost(successor->state);
		auto tentative_g = currentNode->gCost + cost;

		bool openlistContainsSuccessor = openlist.contains(successor);
		if (openlistContainsSuccessor && tentative_g >= successor->gCost)
			continue;

		successor->predecessor = currentNode;
		successor->gCost = tentative_g;
		uint32 hCost = successor->pos.manhdist(end);
		uint32 fCost = (uint32)(tentative_g + hCost);

		if (openlistContainsSuccessor)
			openlist.update(successor, fCost);
		else
			openlist.addNode(successor, fCost);

#ifdef DEBUG_RENDERING
		/* Just for debugging */
		auto stateBefore = world->getBlockState(successor->pos);
		auto debugData = PFBSUtils::getDebugData(stateBefore);
		debugData |= 0b10;
		auto newState = PFBSUtils::setDebugData(stateBefore, debugData);
		world->setBlockState(successor->pos, newState);
		int a = 0;
		/* ==== */
#endif
	}
}

Path* AStar::calculatePath(WorldRenderer* wr, PathFindingWorld* world, BlockPos start, BlockPos end)
{
	/* The higher, the faster, but your memory gets eaten */
	auto initialSize = end.dist(start) * 80;
	NodeHeap nodeHeap = NodeHeap(initialSize);
	auto openlist = NodePrioQueue(initialSize);
	auto closedlist = std::unordered_set<BlockPos>();

	uint32 idCntr = 0;

	openlist.addNode(nodeHeap.newNode(idCntr++, start, world->getBlockState(start), nullptr, 0), 0);

	Path* path = nullptr;

	do
	{
		auto curNode = openlist.pop();

#ifdef DEBUG_RENDERING
		/* Just for debugging */
		auto stateBefore = world->getBlockState(curNode->pos);
		auto debugData = PFBSUtils::getDebugData(stateBefore);
		debugData &= 0b01;
		auto newState = PFBSUtils::setDebugData(stateBefore, debugData);
		world->setBlockState(curNode->pos, newState);
		int a = 0;
		/* ==== */
#endif

		
		if (curNode->pos == end)
		{
			//std::cout << closedlist.size() << " nodes were evaluated, " << openlist.size() << " were open." << std::endl;
			path = makePath(curNode);
			break;
		}

		closedlist.insert(curNode->pos);

		expandNode(nodeHeap, wr, idCntr, start, end, world, openlist, closedlist, curNode);
#ifdef DEBUG_RENDERING
		/* Just for debugging */
		stateBefore = world->getBlockState(curNode->pos);
		debugData = PFBSUtils::getDebugData(stateBefore);
		debugData |= 0b1;
		newState = PFBSUtils::setDebugData(stateBefore, debugData);
		world->setBlockState(curNode->pos, newState);
		static uint8 i = 0;
		
		if(i % 1 == 0)
			wr->render();
		i++;
		std::cout << "CurNode: " << curNode->pos.toString() << std::endl;
		a = 0;
		/* ==== */
#endif

	} while (!openlist.empty());

	return path;
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