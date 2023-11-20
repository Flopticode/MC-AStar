#pragma once

#include "MinecraftTypes.h"
#include "Graph.h"
#include <list>
#include "Node.h"

class Path
{
public:
	Path(std::list<Node*> data);
	std::list<Node*> get();
	void truncate(uint32 length);
	Node* tail();

private:
	std::list<Node*> data;
};