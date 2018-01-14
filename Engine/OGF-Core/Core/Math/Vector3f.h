#pragma once

#include "Core/Core.h"

struct Vector3f
{
	const static Vector3f X_AXIS;
	const static Vector3f Y_AXIS;
	const static Vector3f Z_AXIS;

	union {
		float32 values[3];
		struct {
			float32 x;
			float32 y;
			float32 z;
		};
	};

	Vector3f();
	Vector3f(float32 x, float32 y, float32 z);
	Vector3f(const float32(&values)[3]);

	float32 dot(const Vector3f& v) const;
	Vector3f cross(const Vector3f& v) const;

	Vector3f normalize() const;
	Vector3f reverse() const;

	float32 length() const;
	float32 lengthSquared() const;

	Vector3f operator+(const Vector3f& v) const;
	Vector3f operator+(float32 s) const;
	Vector3f operator-(const Vector3f& v) const;
	Vector3f operator-(float32 s) const;
	Vector3f operator*(float32 s) const;
	Vector3f operator/(float32 s) const;
	bool operator==(const Vector3f& v) const;
};