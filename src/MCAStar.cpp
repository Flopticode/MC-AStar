#include "MCAStar.h"
#include <iostream>

static bool initialized = false;
static Graph* world = nullptr;

MCAStarError init()
{
	if (initialized)
		return MC_ASTAR_ALREADY_INITIALIZED;

	world = new Graph();

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
	out = world->findPath(world->getNodeAt(start), world->getNodeAt(end), 1);
	return MCAStarError::MC_ASTAR_NO_ERROR;
}

MCAStarError addChunk(ChunkPos pos, PathFindingBlockState* chunk)
{
	if (!initialized)
		return MC_ASTAR_NOT_INITIALIZED;
	if (chunk == nullptr)
		return MCAStarError::MC_ASTAR_NULLPTR;
	if (world == nullptr)
		return MCAStarError::MC_ASTAR_NULLPTR;
	world->addChunk(pos, chunk);
	return MCAStarError::MC_ASTAR_NO_ERROR;
}

MCAStarError setBlock(BlockPos pos, PathFindingBlockState state)
{
	if (!initialized)
		return MC_ASTAR_NOT_INITIALIZED;
	world->setBlockState(pos, state);
	return MC_ASTAR_NO_ERROR;
}
