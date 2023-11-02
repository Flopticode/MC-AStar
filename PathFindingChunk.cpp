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

PathFindingChunk::PathFindingChunk(BlockPos start, BlockPos end, PathFindingBlockState* data)
	:data(data),
	start(BlockPos(min(start.x, end.x), min(start.y, end.y), min(start.z, end.z))),
	end(BlockPos(max(start.x, end.x), max(start.y, end.y), max(start.z, end.z))),
	size(Dimension(end.x - start.x + 1, end.y - start.y + 1, end.z - start.z + 1))
{

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
	return relative.z * (size.width * size.height) + relative.y * (size.width) + relative.x;
}

Dimension PathFindingChunk::getSize()
{
	return size;
}