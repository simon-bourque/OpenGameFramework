#pragma once
#ifndef MANIFOLD_H
#define MANIFOLD_H

#include "Math/Vector2f.h"

class Shape;

struct Manifold {
public:
	Shape* colA;
	Shape* colB;
	float32 depth;
	Vector2f direction;
	Manifold();
	Manifold(Shape* colA, Shape* colB, float32 depth, const Vector2f& direction);
	virtual ~Manifold();
};

#endif