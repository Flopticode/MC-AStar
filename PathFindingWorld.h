#pragma once

#include "PathFindingChunk.h"
#include <vector>

class PathFindingWorld
{
public:
	static constexpr uint8 WORLD_CHUNK_HEIGHT = 24;

	PathFindingWorld();

	void addChunk(ChunkPos pos, PathFindingChunk* chunk);
	void removeChunk(ChunkPos pos);
	PathFindingBlockState getBlockState(BlockPos pos);

private:
	Dimension reservation;
	std::vector<PathFindingChunk*> chunks;

	PathFindingChunk* getChunkAt(BlockPos pos);
	uint64 chunkPosToArrayPos(ChunkPos pos);
};