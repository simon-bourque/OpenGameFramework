#include "Rectangle.h"

Rectangle::Rectangle() : Rectangle(0.0f, 0.0f){}

Rectangle::Rectangle(float32 width, float32 height) : Rectangle(0.0f , 0.0f, width, height) {}

Rectangle::Rectangle(float32 x, float32 y, float32 width, float32 height) : Shape(x, y), m_width(width), m_height(height) {
	m_halfWidth = width / 2.0f;
	m_halfHeight = height / 2.0f;

	m_topLeft.x = x - m_halfWidth;
	m_topRight.x = x + m_halfWidth;
	m_bottomLeft.x = x - m_halfWidth;
	m_bottomRight.x = x + m_halfWidth;
	m_topLeft.y = y + m_halfHeight;
	m_topRight.y = y + m_halfHeight;
	m_bottomLeft.y = y - m_halfHeight;
	m_bottomRight.y = y - m_halfHeight;
}

Rectangle::~Rectangle() {}

void Rectangle::setWidth(float32 width) {
	m_width = width;
	m_halfWidth = width / 2.0f;
}

void Rectangle::setHeight(float32 height) {
	m_height = height;
	m_halfHeight = height / 2.0f;
}

void Rectangle::setX(float32 x) {
	Shape::setX(x);
	m_topLeft.x = x - m_halfWidth;
	m_topRight.x = x + m_halfWidth;
	m_bottomLeft.x = x - m_halfWidth;
	m_bottomRight.x = x + m_halfWidth;
}

void Rectangle::setY(float32 y) {
	Shape::setY(y);
	m_topLeft.y = y + m_halfHeight;
	m_topRight.y = y + m_halfHeight;
	m_bottomLeft.y = y - m_halfHeight;
	m_bottomRight.y = y - m_halfHeight;
}

bool Rectangle::intersects(const Rectangle& rect) const {
	return (
		m_topLeft.x < rect.m_topRight.x &&
		m_topRight.x > rect.m_topLeft.x &&
		m_topLeft.y > rect.m_bottomLeft.y &&
		m_bottomLeft.y < rect.m_topLeft.y
		);
}
