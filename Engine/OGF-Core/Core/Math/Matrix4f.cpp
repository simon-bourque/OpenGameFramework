#include "Matrix4f.h"

#include <math.h>

Matrix4f::Matrix4f() :
	m00(1), m01(0), m02(0),	m03(0),
	m10(0), m11(1), m12(0), m13(0),
	m20(0), m21(0), m22(1), m23(0),
	m30(0), m31(0), m32(0), m33(1)
{}

Matrix4f::Matrix4f(
	float32 m00, float32 m01, float32 m02, float32 m03,
	float32 m10, float32 m11, float32 m12, float32 m13,
	float32 m20, float32 m21, float32 m22, float32 m23,
	float32 m30, float32 m31, float32 m32, float32 m33
) :
	m00(m00), m01(m01), m02(m02), m03(m03),
	m10(m10), m11(m11), m12(m12), m13(m13),
	m20(m20), m21(m21), m22(m22), m23(m23),
	m30(m30), m31(m31), m32(m32), m33(m33)
{}

Matrix4f::Matrix4f(const float32(&values)[16]) :
	m00(values[0]), m01(values[1]), m02(values[2]), m03(values[3]),
	m10(values[4]), m11(values[5]), m12(values[6]), m13(values[7]),
	m20(values[8]),	m21(values[9]),	m22(values[10]),m23(values[11]),
	m30(values[12]),m31(values[13]),m32(values[14]),m33(values[15])
{}

Matrix4f Matrix4f::transpose() const {
	return{
		m00, m10, m20, m30,
		m01, m11, m21, m31,
		m02, m12, m22, m32,
		m03, m13, m23, m33
	};
}

float32 Matrix4f::det() const {
	float32 d1 = (m11 * m22 * m33) + (m12 * m23 * m31) + (m13 * m21 * m32) - (m11 * m23 * m32) - (m12 * m21 * m33) - (m13 * m22 * m31);
	float32 d2 = (m10 * m22 * m33) + (m12 * m23 * m30) + (m13 * m20 * m32) - (m13 * m22 * m30) - (m10 * m23 * m32) - (m12 * m20 * m33);
	float32 d3 = (m10 * m21 * m33) + (m11 * m23 * m30) + (m13 * m20 * m31) - (m13 * m21 * m30) - (m10 * m23 * m31) - (m11 * m20 * m33);
	float32 d4 = (m10 * m21 * m32) + (m11 * m22 * m30) + (m12 * m20 * m31) - (m12 * m21 * m30) - (m10 * m22 * m31) - (m11 * m20 * m32);

	return (m00 * d1) - (m01 * d2) + (m02 * d3) - (m03 * d4);
}

Matrix4f Matrix4f::inverse() const {

	float32 c1 = (m11 * m22 * m33) + (m12 * m23 * m31) + (m13 * m21 * m32) - (m11 * m23 * m32) - (m12 * m21 * m33) - (m13 * m22 * m31);
	float32 c2 = -1 * ((m10 * m22 * m33) + (m12 * m23 * m30) + (m13 * m20 * m32) - (m13 * m22 * m30) - (m10 * m23 * m32) - (m12 * m20 * m33));
	float32 c3 = (m10 * m21 * m33) + (m11 * m23 * m30) + (m13 * m20 * m31) - (m13 * m21 * m30) - (m10 * m23 * m31) - (m11 * m20 * m33);
	float32 c4 = -1 * ((m10 * m21 * m32) + (m11 * m22 * m30) + (m12 * m20 * m31) - (m12 * m21 * m30) - (m10 * m22 * m31) - (m11 * m20 * m32));
	float32 c5 = -1 * ((m01 * m22 * m33) + (m02 * m23 * m31) + (m03 * m21 * m32) - (m03 * m22 * m31) - (m01 * m23 * m32) - (m02 * m21 * m33));
	float32 c6 = (m00 * m22 * m33) + (m02 * m23 * m30) + (m03 * m20 * m32) - (m03 * m22 * m30) - (m00 * m23 * m32) - (m02 * m20 * m33);
	float32 c7 = -1 * ((m00 * m21 * m33) + (m01 * m23 * m30) + (m03 * m20 * m31) - (m03 * m21 * m30) - (m00 * m23 * m31) - (m01 * m20 * m33));
	float32 c8 = (m00 * m21 * m32) + (m01 * m22 * m30) + (m02 * m20 * m31) - (m02 * m21 * m30) - (m00 * m22 * m31) - (m01 * m20 * m32);
	float32 c9 = (m01 * m12 * m33) + (m02 * m13 * m31) + (m03 * m11 * m32) - (m03 * m12 * m31) - (m01 * m13 * m32) - (m02 * m11 * m33);
	float32 c10 = -1 * ((m00 * m12 * m33) + (m02 * m13 * m30) + (m03 * m10 * m32) - (m03 * m12 * m30) - (m00 * m13 * m32) - (m02 * m10 * m33));
	float32 c11 = (m00 * m11 * m33) + (m01 * m13 * m30) + (m03 * m10 * m31) - (m03 * m11 * m30) - (m00 * m13 * m31) - (m01 * m10 * m33);
	float32 c12 = -1 * ((m00 * m11 * m32) + (m01 * m12 * m30) + (m02 * m10 * m31) - (m02 * m11 * m30) - (m00 * m12 * m31) - (m01 * m10 * m32));
	float32 c13 = -1 * ((m01 * m12 * m23) + (m02 * m13 * m21) + (m03 * m11 * m22) - (m03 * m12 * m21) - (m01 * m13 * m22) - (m02 * m11 * m23));
	float32 c14 = (m00 * m12 * m23) + (m02 * m13 * m20) + (m03 * m10 * m22) - (m03 * m12 * m20) - (m00 * m13 * m22) - (m02 * m10 * m23);
	float32 c15 = -1 * ((m00 * m11 * m23) + (m01 * m13 * m20) + (m03 * m10 * m21) - (m03 * m11 * m20) - (m00 * m13 * m21) - (m01 * m10 * m23));
	float32 c16 = (m00 * m11 * m22) + (m01 * m12 * m20) + (m02 * m10 * m21) - (m02 * m11 * m20) - (m00 * m12 * m21) - (m01 * m10 * m22);

	float32 det = (m00 * c1) - (m01 * c2 * -1) + (m02 * c3) - (m03 * c4 * -1);

	// TODO CHECK IF INVERTIBLE

	return{
		c1 / det, c5 / det, c9 / det, c13 / det,
		c2 / det, c6 / det, c10 / det, c14 / det,
		c3 / det, c7 / det, c11 / det, c15 / det,
		c4 / det, c8 / det, c12 / det, c16 / det
	};
}

Matrix4f Matrix4f::operator+(const Matrix4f& m) const {
	return{
		m00 + m.m00, m01 + m.m01, m02 + m.m02, m03 + m.m03,
		m10 + m.m10, m11 + m.m11, m12 + m.m12, m13 + m.m13,
		m20 + m.m20, m21 + m.m21, m22 + m.m22, m23 + m.m23,
		m30 + m.m30, m31 + m.m31, m32 + m.m32, m33 + m.m33
	};
}

Matrix4f Matrix4f::operator-(const Matrix4f& m) const {
	return{
		m00 - m.m00, m01 - m.m01, m02 - m.m02, m03 - m.m03,
		m10 - m.m10, m11 - m.m11, m12 - m.m12, m13 - m.m13,
		m20 - m.m20, m21 - m.m21, m22 - m.m22, m23 - m.m23,
		m30 - m.m30, m31 - m.m31, m32 - m.m32, m33 - m.m33
	};
}

Matrix4f Matrix4f::operator*(const Matrix4f& m) const{
	return{
		m00 * m.m00 + m01 * m.m10 + m02 * m.m20 + m03 * m.m30,
		m00 * m.m01 + m01 * m.m11 + m02 * m.m21 + m03 * m.m31,
		m00 * m.m02 + m01 * m.m12 + m02 * m.m22 + m03 * m.m32,
		m00 * m.m03 + m01 * m.m13 + m02 * m.m23 + m03 * m.m33, // first row
		m10 * m.m00 + m11 * m.m10 + m12 * m.m20 + m13 * m.m30,
		m10 * m.m01 + m11 * m.m11 + m12 * m.m21 + m13 * m.m31,
		m10 * m.m02 + m11 * m.m12 + m12 * m.m22 + m13 * m.m32,
		m10 * m.m03 + m11 * m.m13 + m12 * m.m23 + m13 * m.m33, // second row
		m20 * m.m00 + m21 * m.m10 + m22 * m.m20 + m23 * m.m30,
		m20 * m.m01 + m21 * m.m11 + m22 * m.m21 + m23 * m.m31,
		m20 * m.m02 + m21 * m.m12 + m22 * m.m22 + m23 * m.m32,
		m20 * m.m03 + m21 * m.m13 + m22 * m.m23 + m23 * m.m33, // third row
		m30 * m.m00 + m31 * m.m10 + m32 * m.m20 + m33 * m.m30,
		m30 * m.m01 + m31 * m.m11 + m32 * m.m21 + m33 * m.m31,
		m30 * m.m02 + m31 * m.m12 + m32 * m.m22 + m33 * m.m32,
		m30 * m.m03 + m31 * m.m13 + m32 * m.m23 + m33 * m.m33 
	};
}

Matrix4f Matrix4f::operator*(float32 s) const {
	return{
		m00 * s, m01 * s, m02 * s, m03 * s,
		m10 * s, m11 * s, m12 * s, m13 * s,
		m20 * s, m21 * s, m22 * s, m23 * s,
		m30 * s, m31 * s, m32 * s, m33 * s
	};
}

Matrix4f Matrix4f::translation(float32 dx, float32 dy, float32 dz) {
	return{
		1.0f, 0.0f, 0.0f, dx,
		0.0f, 1.0f, 0.0f, dy,
		0.0f, 0.0f, 1.0f, dz, 
		0.0f, 0.0f, 0.0f, 1.0f
	};
}

Matrix4f Matrix4f::scale(float32 sx, float32 sy, float32 sz) {
	return{
		sx, 0.0f, 0.0f, 0.0f,
		0.0f, sy, 0.0f, 0.0f,
		0.0f, 0.0f, sz, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
}

Matrix4f Matrix4f::rotationX(float32 angle) {
	return{
		1, 0, 0, 0,
		0, cos(angle), (-sin(angle)), 0,
		0, sin(angle),  cos(angle), 0,
		0, 0, 0, 1
	};
}

Matrix4f Matrix4f::rotationY(float32 angle) {
	return{
		cos(angle), 0, -sin(angle), 0,
		0, 1, 0, 0,
		sin(angle), 0, cos(angle), 0,
		0, 0, 0, 1
	};
}

Matrix4f Matrix4f::rotationZ(float32 angle) {
	return{
		cos(angle), -sin(angle), 0, 0,
		sin(angle), cos(angle), 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};
}

Matrix4f Matrix4f::orthographic(float32 left, float32 right, float32 top, float32 bottom, float32 near, float32 far) {
	return{
		2.0f / (right - left), 0, 0, ((right + left) / (right - left)) * -1,
		0, 2.0f / (top - bottom), 0, ((top + bottom) / (top - bottom)) * -1,
		0, 0, -2.0f / (far - near), -1 * ((far + near) / (far - near)),
		0, 0, 0, 1
	};
}

Matrix4f Matrix4f::perspective(float32 fov, float32 aspectRatio, float32 near, float32 far) {
	float32 halfHeight = tan(fov / 2)*near;
	float32 top = halfHeight;
	float32 bottom = -halfHeight;

	float32 halfWidth = halfHeight * aspectRatio;
	float32 right = halfWidth;
	float32 left = -halfWidth;

	return{
		(2.0f * near) / (right - left), 0, (right + left) / (right - left), 0,
		0, (2.0f * near) / (top - bottom), (top + bottom) / (top - bottom), 0,
		0, 0, (-1 * (far + near)) / (far - near), (-2.0f * far * near) / (far - near),
		0, 0, -1, 0
	};
}
