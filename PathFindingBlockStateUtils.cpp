#include "PathFindingBlockStateUtils.h"

bool PFBSUtils::canWalkOn(PathFindingBlockState state)
{
	return state & 0b1;
}

uint8 PFBSUtils::getBreakDelay(PathFindingBlockState state)
{
	return (uint8)((state & 0b111111110) >> 1);
}

bool PFBSUtils::canWalkThrough(PathFindingBlockState state)
{
	return state & 0b1000000000;
}

bool PFBSUtils::isInvalid(PathFindingBlockState state)
{
	return state & 0b10000000000;
}

uint8 PFBSUtils::getDebugData(PathFindingBlockState state)
{
	return (state & 0b1111111100000000000) >> 11;
}
PathFindingBlockState PFBSUtils::setDebugData(PathFindingBlockState state, uint8 debugData)
{
	state &= 0b11111111111110000000011111111111;
	state |= (debugData << 11);
	return state;
}

PathFindingBlockState PFBSUtils::createState(bool canWalkOn, bool canWalkThrough, uint8 breakDelay, bool invalid)
{
	return (canWalkOn << 0)
		| (breakDelay << 1)
		| (canWalkThrough << 9)
		| (invalid << 10);
}
