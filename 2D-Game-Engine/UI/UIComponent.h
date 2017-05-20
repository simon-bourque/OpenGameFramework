#pragma once

#include "Math/Geometry/Rectangle.h"

class UIComponent {
protected:
	Rectangle m_bounds;

	bool m_visible;
public:
	UIComponent(const Rectangle& bounds);
	virtual ~UIComponent();

	bool isVisible() const { return m_visible; };
	void setVisible(bool visible) { m_visible = visible; };

	virtual void render() = 0;
};

