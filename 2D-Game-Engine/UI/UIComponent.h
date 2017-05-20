#pragma once

#include "Math/Geometry/Rectangle.h"

class UIComponent {
protected:
	Rectangle m_bounds;
public:
	UIComponent(const Rectangle& bounds);
	virtual ~UIComponent();

	virtual void render() = 0;
};

