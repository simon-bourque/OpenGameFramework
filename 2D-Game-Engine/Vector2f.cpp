#include "Vector2f.h"

#include <cmath>

Vector2f::Vector2f() {}

Vector2f::Vector2f(float32 x, float32 y) : x(x), y(y) {}

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
