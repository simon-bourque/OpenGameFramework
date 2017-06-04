#pragma once

#include "Core/Core.h"

struct Glyph
{
	uint32 index;
	float32 width;
	float32 height;
	float32 xOffset;
	float32 yOffset;
	float32 xAdvance;
	float32 textCoords[12];
	float32 vertices[12];
public:
	Glyph();
	Glyph(uint32 index, float32 width, float32 height, float32 xOffset, float32 yOffset, float32 xAdvance, const float32 (&textCoords)[12], const float32 (&vertices)[12]);
};