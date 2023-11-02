#pragma once

#include "types.h"
#include <math.h>
#include <string>

typedef struct _BlockPos {
	int64 x;
	int64 y;
	int64 z;

	_BlockPos(int64 x, int64 y, int64 z)
		:x(x), y(y), z(z)
	{

	}
	_BlockPos()
		:x(INT64_MIN), y(INT64_MIN), z(INT64_MIN)
	{

	}
	bool isInvalid()
	{
		return x == INT64_MIN && y == INT64_MIN && z == INT64_MIN;
	}
	bool operator==(const _BlockPos& other) const
	{
		return other.x == x && other.y == y && other.z == z;
	}
	bool operator<(const _BlockPos& other) const
	{
		if (other.x < x)
			return true;
		if (other.x > x)
			return false;

		if (other.y < y)
			return true;
		if (other.y > y)
			return false;

		if (other.z < z)
			return true;
		if (other.z > z)
			return false;

		return false;
	}
	uint64 dist(_BlockPos other)
	{
		int64 dX = x - other.x;
		int64 dY = y - other.y;
		int64 dZ = z - other.z;

		return (uint64) sqrt(dX * dX + dY * dY + dZ * dZ);
	}
	uint64 manhdist(_BlockPos other)
	{
		return abs(other.x - x) + abs(other.y - y) + abs(other.z - z);
	}
	std::string toString()
	{
		return "(" + std::to_string(x) + " | " + std::to_string(y) + " | " + std::to_string(z) + ")";
	}
	uint64 hash()
	{
		return 31 * (31 * (31 + x) + y) + z;
	}

} BlockPos;
namespace std {
	template<>
	struct hash<BlockPos> {
		size_t operator()(const BlockPos& obj) const {
			return 31 * (31 * (31 + obj.x) + obj.y) + obj.z;
		}
	};
}

typedef struct _Dimension {
	uint64 width;
	uint64 height;
	uint64 depth;

	_Dimension(uint64 width, uint64 height, uint64 depth)
		:width(width), height(height), depth(depth)
	{

	}
} Dimension;
typedef uint32 PathFindingBlockState;

typedef struct _ChunkPos {
	int32 x;
	int32 y;
	int32 z;

	_ChunkPos(int32 x, int32 y, int32 z)
		:x(x), y(y), z(z)
	{

	}
} ChunkPos;