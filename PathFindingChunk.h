#pragma once

#include "MinecraftTypes.h"

class PathFindingChunk
{
public:
	BlockPos getStart();
	BlockPos getEnd();

	PathFindingBlockState getBlockState(BlockPos pos);
	void setBlockState(BlockPos pos, PathFindingBlockState state);

	PathFindingChunk(BlockPos start, BlockPos end, PathFindingBlockState* data);
	~PathFindingChunk();

private:
	BlockPos start;
	BlockPos end;
	Dimension size;
	PathFindingBlockState* data;

	uint64 blockPosToArrayIndex(BlockPos pos);
	Dimension getSize();
};
