#pragma once
#ifndef VECTOR2F_H
#define VECTOR2F_H

#include "Core/Core.h"

struct Vector2f
{
	const static Vector2f X_AXIS;
	const static Vector2f Y_AXIS;

	union {
		float32 values[2];
		struct {
			float32 x;
			float32 y;
		};
	};

	Vector2f();
	Vector2f(float32 x, float32 y);
	Vector2f(const float32 (&values)[2]);

	float32 dot(const Vector2f& v) const;
	
	Vector2f normalize() const;
	Vector2f reverse() const;
	
	float32 length() const;
	float32 lengthSquared() const;

	Vector2f operator+(const Vector2f& v) const;
	Vector2f operator+(float32 s) const;
	Vector2f operator-(const Vector2f& v) const;
	Vector2f operator-(float32 s) const;
	Vector2f operator*(float32 s) const;
	Vector2f operator/(float32 s) const;
};

#endif

