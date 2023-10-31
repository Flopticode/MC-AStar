#pragma once

#include "PathFindingWorld.h"
#include "Path.h"

namespace AStar
{
	Path* calculatePath(PathFindingWorld* world, BlockPos start, BlockPos end);
}