#include "SpriteComponent.h"

#include "2D/Graphics/Graphics2D.h"
#include "2D/Graphics/Renderer/SpriteRenderer.h"

#include "2D/Object/GameObject.h"

#include "2D/Event.h"

#include "Core/Graphics/GraphicsContext.h"

GET_COMPONENT_TYPE_DEFINITION(SpriteComponent)

SpriteComponent::SpriteComponent(GameObject* parentObject, TextureRef textureRef) : RenderableComponent(parentObject),
	m_textureRef(textureRef),
	m_horizontalFlip(false),
	m_verticalFlip(false) 
{}

SpriteComponent::~SpriteComponent() {}

void SpriteComponent::tick(float32 delta) {}

void SpriteComponent::render() {
	getGraphics2DInstance()->getSpriteRenderer().renderSprite(&m_parentObject->transform, m_textureRef, m_horizontalFlip, m_verticalFlip);
}

void SpriteComponent::receiveEvent(const Event& event) {
	switch (event.type) {
	case Event::Type::FLIP_SPRITE: {
		m_horizontalFlip = event.param.asBool;
		break;
	}
	default:
		break;
	}
}

void SpriteComponent::replaceTexture(std::string newTexture) {
	getGraphicsContextInstance()->getTextureCache()->destroyTexture(m_textureRef);
	m_textureRef = getGraphicsContextInstance()->getTextureCache()->loadTexture(newTexture);
}

#ifdef DEBUG_BUILD
void SpriteComponent::debugRender() {}
#endif