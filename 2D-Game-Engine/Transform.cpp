#include "Transform.h"

#include "Matrix3f.h"

void Transform::translate(float32 dx, float32 dy) {
	xPos += dx;
	yPos += dy;
}

void Transform::scale(float32 scale) {
	xScale *= scale;
	yScale *= scale;
}

Matrix3f Transform::toMatrix() const {
	// TODO pre multiply???
	return Matrix3f::translation(xPos, yPos) * Matrix3f::scale(xScale, yScale) * Matrix3f::rotation(rot);
}
