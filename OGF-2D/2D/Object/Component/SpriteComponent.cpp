#include "SpriteComponent.h"

#include "2D/Graphics/Graphics2D.h"
#include "2D/Graphics/Renderer/SpriteRenderer.h"

#include "2D/Object/GameObject.h"

#include "Core/Event.h"

GET_COMPONENT_TYPE_DEFINITION(SpriteComponent)

SpriteComponent::SpriteComponent(GameObject* parentObject, Texture* texture) : RenderableComponent(parentObject),
	m_texture(texture),
	m_horizontalFlip(false),
	m_verticalFlip(false) 
{}


SpriteComponent::~SpriteComponent() {}

void SpriteComponent::tick(float32 delta) {}

void SpriteComponent::render() {
	getGraphics2DInstance()->getSpriteRenderer().renderSprite(&m_parentObject->transform, m_texture, m_horizontalFlip, m_verticalFlip);
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

#ifdef DEBUG_BUILD
void SpriteComponent::debugRender() {}
#endif