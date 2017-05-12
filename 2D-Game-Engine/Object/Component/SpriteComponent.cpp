#include "SpriteComponent.h"

#include "Graphics/RenderSystem.h"
#include "Graphics/Renderer/SpriteRenderer.h"

#include "Object/GameObject.h"

#include "Core/Event.h"

SpriteComponent::SpriteComponent(GameObject* parentObject, Texture* texture) : RenderableComponent(parentObject),
	m_texture(texture),
	m_horizontalFlip(false),
	m_verticalFlip(false) 
{}


SpriteComponent::~SpriteComponent() {}

void SpriteComponent::tick(float32 delta, Game* game) {}

void SpriteComponent::render(RenderSystem* rs) {
	rs->getSpriteRenderer()->renderSprite(&m_parentObject->getTransform(), m_texture, m_horizontalFlip, m_verticalFlip);
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