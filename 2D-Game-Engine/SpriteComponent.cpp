#include "SpriteComponent.h"

#include "RenderSystem.h"
#include "SpriteRenderer.h"

#include "GameObject.h"

SpriteComponent::SpriteComponent(GameObject* parentObject, Texture* texture) : RenderableComponent(parentObject),
	m_texture(texture),
	m_horizontalFlip(false),
	m_verticalFlip(false) 
{}


SpriteComponent::~SpriteComponent() {}

void SpriteComponent::tick(float32 delta, Game* game) {}

void SpriteComponent::render(const RenderSystem& rs) {
	rs.getSpriteRenderer()->renderSprite(&m_parentObject->getTransform(), m_texture, m_horizontalFlip, m_verticalFlip);
}