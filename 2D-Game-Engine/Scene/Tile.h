#pragma once

#include "Core/Core.h"

struct Tile
{
	float32 x;
	float32 y;
	int32 index;
	Tile();
	Tile(float32 x, float32 y, int32 index);
};