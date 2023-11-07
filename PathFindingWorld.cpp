#include "PathFindingWorld.h"
#include <stdexcept>
#include <string>
#include "PathFindingBlockStateUtils.h"
#include <algorithm>
#include <iostream>

PathFindingWorld::PathFindingWorld()
	:chunks()
{
	chunks.resize(8);

	/* Initialize chunk directions */
	for (uint32 i = 0; i < 8; i++)
	{
		reservations.push_back(Dimension(10, WORLD_CHUNK_HEIGHT, 10));
		
		size_t numChunks = (size_t)10 * WORLD_CHUNK_HEIGHT * 10;
		chunks[i].resize(numChunks);

		for (uint32 j = 0; j < numChunks; j++)
			chunks[i][j] = nullptr;
	}
}
PathFindingWorld::~PathFindingWorld()
{
	for (auto& chunkDir : chunks)
		for (auto& chunk : chunkDir)
			delete chunk;
}

bool PathFindingWorld::isChunk(ChunkPos pos)
{
	auto dirIndex = getChunkDirectionIndex(pos);
	return chunks[dirIndex][chunkPosToArrayPos(dirIndex, pos)] != nullptr;
}
void PathFindingWorld::addChunk(ChunkPos pos, PathFindingChunk* chunk)
{
	auto dirIndex = getChunkDirectionIndex(pos);
	auto arrPos = chunkPosToArrayPos(dirIndex, pos);

	Dimension curReservation = reservations[dirIndex];
	std::vector<PathFindingChunk*>& dirChunks = chunks[dirIndex];

	if (arrPos >=
		(curReservation.width - 2)
		* (curReservation.height - 2)
		* (curReservation.depth - 2))
	{
		/* Using 10 chunks of buffer */
		curReservation.width = std::max((int)curReservation.width, pos.x + 10);
		curReservation.height = std::max((int)curReservation.height, pos.y + 10);
		curReservation.depth = std::max((int)curReservation.depth, pos.z + 10);

		size_t sizeBefore = dirChunks.size();
		dirChunks.resize(curReservation.width * curReservation.height * curReservation.depth);
		for (uint32 i = sizeBefore; i < dirChunks.size(); i++)
			dirChunks[i] = nullptr;

		reservations[dirIndex] = curReservation;
	}

	if (arrPos < dirChunks.size() && dirChunks[arrPos] != nullptr)
	{
		throw std::invalid_argument("There is already a chunk at chunk pos ("
		+ std::to_string(pos.x) + " | " + std::to_string(pos.y) + " | " + std::to_string(pos.z) + ")");
	}

	dirChunks[arrPos] = chunk;
}

void PathFindingWorld::removeChunk(ChunkPos pos)
{
	auto dirIndex = getChunkDirectionIndex(pos);
	chunks[dirIndex][chunkPosToArrayPos(dirIndex, pos)] = nullptr;
}

PathFindingBlockState PathFindingWorld::getBlockState(BlockPos pos)
{
	auto chunk = getChunkAt(pos);

	if (chunk == nullptr)
	{
		return PFBSUtils::createState(false, false, 255, true);
	}
	return chunk->getBlockState(pos);
}
void PathFindingWorld::setBlockState(BlockPos pos, PathFindingBlockState newState)
{
	getChunkAt(pos)->setBlockState(pos, newState);
}

PathFindingChunk* PathFindingWorld::getChunkAt(BlockPos pos)
{
	auto dirIndex = getChunkDirectionIndex(pos);
	auto arrPos = chunkPosToArrayPos(dirIndex, ChunkPos((int32)(abs(pos.x) / 16), (int32)(abs(pos.y) / 16), (int32)(abs(pos.z) / 16)));
	if (arrPos >= chunks[dirIndex].size())
		return nullptr;
	return chunks[dirIndex][arrPos];
}

uint64 PathFindingWorld::chunkPosToArrayPos(uint8 dirIndex, ChunkPos pos)
{
	Dimension reservation = reservations[dirIndex];
	return pos.z * (reservation.width * reservation.height)
		+ pos.y * (reservation.width)
		+ pos.x;
}

uint8 PathFindingWorld::getChunkDirectionIndex(BlockPos pos)
{
	return (((pos.z >= 0) << 2) | ((pos.y >= 0) << 1) | (pos.x >= 0));
}
uint8 PathFindingWorld::getChunkDirectionIndex(ChunkPos pos)
{
	return (((pos.z >= 0) << 2) | ((pos.y >= 0) << 1) | (pos.x >= 0));
}