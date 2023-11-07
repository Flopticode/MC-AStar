#include "MCAStar.h"
#include <iostream>

static bool initialized = false;
static PathFindingWorld* world = nullptr;

MCAStarError init()
{
	if (initialized)
		return MC_ASTAR_ALREADY_INITIALIZED;

	world = new PathFindingWorld();

	/* e.g. OOM */
	if (world == nullptr)
		return MC_ASTAR_WORLD_INST_FAILED;

	initialized = true;

	return MC_ASTAR_NO_ERROR;
}

MCAStarError calculatePath(Path*& out, BlockPos start, BlockPos end)
{
	if (!initialized)
		return MCAStarError::MC_ASTAR_NOT_INITIALIZED;
	if (&out == nullptr)
		return MCAStarError::MC_ASTAR_NULLPTR;
	out = AStar::calculatePath(nullptr, world, start, end);
	return MCAStarError::MC_ASTAR_NO_ERROR;
}

MCAStarError addChunk(ChunkPos pos, PathFindingChunk* chunk)
{
	if (!initialized)
		return MC_ASTAR_NOT_INITIALIZED;
	if (chunk == nullptr)
		return MCAStarError::MC_ASTAR_NULLPTR;
	if (world == nullptr)
		return MCAStarError::MC_ASTAR_NULLPTR;
	if (world->isChunk(pos))
		return MCAStarError::MC_ASTAR_INVALID_POSITION;
	world->addChunk(pos, chunk);
	return MCAStarError::MC_ASTAR_NO_ERROR;
}

MCAStarError setBlock(BlockPos pos, PathFindingBlockState state)
{
	if (!initialized)
		return MC_ASTAR_NOT_INITIALIZED;
	if (!world->isChunk(ChunkPos(pos.x / 16, pos.y / 16, pos.z / 16)))
		return MC_ASTAR_INVALID_POSITION;
	world->setBlockState(pos, state);
	return MC_ASTAR_NO_ERROR;
}
