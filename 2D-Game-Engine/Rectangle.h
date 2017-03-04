#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "Core.h"

#include "Shape.h"
#include "Point.h"

class Rectangle : public Shape
{
private:
	float32 m_width;
	float32 m_height;

	float32 m_halfWidth;
	float32 m_halfHeight;

	Point m_topLeft;
	Point m_topRight;
	Point m_bottomLeft;
	Point m_bottomRight;
public:
	Rectangle();
	Rectangle(float32 width, float32 height);
	Rectangle(float32 x, float32 y, float32 width, float32 height);
	~Rectangle();

	float32 getWidth() const { return m_width; }
	float32 getHeight() const { return m_height; }
	Point getTopLeft() const { return m_topLeft; }
	Point getTopRight() const { return m_topRight; }
	Point getBottomLeft() const { return m_bottomLeft; }
	Point getBottomRight() const { return m_bottomRight; }
	float32 getHalfWidth() const { return m_halfWidth; }
	float32 getHalfHeight() const { return m_halfHeight; }

	void setWidth(float32 width);
	void setHeight(float32 height);
	virtual void setX(float32 x) override;
	virtual void setY(float32 y) override;
};

#endif

