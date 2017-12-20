#pragma once

#include "2D/Object/Component/ObjectComponent.h"

class RenderableComponent : public ObjectComponent {
private:
	bool m_visible;
public:
	RenderableComponent(GameObject* parentObject);
	virtual ~RenderableComponent();

	bool isVisible() const { return m_visible; };
	void setVisible(bool visible) { m_visible = visible; };

	virtual void render() = 0;
};