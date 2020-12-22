#pragma once

#include "Core/Core.h"

struct Vector4f
{
	const static Vector4f X_AXIS;
	const static Vector4f Y_AXIS;
	const static Vector4f Z_AXIS;
	const static Vector4f W_AXIS;

	union {
		float32 values[2];
		struct {
			float32 x;
			float32 y;
			float32 z;
			float32 w;
		};
	};

	Vector4f();
	Vector4f(float32 x, float32 y, float32 z, float32 w);
	Vector4f(const float32(&values)[4]);

	float32 dot(const Vector4f& v) const;

	Vector4f normalize() const;
	Vector4f reverse() const;

	float32 length() const;
	float32 lengthSquared() const;

	Vector4f operator+(const Vector4f& v) const;
	Vector4f operator+(float32 s) const;
	Vector4f operator-(const Vector4f& v) const;
	Vector4f operator-(float32 s) const;
	Vector4f operator*(float32 s) const;
	Vector4f operator/(float32 s) const;
	bool operator==(const Vector4f& v) const;
};