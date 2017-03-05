#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Core.h"

struct Matrix3f;

struct Transform
{
	float32 xPos = 0.0f;
	float32 yPos = 0.0f;
	float32 rot = 0.0f;
	float32 xScale = 1.0f;
	float32 yScale = 1.0f;

	void translate(float32 dx, float32 dy);
	void scale(float32 scale);
	Matrix3f toMatrix() const;
};

#endif

