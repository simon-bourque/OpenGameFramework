#pragma once

#include "Core/Core.h"

struct Matrix3f;
struct Vector2f;

struct Transform
{
	float32 xPos;
	float32 yPos;
	float32 rotation;
	float32 xScale;
	float32 yScale;

	Transform();
	Transform(float32 xPos, float32 yPos, float32 rotation = 0, float32 xScale = 1, float32 yScale = 1);
	void translate(float32 dx, float32 dy);
	void translate(const Vector2f& displacement);
	void rotate(float32 angle);
	void scale(float32 scale);
	Matrix3f toMatrix() const;
};

