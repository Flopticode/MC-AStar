#include "WorldRenderer.h"
#include "WindowDisplay.h"
#include "PathFindingBlockStateUtils.h"
#include <chrono>
#include <iostream>

WorldRenderer::WorldRenderer(PathFindingWorld* world)
	:world(world)
{
	render();
	WindowHandler::init(WindowHandler::NORMAL, true, [this]() -> Bitmap* {
		bmpInfo.bitmapMutex.lock();
		auto bmp = bmpInfo.bmp;
		bmpInfo.bitmapMutex.unlock();
		return bmp;
	});
}
void WorldRenderer::render()
{
	auto bmp = Bitmap::create(1920, 1080);
	
	auto ts1 = std::chrono::high_resolution_clock::now();
	bmp->forEachPixel([this](Point pt) {
		BlockPos blockPos(pt.x / 20, 0, pt.y / 20);
		auto state = world->getBlockState(blockPos);
		auto color = Color::fromMonochromeInt(PFBSUtils::getBreakDelay(state) * 25);
		color = color / 2;

		if (PFBSUtils::getDebugData(state) & 0b1)
			color.g = 1;
		if (PFBSUtils::getDebugData(state) & 0b10)
			color.r = 1;

		return color;
	});
	auto ts2 = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(ts2 - ts1);

	bmpInfo.bitmapMutex.lock();
	bmpInfo.bmp = bmp;
	bmpInfo.bitmapMutex.unlock();

	WindowHandler::repaint();
}
