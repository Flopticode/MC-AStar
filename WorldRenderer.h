#pragma once

#include "MinecraftTypes.h"
#include "PathFindingWorld.h"
#include "GraphicsTypes.h"
#include <mutex>

class WorldRenderer
{
public:
	WorldRenderer(PathFindingWorld* world);
	void render();

private:
	PathFindingWorld* world;
	
	typedef struct {
		std::mutex bitmapMutex;
		Bitmap* bmp;
	} BitmapInfo;
	BitmapInfo bmpInfo;
};