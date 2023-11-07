#pragma once

#include "MinecraftTypes.h"
#include <list>

class Path
{
public:
	Path(std::list<BlockPos> data);
	std::list<BlockPos> get();

private:
	std::list<BlockPos> data;
};