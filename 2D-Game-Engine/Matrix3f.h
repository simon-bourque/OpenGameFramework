#ifndef MATRIX3F_H
#define MATRIX3F_H

#include "Core.h"

struct Matrix3f
{
	float32 m00 = 1.0f;
	float32 m01 = 0.0f;
	float32 m02 = 0.0f;
	float32 m10 = 0.0f;
	float32 m11 = 1.0f;
	float32 m12 = 0.0f;
	float32 m20 = 0.0f;
	float32 m21 = 0.0f;
	float32 m22 = 1.0f;

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
