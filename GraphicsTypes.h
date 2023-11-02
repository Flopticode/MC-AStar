#pragma once

#include "types.h"
#include <functional>

typedef struct _Point {
	uint16 x, y;

	_Point(uint16 x, uint16 y);

	double distance(_Point& other);
	uint32 distSqr(_Point& other);
} Point;
typedef struct _Color {
	double r, g, b;

	_Color(double r, double g, double b);
	_Color(double monoColor);
	_Color();

	double getRed();
	double getGreen();
	double getBlue();
	uint8 getRedInt();
	uint8 getGreenInt();
	uint8 getBlueInt();

	static _Color fromRGB(uint8 r, uint8 g, uint8 b);
	static _Color fromMonochromeInt(uint8 monoColor);

	
} Color;

static _Color operator-(const _Color& color, double dif)
{
	return _Color(color.r - dif, color.g - dif, color.b - dif);
}
static _Color operator-(double dif, const _Color& color)
{
	return _Color(dif - color.r, dif - color.g, dif - color.b);
}
static _Color operator/(const _Color& color, double dif)
{
	return _Color(color.r / dif, color.g / dif, color.b / dif);
}
static _Color operator/(double dif, const _Color& color)
{
	return _Color(dif / color.r, dif / color.g, dif / color.b);
}


typedef struct _Bitmap {
	uint8* pixels;
	uint16 width;
	uint16 height;

	_Bitmap(uint8* pixels, uint16 width, uint16 height);
	_Bitmap();
	~_Bitmap();

	void forEachPixel(std::function<Color(Point)> func);

	static uint8 getBitsPerPixel();
	static _Bitmap* create(uint16 width, uint16 height);
} Bitmap;