#pragma once

#include "PathFindingWorld.h"
#include "Path.h"
#include "WorldRenderer.h"

namespace AStar
{
	Path* calculatePath(uint32& closedNodes, uint32& openNodes, WorldRenderer* wr, PathFindingWorld* world, BlockPos start, BlockPos end);
}