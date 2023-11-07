#pragma once

#include "PathFindingChunk.h"
#include <vector>
#include <unordered_map>

class PathFindingWorld
{
public:
	static constexpr uint8 WORLD_CHUNK_HEIGHT = 24;

	PathFindingWorld();
	~PathFindingWorld();

	void addChunk(ChunkPos pos, PathFindingChunk* chunk);
	bool isChunk(ChunkPos pos);
	void removeChunk(ChunkPos pos);
	PathFindingBlockState getBlockState(BlockPos pos);
	void setBlockState(BlockPos pos, PathFindingBlockState newState);

private:
	/* An element of this vector contains all the chunks located in the
	same direction relative to the (0|0|0) point. The vector will contain
	exactly 8 elements (one for every direction):
	+x +y +z
	+x +y -z
	+x -y +z
	+x -y -z
	-x +y +z
	-x +y -z
	-x -y +z
	-x -y -z
	To address one specific chunk, use the getChunkAt function.*/
	std::unordered_map<ChunkPos, PathFindingChunk*> chunks;
};