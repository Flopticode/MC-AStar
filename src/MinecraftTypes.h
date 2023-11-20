#pragma once

#include "types.h"
#include <math.h>
#include <string>

typedef struct _ChunkPos {
	int32 x;
	int32 y;
	int32 z;

	_ChunkPos(int32 x, int32 y, int32 z);

	bool operator==(const _ChunkPos& other) const;
} ChunkPos;
namespace std {
	template<>
	struct hash<ChunkPos> {
		size_t operator()(const ChunkPos& obj) const;
	};
}

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

	ChunkPos toChunkPos();

} BlockPos;
namespace std {
	template<>
	struct hash<BlockPos> {
		size_t operator()(const BlockPos& obj) const;
	};
}

typedef struct _Dimension {
	uint64 width;
	uint64 depth;

	_Dimension(uint64 width, uint64 depth);
} Dimension;
typedef uint32 PathFindingBlockState;
