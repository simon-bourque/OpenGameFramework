#pragma once

#include "Core/Core.h"

struct Matrix4f
{
	union {
		float32 values[16];
		struct {
			float32 m00;
			float32 m01;
			float32 m02;
			float32 m03;
			float32 m10;
			float32 m11;
			float32 m12;
			float32 m13;
			float32 m20;
			float32 m21;
			float32 m22;
			float32 m23;
			float32 m30;
			float32 m31;
			float32 m32;
			float32 m33;
		};
	};

	Matrix4f();
	Matrix4f(
		float32 m00, float32 m01, float32 m02, float32 m03,
		float32 m10, float32 m11, float32 m12, float32 m13,
		float32 m20, float32 m21, float32 m22, float32 m23,
		float32 m30, float32 m31, float32 m32, float32 m33
	);
	Matrix4f(const float32(&values)[16]);

	Matrix4f transpose() const;
	float32 det() const;
	Matrix4f inverse() const;

	Matrix4f operator+(const Matrix4f& m) const;
	Matrix4f operator-(const Matrix4f& m) const;
	Matrix4f operator*(const Matrix4f& m) const;
	Matrix4f operator*(float32 s) const;

	static Matrix4f translation(float32 dx, float32 dy, float32 dz);
	static Matrix4f scale(float32 sx, float32 sy, float32 sz);
	static Matrix4f rotationX(float32 angle);
	static Matrix4f rotationY(float32 angle);
	static Matrix4f rotationZ(float32 angle);
	static Matrix4f orthographic(float32 left, float32 right, float32 top, float32 bottom, float32 near, float32 far);
	static Matrix4f perspective(float32 fov, float32 aspectRatio, float32 near, float32 far);
};