#pragma once

#include "UIComponent.h"
#include "Math/Geometry/Rectangle.h"

class Texture;

class UIImage : public UIComponent {
private:
	Texture* m_texture;
public:
	UIImage(const Rectangle& bounds, Texture* texture);
	virtual ~UIImage();

	void render() override;
};

