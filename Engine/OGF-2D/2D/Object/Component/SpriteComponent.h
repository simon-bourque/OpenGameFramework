#pragma once

#include "Core/Core.h"
#include "Core/Graphics/TextureCache.h"

#include "2D/Object/Component/RenderableComponent.h"

#include "2D/Object/Component/ComponentType.h"

#include <string>

class GameObject;
class Game;
class RenderSystem;

class SpriteComponent : public RenderableComponent {
private:
	TextureRef m_textureRef;

	bool m_horizontalFlip;
	bool m_verticalFlip;
public:
	SpriteComponent(GameObject* parentObject, TextureRef textureRef);
	virtual ~SpriteComponent();

	bool isHorizontallyFlipped() const { return m_horizontalFlip; };
	bool isVerticallyFlipped() const { return m_verticalFlip; };
	void setHorizontalFlip(bool horizontalFlip) { m_horizontalFlip = horizontalFlip; }
	void setVerticalFlip(bool verticalFlip) { m_verticalFlip = verticalFlip; }

	void replaceTexture(std::string newTexture);

	virtual void tick(float32 delta) override;
	virtual void render() override;

	virtual void receiveEvent(const Event& event) override;

#ifdef DEBUG_BUILD
	virtual void debugRender() override;
#endif

	GET_COMPONENT_TYPE_DECLERATION
};

REGISTER_COMPONENT_TYPE(SpriteComponent)