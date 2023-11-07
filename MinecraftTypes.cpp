#include "MinecraftTypes.h"

#pragma once

#include "types.h"
#include <math.h>
#include <string>

_BlockPos::_BlockPos(int32 x, int32 y, int32 z)
	:x(x), y(y), z(z)
{

}
_BlockPos::_BlockPos()
	:x(INT32_MIN), y(INT32_MIN), z(INT32_MIN)
{

}
bool _BlockPos::isInvalid()
{
	return x == INT32_MIN && y == INT32_MIN && z == INT32_MIN;
}
bool _BlockPos::operator==(const _BlockPos& other) const
{
	return other.x == x && other.y == y && other.z == z;
}
bool _BlockPos::operator<(const _BlockPos& other) const
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
uint32 _BlockPos::dist(_BlockPos other)
{
	/* subtraction will not cause an underflow because Minecraft worlds are
	way too small for that to happen */
	int32 dX = x - other.x;
	int32 dY = y - other.y;
	int32 dZ = z - other.z;

	/* cast to double will not be an issue due to Minecraft not being able to
	load 94,906,265 at the same time */
	return (uint32)sqrt((double)(dX * dX + dY * dY + dZ * dZ));
}
uint32 _BlockPos::manhdist(_BlockPos other)
{
	/* this will not cause an int32 overflow due to Minecraft worlds not being
	large enough */
	return abs(other.x - x) + abs(other.y - y) + abs(other.z - z);
}
std::string _BlockPos::toString()
{
	return "(" + std::to_string(x)
		+ " | " + std::to_string(y)
		+ " | " + std::to_string(z) + ")";
}
uint64 _BlockPos::hash()
{
	/* still not causing an overflow due to the maximum for x, y, z being 60,000,000 */
	return (uint64)31 * (31 * (31 + x) + y) + z;
}
ChunkPos _BlockPos::toChunkPos()
{
	return ChunkPos(x / 16, y / 16, z / 16);
}

size_t std::hash<BlockPos>::operator()(const BlockPos& obj) const
{
	/* don't trust intellisense, this will not cause an overflow */
	return (uint64)31 * (31 * (31 + obj.x) + obj.y) + obj.z;
}

bool _ChunkPos::operator==(const _ChunkPos& other) const
{
	return x == other.x && y == other.y && z == other.z;
}

_Dimension::_Dimension(uint64 width, uint64 depth)
	:width(width), depth(depth)
{

}

_ChunkPos::_ChunkPos(int32 x, int32 y, int32 z)
	:x(x), y(y), z(z)
{

}

size_t std::hash<_ChunkPos>::operator()(const _ChunkPos& obj) const
{
	/* don't trust intellisense, this will not cause an overflow */
	return (uint64)31 * (31 * (31 + obj.x) + obj.y) + obj.z;
}