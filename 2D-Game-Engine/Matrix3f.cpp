#include "Matrix3f.h"
#include <math.h>

Matrix3f Matrix3f::transpose() const {
	Matrix3f tmp;

	tmp.m00 = m00;
	tmp.m01 = m10;
	tmp.m02 = m20;
	tmp.m10 = m01;
	tmp.m11 = m11;
	tmp.m12 = m21;
	tmp.m20 = m02;
	tmp.m21 = m12;
	tmp.m22 = m22;

	return tmp;
}

float32 Matrix3f::det() const {
	return m00 * m11 * m22 + m01 * m12 * m20 + m02 * m10 * m21 - m01 * m10 * m22 - m00 * m12 * m21 - m02 * m11 * m20;
}

Matrix3f Matrix3f::inverse() const {
	// TODO check if actually invertible
	Matrix3f tmp;

	float32 invDet = 1 / det();

	// First row
	tmp.m00 = (m11 * m22 - m12 * m21) * invDet;
	tmp.m10 = (m20 * m12 - m10 * m22) * invDet;
	tmp.m20 = (m10 * m21 - m20 * m11) * invDet;

	// Second row
	tmp.m01 = (m21 * m02 - m01 * m22) * invDet;
	tmp.m11 = (m00 * m22 - m20 * m02) * invDet;
	tmp.m21 = (m20 * m01 - m00 * m21) * invDet;

	// Third row
	tmp.m02 = (m01 * m12 - m11 * m02) * invDet;
	tmp.m12 = (m10 * m02 - m00 * m12) * invDet;
	tmp.m22 = (m00 * m11 - m10 * m01) * invDet;

	return tmp;
}

Matrix3f Matrix3f::operator+(const Matrix3f& m) {
	Matrix3f tmp;
	tmp.m00 = m00 + m.m00;
	tmp.m01 = m01 + m.m01;
	tmp.m02 = m02 + m.m02;
	tmp.m10 = m10 + m.m10;
	tmp.m11 = m11 + m.m11;
	tmp.m12 = m12 + m.m12;
	tmp.m20 = m20 + m.m20;
	tmp.m21 = m21 + m.m21;
	tmp.m22 = m22 + m.m22;
	return tmp;
}

Matrix3f Matrix3f::operator-(const Matrix3f& m) {
	Matrix3f tmp;
	tmp.m00 = m00 - m.m00;
	tmp.m01 = m01 - m.m01;
	tmp.m02 = m02 - m.m02;
	tmp.m10 = m10 - m.m10;
	tmp.m11 = m11 - m.m11;
	tmp.m12 = m12 - m.m12;
	tmp.m20 = m20 - m.m20;
	tmp.m21 = m21 - m.m21;
	tmp.m22 = m22 - m.m22;
	return tmp;
}

Matrix3f Matrix3f::operator*(const Matrix3f& m) {
	Matrix3f tmp;
	// First row
	tmp.m00 = m00 * m.m00 + m01 * m.m10 + m02 * m.m20;
	tmp.m01 = m00 * m.m01 + m01 * m.m11 + m02 * m.m21;
	tmp.m02 = m00 * m.m02 + m01 * m.m12 + m02 * m.m22;

	// Second row
	tmp.m10 = m10 * m.m00 + m11 * m.m10 + m12 * m.m20;
	tmp.m11 = m10 * m.m01 + m11 * m.m11 + m12 * m.m21;
	tmp.m12 = m10 * m.m02 + m11 * m.m12 + m12 * m.m22;

	// Third row
	tmp.m20 = m20 * m.m00 + m21 * m.m10 + m22 * m.m20;
	tmp.m21 = m20 * m.m01 + m21 * m.m11 + m22 * m.m21;
	tmp.m22 = m20 * m.m02 + m21 * m.m12 + m22 * m.m22;

	return tmp;
}

Matrix3f Matrix3f::operator*(float32 s) {
	Matrix3f tmp;
	tmp.m00 = m00 * s;
	tmp.m01 = m01 * s;
	tmp.m02 = m02 * s;
	tmp.m10 = m10 * s;
	tmp.m11 = m11 * s;
	tmp.m12 = m12 * s;
	tmp.m20 = m20 * s;
	tmp.m21 = m21 * s;
	tmp.m22 = m22 * s;
	return tmp;
}

Matrix3f Matrix3f::translation(float32 dx, float32 dy) {
	Matrix3f transform;

	transform.m02 = dx;
	transform.m12 = dy;

	return transform;
}

Matrix3f Matrix3f::scale(float32 sx, float32 sy) {
	Matrix3f transform;

	transform.m00 = sx;
	transform.m11 = sy;

	return transform;
}

Matrix3f Matrix3f::rotation(float32 angle) {
	Matrix3f transform;

	transform.m00 = cos(angle);
	transform.m01 = -sin(angle);
	transform.m10 = sin(angle);
	transform.m11 = cos(angle);

	return transform;
}

Matrix3f Matrix3f::orthographic(float32 l, float32 r, float32 t, float32 b, float32 n, float32 f) {
	Matrix3f transform;

	transform.m00 = 2 / (r - l);
	transform.m11 = 2 / (t - b);
	transform.m02 = -((r + l) / (r - l));
	transform.m12 = -((t + b) / (t - b));

	return transform;
}
