#pragma once
#ifndef RENDERABLECOMPONENT_H
#define RENDERABLECOMPONENT_H

#include "Object/Component/ObjectComponent.h"

class RenderSystem;

class RenderableComponent : public ObjectComponent {
private:
	bool m_visible;
public:
	RenderableComponent(GameObject* parentObject);
	virtual ~RenderableComponent();

	bool isVisible() const { return m_visible; };
	void setVisible(bool visible) { m_visible = visible; };

	virtual void render(RenderSystem* rs) = 0;
};

#endif