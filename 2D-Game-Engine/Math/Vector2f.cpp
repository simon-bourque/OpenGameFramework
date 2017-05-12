#include "Vector2f.h"

#include <cmath>

const Vector2f Vector2f::X_AXIS = Vector2f(1,0);
const Vector2f Vector2f::Y_AXIS = Vector2f(0, 1);

Vector2f::Vector2f() : x(0), y(0) {}

Vector2f::Vector2f(float32 x, float32 y) : x(x), y(y) {}

Vector2f::Vector2f(const float32(&values)[2]) : x(values[0]), y(values[1]) {}

float32 Vector2f::dot(const Vector2f& v) const {
	return (x * v.x) + (y * v.y);
}

Vector2f Vector2f::normalize() const {
	float32 norm = length();
	return {x / norm, y / norm};
}

Vector2f Vector2f::reverse() const {
	return{-x, -y};
}

float32 Vector2f::length() const {
	return sqrt((x*x) + (y*y));
}

float32 Vector2f::lengthSquared() const {
	return (x*x) + (y*y);
}

Vector2f Vector2f::operator+(const Vector2f& v) const {
	return { x + v.x, y + v.y };
}

Vector2f Vector2f::operator+(float32 s) const {
	return{ x + s, y + s };
}

Vector2f Vector2f::operator-(const Vector2f& v) const {
	return { x - v.x, y - v.y };
}

Vector2f Vector2f::operator-(float32 s) const {
	return{ x - s, y - s };
}

Vector2f Vector2f::operator*(float32 s) const {
	return { x * s, y * s };
}

Vector2f Vector2f::operator/(float32 s) const {
	return{ x / s, y / s };
}

bool Vector2f::operator==(const Vector2f& v) const {
	// TODO actually implement a proper way to compare floating point numbers
	return abs(x - v.x) < 0.000005 && abs(y - v.y) < 0.000005;
}
