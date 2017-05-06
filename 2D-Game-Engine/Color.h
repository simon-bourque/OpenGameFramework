#ifndef COLOR_H
#define COLOR_H

#include "Core.h"

struct Color
{
	const static Color BLACK;
	const static Color WHITE;
	const static Color GREY;
	const static Color RED;
	const static Color GREEN;
	const static Color BLUE;
	const static Color CYAN;
	const static Color MAGENTA;
	const static Color YELLOW;

	union {
		float32 values[4];
		struct {
			float32 r;
			float32 g;
			float32 b;
			float32 a;
		};
	};

	Color();
	Color(float32 r, float32 g, float32 b, float32 a = 1.0f);
	Color(uint8 r, uint8 g, uint8 b, uint8 a = 255);
	virtual ~Color();

	static Color createColorHexRGB(uint32 rgb);
};

#endif