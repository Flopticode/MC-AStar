#pragma once

#include "MinecraftTypes.h"
#include "PathFindingWorld.h"
#include "AStar.h"

typedef enum {
	MC_ASTAR_NO_ERROR = 0,
	MC_ASTAR_WORLD_INST_FAILED = 1,
	MC_ASTAR_NULLPTR = 2,
	MC_ASTAR_INVALID_POSITION = 3,
	MC_ASTAR_ALREADY_INITIALIZED = 4,
	MC_ASTAR_NOT_INITIALIZED = 5,
	MC_ASTAR_ALLOCATION_FAILED = 6
} MCAStarError;

MCAStarError init();
MCAStarError calculatePath(Path*& out, BlockPos start, BlockPos end);
MCAStarError addChunk(ChunkPos pos, PathFindingChunk* chunk);
MCAStarError setBlock(BlockPos pos, PathFindingBlockState state);
