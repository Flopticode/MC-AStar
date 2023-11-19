#pragma once

#include "types.h"
#include <unordered_map>
#include <unordered_set>
#include "MinecraftTypes.h"

class Graph;

/* A node class specifically designed for Graphs used for Path Refinement A* */
class Node
{
public:
	static constexpr uint32 NODE_INVALID = 0xFFFFFFFF;

	typedef std::pair<uint32, uint32> Edge;
	typedef std::unordered_map<uint32, uint32> Edges;

	Node(BlockPos pos, uint16 abstractionLevel, uint32 parent, std::unordered_set<uint32> subNodes, Edges neighbors, PathFindingBlockState state);
	Node(BlockPos pos, uint16 abstractionLevel, std::unordered_set<uint32> subNodes, Edges neighbors, PathFindingBlockState state);
	bool contains(Graph* nodeSpace, uint32 subNode);
	std::unordered_map<uint32, uint32>& getEdges();
	std::unordered_set<uint32> getSubLevel(uint16 depth, Graph* nodeSpcae);
	uint32 getParent(Graph* graph, uint16 level);
	uint32 getParent();
	uint16 getLevel();
	BlockPos getPos();
	Node* getNodeAt(BlockPos pos, Graph* nodeSpace);

	uint32 gCost;
	Node* predecessor;
	PathFindingBlockState state;

private:
	BlockPos pos;
	uint16 abstractionLevel;
	uint32 parent;
	std::unordered_set<uint32> subNodes;
	Edges neighbors;
};