#include "PathFindingWorld.h"
#include <stdexcept>
#include <string>
#include "PathFindingBlockStateUtils.h"
#include <algorithm>
#include <iostream>

PathFindingWorld::PathFindingWorld()
	:chunks()
{
	chunks.rehash(100000);
}
PathFindingWorld::~PathFindingWorld()
{
	for (auto& entry : chunks)
		delete entry.second;
}

bool PathFindingWorld::isChunk(ChunkPos pos)
{
	return chunks.count(pos) > 0;
}
void PathFindingWorld::addChunk(ChunkPos pos, PathFindingChunk* chunk)
{
	if (isChunk(pos))
	{
		throw std::invalid_argument("There is already a chunk at chunk pos ("
		+ std::to_string(pos.x) + " | " + std::to_string(pos.y) + " | " + std::to_string(pos.z) + ")");
	}
	chunks[pos] = chunk;
}

void PathFindingWorld::removeChunk(ChunkPos pos)
{
	chunks.erase(pos);
}

PathFindingBlockState PathFindingWorld::getBlockState(BlockPos pos)
{
	auto chunk = chunks[pos.toChunkPos()];

	if (chunk == nullptr)
	{
		return PFBSUtils::createState(false, false, 255, true);
	}
	return chunk->getBlockState(pos);
}

void PathFindingWorld::setBlockState(BlockPos pos, PathFindingBlockState newState)
{
	chunks[pos.toChunkPos()]->setBlockState(pos, newState);
}
