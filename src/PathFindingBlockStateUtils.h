#pragma once

#include "MinecraftTypes.h"

namespace PFBSUtils
{
	bool canWalkOn(PathFindingBlockState state);
	uint8 getBreakDelay(PathFindingBlockState state);
	bool canWalkThrough(PathFindingBlockState state);
	bool isInvalid(PathFindingBlockState state);
	uint8 getDebugData(PathFindingBlockState state);
	PathFindingBlockState setDebugData(PathFindingBlockState state, uint8 debugData);

	PathFindingBlockState createState(bool canWalkOn, bool canWalkThrough, uint8 breakDelay, bool invalid);
}