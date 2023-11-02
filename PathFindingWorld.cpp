#include "PathFindingWorld.h"
#include <stdexcept>
#include <string>
#include "PathFindingBlockStateUtils.h"
#include <algorithm>

PathFindingWorld::PathFindingWorld()
	:reservation(10, WORLD_CHUNK_HEIGHT, 10),
	chunks()
{
	chunks.resize(reservation.width * reservation.height * reservation.depth);
}

void PathFindingWorld::addChunk(ChunkPos pos, PathFindingChunk* chunk)
{
	auto arrPos = chunkPosToArrayPos(pos);

	if (arrPos >= (reservation.width - 1) * (reservation.height - 1) * (reservation.depth - 1))
	{
		/* Using 10 chunks of buffer */
		reservation.width = std::max((int)reservation.width, pos.x + 10);
		reservation.height = std::max((int)reservation.height, pos.y + 10);
		reservation.depth = std::max((int)reservation.depth, pos.z + 10);
		chunks.resize(reservation.width * reservation.height * reservation.depth);
	}

	if (arrPos < chunks.size() && chunks[arrPos] != nullptr)
	{
		throw std::invalid_argument("There is already a chunk at chunk pos ("
		+ std::to_string(pos.x) + " | " + std::to_string(pos.y) + " | " + std::to_string(pos.z) + ")");
	}
	chunks[arrPos] = chunk;
}

void PathFindingWorld::removeChunk(ChunkPos pos)
{
	chunks[chunkPosToArrayPos(pos)] = nullptr;
}

PathFindingBlockState PathFindingWorld::getBlockState(BlockPos pos)
{
	auto chunk = getChunkAt(pos);

	if (chunk == nullptr)
		return PFBSUtils::createState(false, false, 255, true);
	return chunk->getBlockState(pos);
}
void PathFindingWorld::setBlockState(BlockPos pos, PathFindingBlockState newState)
{
	getChunkAt(pos)->setBlockState(pos, newState);
}

PathFindingChunk* PathFindingWorld::getChunkAt(BlockPos pos)
{
	auto arrPos = chunkPosToArrayPos(ChunkPos((int32)(pos.x / 16), (int32)(pos.y / 16), (int32)(pos.z / 16)));
	return chunks[arrPos];
}

uint64 PathFindingWorld::chunkPosToArrayPos(ChunkPos pos)
{
	return pos.z * (reservation.width * reservation.height)
		+ pos.y * (reservation.width)
		+ pos.x;
}
