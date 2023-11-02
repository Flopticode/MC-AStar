#pragma once

#include <functional>
#include "types.h"
#include "GraphicsTypes.h"

namespace WindowHandler
{
	enum InitMode {
		NORMAL = 1,
		WALLPAPER = 2
	};

	int init(InitMode initMode, bool pManualRepaint, std::function<Bitmap*()> onDraw);
	void update();
	void repaint();
}