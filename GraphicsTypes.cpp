#include "GraphicsTypes.h"
#include <math.h>
#include <thread>
#include <vector>
#include <string>

#define max(a, b) ((a > b ? a : b))
#define min(a, b) ((a < b ? a : b))

auto numCores = std::thread::hardware_concurrency();

_Bitmap::_Bitmap(uint8* pixels, uint16 width, uint16 height)
	:pixels(pixels), width(width), height(height)
{

}
_Bitmap::_Bitmap() :pixels(nullptr), width(0), height(0)
{

}
_Bitmap::~_Bitmap()
{
	if(pixels != nullptr)
		delete[] pixels;
}

uint8 _Bitmap::getBitsPerPixel()
{
	return 24;
}
_Bitmap* _Bitmap::create(uint16 width, uint16 height)
{
	return new _Bitmap(new uint8[(uint32)width * height * getBitsPerPixel()], width, height);
}
void _Bitmap::forEachPixel(std::function<Color(Point)> func)
{
	if (numCores == 0)
		numCores = 1;

	std::vector<std::thread> threads = std::vector<std::thread>();
	threads.reserve(numCores);
	for (uint16 i = 0; i < numCores; i++)
	{
		uint16 rangeStart = (width / numCores) * i;
		uint16 rangeEnd = (width / numCores) * (i+1);

		threads.push_back(std::thread([rangeStart, rangeEnd, this, func]() {
			for (uint16 x = rangeStart; x < rangeEnd; x++)
			{
				for (uint16 y = 0; y < height; y++)
				{
					Point pos = { x, y };
					Color c = func(pos);
					c.r = max(0, min(c.r, 1));
					c.g = max(0, min(c.g, 1));
					c.b = max(0, min(c.b, 1));
					uint8* pixelPtr = &pixels[(y * width + x) * 3];
					pixelPtr[0] = c.getBlueInt();
					pixelPtr[1] = c.getGreenInt();
					pixelPtr[2] = c.getRedInt();
				}
			}
		}));
	}

	for (std::thread& t : threads)
	{
		t.join();
	}
}

_Point::_Point(uint16 x, uint16 y)
	:x(x),y(y)
{

}
double _Point::distance(_Point& other)
{
	return sqrt(distSqr(other));
}
uint32 _Point::distSqr(_Point& other)
{
	int32 dX = x - other.x;
	int32 dY = y - other.y;
	return (uint16)(dX * dX + dY * dY);
}

_Color::_Color(double r, double g, double b)
	:r(r),g(g),b(b)
{

}
_Color::_Color(double monoColor)
	:r(monoColor), g(monoColor), b(monoColor)
{

}
_Color::_Color()
	:r(0),g(0),b(0)
{

}

double _Color::getRed()
{
	return r;
}
double _Color::getGreen()
{
	return g;
}
double _Color::getBlue()
{
	return b;
}
uint8 _Color::getRedInt()
{
	return (uint8)(r * 255);
}
uint8 _Color::getGreenInt()
{
	return (uint8)(g * 255);
}
uint8 _Color::getBlueInt()
{
	return (uint8)(b * 255);
}

_Color _Color::fromRGB(uint8 r, uint8 g, uint8 b)
{
	return _Color((double)r / 255, (double)g / 255, (double)b / 255);
}
_Color _Color::fromMonochromeInt(uint8 monoColor)
{
	return _Color((double)monoColor / 255);
}
