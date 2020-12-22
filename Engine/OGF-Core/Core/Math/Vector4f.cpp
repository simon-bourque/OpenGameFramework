#include "Vector4f.h"

#include <cmath>

const Vector4f Vector4f::X_AXIS = Vector4f(1, 0, 0, 0);
const Vector4f Vector4f::Y_AXIS = Vector4f(0, 1, 0, 0);
const Vector4f Vector4f::Z_AXIS = Vector4f(0, 0, 1, 0);
const Vector4f Vector4f::W_AXIS = Vector4f(0, 0, 0, 1);

Vector4f::Vector4f() : x(0), y(0), z(0), w(0) {}

Vector4f::Vector4f(float32 x, float32 y, float32 z, float32 w) : x(x), y(y), z(z), w(w) {}

Vector4f::Vector4f(const float32(&values)[4]) : x(values[0]), y(values[1]), z(values[2]), w(values[3]) {}

float32 Vector4f::dot(const Vector4f& v) const {
	return (x * v.x) + (y * v.y) + (z * v.z) + (w * v.z);
}

Vector4f Vector4f::normalize() const {
	float32 norm = length();
	return{ x / norm, y / norm, z / norm, w / norm };
}

Vector4f Vector4f::reverse() const {
	return{ -x, -y, -z, -w };
}

float32 Vector4f::length() const {
	return sqrt((x*x) + (y*y) + (z*z) + (w*w));
}

float32 Vector4f::lengthSquared() const {
	return (x*x) + (y*y) + (z*z) + (w*w);
}

Vector4f Vector4f::operator+(const Vector4f& v) const {
	return{ x + v.x, y + v.y, z + v.z, w + v.w };
}

Vector4f Vector4f::operator+(float32 s) const {
	return{ x + s, y + s, z + s, w + s };
}

Vector4f Vector4f::operator-(const Vector4f& v) const {
	return{ x - v.x, y - v.y, z - v.z, w - v.w };
}

Vector4f Vector4f::operator-(float32 s) const {
	return{ x - s, y - s, z - s, w - s };
}

Vector4f Vector4f::operator*(float32 s) const {
	return{ x * s, y * s, z * s, w * s };
}

Vector4f Vector4f::operator/(float32 s) const {
	return{ x / s, y / s, z / s, w / s };
}

bool Vector4f::operator==(const Vector4f& v) const {
	// TODO actually implement a proper way to compare floating point numbers
	return abs(x - v.x) < 0.000005 && abs(y - v.y) < 0.000005 && abs(z - v.z) < 0.000005 && abs(w - v.w) < 0.000005;
}