#include "Graph.h"
#include "NodePrioQueue.h"
#include "PathFindingBlockStateUtils.h"

Node::Node(BlockPos pos, uint16 abstractionLevel, uint32 parent, std::unordered_set<uint32> subNodes, Edges edges, PathFindingBlockState state)
	:abstractionLevel(abstractionLevel),
	parent(parent),
	subNodes(subNodes),
	neighbors(edges),
	gCost(0),
	predecessor(nullptr),
	state(state),
	pos(pos)
{
	
}
Node::Node(BlockPos pos, uint16 abstractionLevel, std::unordered_set<uint32> subNodes, Edges edges, PathFindingBlockState state)
	:abstractionLevel(abstractionLevel),
	parent(Node::NODE_INVALID),
	subNodes(subNodes),
	neighbors(edges),
	gCost(0),
	predecessor(nullptr),
	state(state),
	pos(pos)
{

}
Node::Edges& Node::getEdges()
{
	return neighbors;
}
uint32 Node::getParent(Graph* graph, uint16 level)
{
	Node* curNode = this;
	for (uint16 l = 0; l < level - 1; l++)
	{
		uint32 parentID = curNode->parent;
		if (parentID == Node::NODE_INVALID)
			return Node::NODE_INVALID;

		curNode = &graph->getNode(parentID);
	}
	return curNode->parent;
}
uint32 Node::getParent()
{
	return parent;
}
std::unordered_set<uint32> Node::getSubLevel(uint16 depth, Graph* nodeSpace)
{
	if (depth == 1)
		return subNodes;

	auto nextLevel = std::unordered_set<uint32>();

	for (uint32 subNode : subNodes)
	{
		auto subsubLevel = nodeSpace->getNode(subNode).getSubLevel(depth - 1, nodeSpace);
		for (uint32 node : subsubLevel)
		{
			nextLevel.insert(node);
		}
	}
	return nextLevel;
}
uint16 Node::getLevel()
{
	return abstractionLevel;
}
BlockPos Node::getPos()
{
	return pos;
}

Node* Node::getNodeAt(BlockPos pos, Graph* nodeSpace)
{
	if (this->pos == pos)
		return this;
	for (auto& edge : this->getEdges())
	{
		Node& node = nodeSpace->getNode(edge.second);
		if (node.getPos() == pos)
			return &node;
	}
	return nullptr;
}

bool Node::contains(Graph* nodeSpace, uint32 pNode)
{
	for (uint32 subNode : subNodes)
	{
		if (subNode == pNode)
			return true;
		if (nodeSpace->getNode(subNode).contains(nodeSpace, pNode))
			return true;
	}
	return false;
}


Graph::Graph()
{

}

uint32 Graph::newNode(BlockPos pos, uint16 abstractionLevel, uint32 parent, std::unordered_set<uint32> subNodes,
	Node::Edges edges, PathFindingBlockState state)
{
	uint32 newIndex = (uint32)nodeHeap.size();
	Node newNode = Node(pos, abstractionLevel, parent, subNodes, edges, state);
	nodeHeap.push_back(newNode);
	return newIndex;
}

bool Graph::contains(uint32 node)
{
	for (uint32 rootNode : rootNodes)
	{
		if (nodeHeap[rootNode].contains(this, node))
			return true;
	}
	return false;
}

uint32 Graph::getAbstractionHierarchy(Node* node1, Node* node2)
{
	if (node1 == nullptr || node2 == nullptr)
		return Node::NODE_INVALID;
	if (node1 == node2)
		return 0xFFFFFFFE;

	uint32 id1 = node1->getParent();
	uint32 id2 = node2->getParent();

	/* Start and end node are always on the same abstraction level,
	so tracing id1 and id2 up the abstraction hierarchy should either
	reveal a common abstract node or result in NODE_INVALID, meaning
	there is no abstraction hierarchy containing both nodes.
	It is sufficient to check for only id1 != NODE::NODE_INVALID
	due to id1 and id2 being equally deep in the hierarchy. */
	while (id1 != id2 && id1 != Node::NODE_INVALID)
	{
		id1 = getNode(id1).getParent();
		id2 = getNode(id2).getParent();
	}
	return id1;
}

Path* Graph::makePath(Node* node)
{
	auto nodes = std::list<Node*>();
	while (node != nullptr)
	{
		nodes.push_back(node);
		node = node->predecessor;
	}
	nodes.reverse();
	return new Path(nodes);
}

void Graph::expandNode(Node* start, Node* end, NodePrioQueue& openlist, std::unordered_set<Node*>& closedlist, Node* currentNode)
{
	for (auto& edge : currentNode->getEdges())
	{
		Node& successor = getNode(edge.second);
		BlockPos successorPos = successor.getPos();

		if (closedlist.count(&successor) > 0)
			continue;

		uint32 cost = edge.first;
		auto tentative_g = currentNode->gCost + cost;

		bool openlistContainsSuccessor = openlist.contains(&successor);
		if (openlistContainsSuccessor && tentative_g >= successor.gCost)
			continue;

		successor.predecessor = currentNode;
		successor.gCost = tentative_g;
		uint32 hCost = successorPos.manhdist(end->getPos());
		uint32 fCost = (uint32)(tentative_g + hCost);

		if (openlistContainsSuccessor)
			openlist.update(&successor, fCost);
		else
			openlist.addNode(&successor, fCost);
	}
}

Path* Graph::doAStarSearch(std::unordered_set<Node*>* nodeRestriction, Node* start, Node* end)
{
	BlockPos endPos = end->getPos();

	/* The higher, the faster, but your memory gets eaten */
	auto initialSize = start->getPos().dist(endPos) * 100;
	auto openlist = NodePrioQueue(initialSize);
	auto closedlist = std::unordered_set<Node*>();

	uint32 idCntr = 0;

	start->gCost = 0;
	start->predecessor = nullptr;
	openlist.addNode(start, 0);

	Path* path = nullptr;
	do
	{
		auto curNode = openlist.pop();

		if (curNode->getPos() == endPos)
		{
			path = makePath(curNode);
			break;
		}

		closedlist.insert(curNode);
		expandNode(start, end, openlist, closedlist, curNode);
	} while (!openlist.empty());

	return path;
}

Path* Graph::refinePath(Path* path, Node* startNode, Node* end)
{
	auto set = new std::unordered_set<Node*>();
	for (Node* node : path->get())
		set->insert(node);

	return doAStarSearch(set, startNode, end);
}

Path* Graph::findPath(Node* startNode, Node* endNode, uint32 k)
{
	uint32 abstrHierarchy = getAbstractionHierarchy(startNode, endNode);

	/* Obviously, if no abstract node contains both the start and end node,
	there will be no path between the two */
	if (abstrHierarchy == Node::NODE_INVALID)
	{
		return nullptr;
	}

	Node hierarchyRoot = getNode(abstrHierarchy);
	uint32 startLevel = hierarchyRoot.getLevel();
	Path* path = new Path(std::list<Node*>());

	for (int32 level = startLevel; level > 0; level--)
	{
		Path* newPath = refinePath(path, &getNode(startNode->getParent(this, level)), path->tail());
		newPath->truncate(k);
		delete path;
		path = newPath;
	}
	return path;
}

Node& Graph::getNode(uint32 id)
{
	return nodeHeap[id];
}

Node* Graph::getNodeAt(BlockPos pos)
{
	for (uint32 root : this->rootNodes)
	{
		Node* node = getNode(root).getNodeAt(pos, this);
		if (node != nullptr)
			return node;
	}
	return nullptr;
}

void Graph::addChunk(ChunkPos pos, PathFindingBlockState* data)
{
	for(uint16 x = 0; x < 16; x++)
		for(uint16 y = 0; y < 16; y++)
			for (uint16 z = 0; z < 16; z++)
			{
				BlockPos bPos = BlockPos(pos.x * 16 + x, pos.y * 16 + y, pos.z * 16 + z);
				setBlockState(bPos, data[z * (16 * 16) + y * 16 + x]);
			}
}

void Graph::setBlockState(BlockPos pos, PathFindingBlockState state)
{
	uint32 nNodeID = newNode(pos, 1, Node::NODE_INVALID, std::unordered_set<uint32>(), std::unordered_map<uint32, uint32>(), state);
	Node* nNode = &getNode(nNodeID);

	BlockPos neighborsPos[] = {
		BlockPos(pos.x - 1, pos.y, pos.z),
		BlockPos(pos.x + 1, pos.y, pos.z),
		BlockPos(pos.x, pos.y - 1, pos.z),
		BlockPos(pos.x, pos.y + 1, pos.z),
		BlockPos(pos.x, pos.y, pos.z - 1),
		BlockPos(pos.x, pos.y, pos.z + 1),
	};
	for (BlockPos neighborPos : neighborsPos)
	{

	}
}
