#include "SpriteComponent.h"

#include "Graphics/RenderSystem.h"
#include "Graphics/Renderer/SpriteRenderer.h"

#include "Object/GameObject.h"

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
	RenderSystem::get()->getSpriteRenderer()->renderSprite(&m_parentObject->transform, m_texture, m_horizontalFlip, m_verticalFlip);
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