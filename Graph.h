#pragma once

#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "MinecraftTypes.h"

#include "Path.h"
#include "NodePrioQueue.h"

class Path;
class Graph
{
public:
	Graph();

	uint32 newNode(BlockPos pos, uint16 abstractionLevel, uint32 parent, std::unordered_set<uint32> subNodes,
		Node::Edges edges, PathFindingBlockState state);
	bool contains(uint32 node);
	Path* findPath(Node* startNode, Node* endNode, uint32 k);
	Node& getNode(uint32 id);
	Path* doAStarSearch(std::unordered_set<Node*>* nodeRestriction, Node* startNode, Node* endNode);
	Node* getNodeAt(BlockPos pos);

	void addChunk(ChunkPos pos, PathFindingBlockState* data);
	void setBlockState(BlockPos pos, PathFindingBlockState state);

private:
	std::vector<Node> nodeHeap;
	std::vector<uint32> rootNodes;

	Path* makePath(Node* endNode);
	void expandNode(Node* start, Node* end, NodePrioQueue& openlist, std::unordered_set<Node*>& closedlist, Node* currentNode);
	uint32 getAbstractionHierarchy(Node* node1, Node* node2);
	Path* refinePath(Path* path, Node* startNode, Node* end);
};
