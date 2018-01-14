#include "Vector3f.h"

#include <cmath>

const Vector3f Vector3f::X_AXIS = Vector3f(1, 0, 0);
const Vector3f Vector3f::Y_AXIS = Vector3f(0, 1, 0);
const Vector3f Vector3f::Z_AXIS = Vector3f(0, 0, 1);

Vector3f::Vector3f() : x(0), y(0), z(0) {}

Vector3f::Vector3f(float32 x, float32 y, float32 z) : x(x), y(y), z(z) {}

Vector3f::Vector3f(const float32(&values)[3]) : x(values[0]), y(values[1]), z(values[2]) {}

float32 Vector3f::dot(const Vector3f& v) const {
	return (x * v.x) + (y * v.y) + (z * v.z);
}

Vector3f Vector3f::cross(const Vector3f& v) const {
	return{
	(y * v.z) - (z * v.y),
	(z * v.x) - (x * v.z),
	(x * v.y) - (y * v.x)
	};
}

Vector3f Vector3f::normalize() const {
	float32 norm = length();
	return{ x / norm, y / norm, z/ norm };
}

Vector3f Vector3f::reverse() const {
	return{ -x, -y, -z};
}

float32 Vector3f::length() const {
	return sqrt((x*x) + (y*y) + (z*z));
}

float32 Vector3f::lengthSquared() const {
	return (x*x) + (y*y) + (z*z);
}

Vector3f Vector3f::operator+(const Vector3f& v) const {
	return{ x + v.x, y + v.y, z + v.z };
}

Vector3f Vector3f::operator+(float32 s) const {
	return{ x + s, y + s, z + s };
}

Vector3f Vector3f::operator-(const Vector3f& v) const {
	return{ x - v.x, y - v.y, z - v.z };
}

Vector3f Vector3f::operator-(float32 s) const {
	return{ x - s, y - s, z - s };
}

Vector3f Vector3f::operator*(float32 s) const {
	return{ x * s, y * s, z * s };
}

Vector3f Vector3f::operator/(float32 s) const {
	return{ x / s, y / s, z / s};
}

bool Vector3f::operator==(const Vector3f& v) const {
	// TODO actually implement a proper way to compare floating point numbers
	return abs(x - v.x) < 0.000005 && abs(y - v.y) < 0.000005 && abs(z - v.z) < 0.000005;
}