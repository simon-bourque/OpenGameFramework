#pragma once

#include "Core/Math/Vector2f.h"

#include <vector>

class Shape;

struct Manifold {
public:
	const Shape* colA;
	const Shape* colB;
	float32 minDepth;
	Vector2f depth;
	Vector2f direction;
	std::vector<Vector2f> contactPoints;
	Manifold();
	virtual ~Manifold();
};