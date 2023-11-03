#pragma once

#include "types.h"
#include <math.h>
#include <string>

typedef struct _BlockPos {
	int32 x;
	int32 y;
	int32 z;

	_BlockPos(int32 x, int32 y, int32 z);
	_BlockPos();
	bool isInvalid();
	bool operator==(const _BlockPos& other) const;
	bool operator<(const _BlockPos& other) const;
	uint32 dist(_BlockPos other);
	uint32 manhdist(_BlockPos other);
	std::string toString();
	uint64 hash();

} BlockPos;
namespace std {
	template<>
	struct hash<BlockPos> {
		size_t operator()(const BlockPos& obj) const;
	};
}

typedef struct _Dimension {
	uint64 width;
	uint64 height;
	uint64 depth;

	_Dimension(uint64 width, uint64 height, uint64 depth);
} Dimension;
typedef uint32 PathFindingBlockState;

typedef struct _ChunkPos {
	int32 x;
	int32 y;
	int32 z;

	_ChunkPos(int32 x, int32 y, int32 z);
} ChunkPos;
