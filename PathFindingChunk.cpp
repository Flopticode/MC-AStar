#include "PathFindingChunk.h"
#include <algorithm>
#include <stdexcept>
#include <string>
#include "PathFindingBlockStateUtils.h"

using namespace std;

BlockPos PathFindingChunk::getStart()
{
	return start;
}
BlockPos PathFindingChunk::getEnd()
{
	return end;
}

PathFindingBlockState PathFindingChunk::getBlockState(BlockPos pos)
{
	uint64 index;
	try
	{
		index = blockPosToArrayIndex(pos);
	}
	catch (std::invalid_argument&)
	{
		return PFBSUtils::createState(false, false, 255, true);
	}

	return data[index];
}
void PathFindingChunk::setBlockState(BlockPos pos, PathFindingBlockState state)
{
	data[blockPosToArrayIndex(pos)] = state;
}

PathFindingChunk::PathFindingChunk(BlockPos start, PathFindingBlockState* data)
	:data(data),
	start(start),
	end(start.x + 15, start.y + 15, start.z + 15)
{

}
PathFindingChunk::~PathFindingChunk()
{
	delete[] data;
}

uint64 PathFindingChunk::blockPosToArrayIndex(BlockPos pos)
{
	if (pos.x < start.x || pos.y < start.y || pos.z < start.z
		|| pos.x > end.x || pos.y > end.y || pos.z > end.z)
	{
		throw std::invalid_argument("BlockPos ("
			+ std::to_string(pos.x) + " | "
			+ std::to_string(pos.y) + " | "
			+ std::to_string(pos.z) + ") is not in this chunk!");
	}

	BlockPos relative = BlockPos(pos.x - start.x, pos.y - start.y, pos.z - start.z);
	return relative.z * 16 * 16 + relative.y * 16 + relative.x;
}
