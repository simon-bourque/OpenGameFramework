#pragma once

#include "Core/Core.h"

#include "Object/Component/RenderableComponent.h"

#include "Object/Component/ComponentType.h"

class GameObject;
class Game;
class Texture;
class RenderSystem;

class SpriteComponent : public RenderableComponent {
private:
	Texture* m_texture;

	bool m_horizontalFlip;
	bool m_verticalFlip;
public:
	SpriteComponent(GameObject* parentObject, Texture* texture);
	virtual ~SpriteComponent();

	bool isHorizontallyFlipped() const { return m_horizontalFlip; };
	bool isVerticallyFlipped() const { return m_verticalFlip; };
	void setHorizontalFlip(bool horizontalFlip) { m_horizontalFlip = horizontalFlip; }
	void setVerticalFlip(bool verticalFlip) { m_verticalFlip = verticalFlip; }

	virtual void tick(float32 delta) override;
	virtual void render() override;

	virtual void receiveEvent(const Event& event) override;

#ifdef DEBUG_BUILD
	virtual void debugRender() override;
#endif

	GET_COMPONENT_TYPE_DECLERATION
};

REGISTER_COMPONENT_TYPE(SpriteComponent)