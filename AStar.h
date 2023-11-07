#pragma once

#include "PathFindingWorld.h"
#include "Path.h"
#include "WorldRenderer.h"

namespace AStar
{
	Path* calculatePath(WorldRenderer* wr, PathFindingWorld* world, BlockPos start, BlockPos end);
}