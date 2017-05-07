#include "Transform.h"

#include "Math/Matrix3f.h"
#include "Math/Vector2f.h"

Transform::Transform() : xPos(0), yPos(0), rotation(0), xScale(1), yScale(1) {}

Transform::Transform(float32 xPos, float32 yPos, float32 rotation, float32 xScale, float32 yScale) : 
	xPos(xPos), yPos(yPos), rotation(rotation), xScale(xScale), yScale(yScale) {}

void Transform::translate(float32 dx, float32 dy) {
	xPos += dx;
	yPos += dy;
}

void Transform::translate(const Vector2f& displacement) {
	xPos += displacement.x;
	yPos += displacement.y;
}

void Transform::rotate(float32 angle) {
	rotation += angle;
}

void Transform::scale(float32 scale) {
	xScale *= scale;
	yScale *= scale;
}

Matrix3f Transform::toMatrix() const {
	// TODO pre multiply???
	return Matrix3f::translation(xPos, yPos) * Matrix3f::scale(xScale, yScale) * Matrix3f::rotation(rotation);
}
