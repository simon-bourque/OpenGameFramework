#pragma once
#ifndef MATRIX3F_H
#define MATRIX3F_H

#include "Core/Core.h"

struct Matrix3f
{
	union {
		float32 values[9];
		struct {
			float32 m00;
			float32 m01;
			float32 m02;
			float32 m10;
			float32 m11;
			float32 m12;
			float32 m20;
			float32 m21;
			float32 m22;
		};
	};

	Matrix3f();
	Matrix3f(float32 m00, float32 m01, float32 m02, float32 m10, float32 m11, float32 m12, float32 m20, float32 m21, float32 m22);
	Matrix3f(const float32 (&values)[9]);

	Matrix3f transpose() const;
	float32 det() const;
	Matrix3f inverse() const;

	Matrix3f operator+(const Matrix3f& m) const;
	Matrix3f operator-(const Matrix3f& m) const;
	Matrix3f operator*(const Matrix3f& m) const;
	Matrix3f operator*(float32 s) const;

	static Matrix3f translation(float32 dx, float32 dy);
	static Matrix3f scale(float32 sx, float32 sy);
	static Matrix3f rotation(float32 angle);
	static Matrix3f orthographic(float32 l, float32 r, float32 t, float32 b, float32 n, float32 f);
};

#endif
