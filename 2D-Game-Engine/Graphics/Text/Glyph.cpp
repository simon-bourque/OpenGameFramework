#include "Glyph.h"

Glyph::Glyph() : index(0), width(0), height(0), xOffset(0), yOffset(0), xAdvance(0)
{
	for (int32 i = 0; i < 12; i++) {
		this->textCoords[i] = 0;
		this->vertices[i] = 0;
	}
}


Glyph::Glyph(uint32 index, float32 width, float32 height, float32 xOffset, float32 yOffset, float32 xAdvance, const float32(&textCoords)[12], const float32(&vertices)[12])
	: index(index),
	width(width),
	height(height),
	xOffset(xOffset),
	yOffset(yOffset),
	xAdvance(xAdvance)
{
	for (int32 i = 0; i < 12; i++) {
		this->textCoords[i] = textCoords[i];
		this->vertices[i] = vertices[i];
	}
}
