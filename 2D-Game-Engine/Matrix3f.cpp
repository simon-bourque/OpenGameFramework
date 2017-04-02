#include "Matrix3f.h"
#include <math.h>

Matrix3f::Matrix3f() : 
	m00(1),
	m01(0),
	m02(0),
	m10(0),
	m11(1),
	m12(0),
	m20(0),
	m21(0),
	m22(1) 
{}

Matrix3f::Matrix3f(float32 m00, float32 m01, float32 m02, float32 m10, float32 m11, float32 m12, float32 m20, float32 m21, float32 m22) : 
	m00(m00),
	m01(m01),
	m02(m02),
	m10(m10),
	m11(m11),
	m12(m12),
	m20(m20),
	m21(m21),
	m22(m22) 
{}

Matrix3f::Matrix3f(const float32 (&values)[9]) : 
	m00(values[0]),
	m01(values[1]),
	m02(values[2]),
	m10(values[3]),
	m11(values[4]),
	m12(values[5]),
	m20(values[6]),
	m21(values[7]),
	m22(values[8])
{}

Matrix3f Matrix3f::transpose() const {
	return {
		m00,
		m10,
		m20,
		m01,
		m11,
		m21,
		m02,
		m12,
		m22
	};
}

float32 Matrix3f::det() const {
	return m00 * m11 * m22 + m01 * m12 * m20 + m02 * m10 * m21 - m01 * m10 * m22 - m00 * m12 * m21 - m02 * m11 * m20;
}

Matrix3f Matrix3f::inverse() const {
	// TODO check if actually invertible

	float32 invDet = 1 / det();

	return {
		(m11 * m22 - m12 * m21) * invDet,
		(m21 * m02 - m01 * m22) * invDet,
		(m01 * m12 - m11 * m02) * invDet,
		(m20 * m12 - m10 * m22) * invDet,
		(m00 * m22 - m20 * m02) * invDet,
		(m10 * m02 - m00 * m12) * invDet,
		(m10 * m21 - m20 * m11) * invDet,
		(m20 * m01 - m00 * m21) * invDet,
		(m00 * m11 - m10 * m01) * invDet
	};
}

Matrix3f Matrix3f::operator+(const Matrix3f& m) const {
	return {
		m00 + m.m00,
		m01 + m.m01,
		m02 + m.m02,
		m10 + m.m10,
		m11 + m.m11,
		m12 + m.m12,
		m20 + m.m20,
		m21 + m.m21,
		m22 + m.m22
	};
}

Matrix3f Matrix3f::operator-(const Matrix3f& m) const {
	return{
		m00 - m.m00,
		m01 - m.m01,
		m02 - m.m02,
		m10 - m.m10,
		m11 - m.m11,
		m12 - m.m12,
		m20 - m.m20,
		m21 - m.m21,
		m22 - m.m22
	};
}

Matrix3f Matrix3f::operator*(const Matrix3f& m) const {
	return {
		m00 * m.m00 + m01 * m.m10 + m02 * m.m20,
		m00 * m.m01 + m01 * m.m11 + m02 * m.m21,
		m00 * m.m02 + m01 * m.m12 + m02 * m.m22,
		m10 * m.m00 + m11 * m.m10 + m12 * m.m20,
		m10 * m.m01 + m11 * m.m11 + m12 * m.m21,
		m10 * m.m02 + m11 * m.m12 + m12 * m.m22,
		m20 * m.m00 + m21 * m.m10 + m22 * m.m20,
		m20 * m.m01 + m21 * m.m11 + m22 * m.m21,
		m20 * m.m02 + m21 * m.m12 + m22 * m.m22
	};
}

Matrix3f Matrix3f::operator*(float32 s) const {
	return {
		m00 * s,
		m01 * s,
		m02 * s,
		m10 * s,
		m11 * s,
		m12 * s,
		m20 * s,
		m21 * s,
		m22 * s
	};
}

Matrix3f Matrix3f::translation(float32 dx, float32 dy) {
	return {
		1.0f,
		0.0f,
		dx,
		0.0f,
		1.0f,
		dy,
		0.0f,
		0.0f,
		1.0f
	};
}

Matrix3f Matrix3f::scale(float32 sx, float32 sy) {
	return {
		sx,
		0.0f,
		0.0f,
		0.0f,
		sy,
		0.0f,
		0.0f,
		0.0f,
		1.0f
	};
}

Matrix3f Matrix3f::rotation(float32 angle) {
	return {
		cos(angle),
		-sin(angle),
		0.0f,
		sin(angle),
		cos(angle),
		0.0f,
		0.0f,
		0.0f,
		1.0f
	};
}

Matrix3f Matrix3f::orthographic(float32 l, float32 r, float32 t, float32 b, float32 n, float32 f) {
	return{
		2 / (r - l),
		0.0f,
		2 / (t - b),
		0.0f,
		2 / (t - b),
		-((t + b) / (t - b)),
		0.0f,
		0.0f,
		1.0f
	};
}
