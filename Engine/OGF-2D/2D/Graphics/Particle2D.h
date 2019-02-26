#pragma once

#include "Core/Math/Vector2f.h"
#include "Core/Graphics/Color.h"

struct Particle2D {
	Vector2f position;
	Vector2f velocity;
	float32 lifespan;
	float32 life;
	Color color;
};