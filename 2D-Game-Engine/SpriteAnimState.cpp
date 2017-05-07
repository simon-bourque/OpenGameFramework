#include "SpriteAnimState.h"

#include "AnimatorComponent.h"
#include "GameObject.h"
#include "Transform.h"

#include "RenderSystem.h"
#include "SpriteRenderer.h"

SpriteAnimState::SpriteAnimState(Texture* texture) : AnimState(texture) {}


SpriteAnimState::~SpriteAnimState() {}

// Do nothing
void SpriteAnimState::onEnter() {}
void SpriteAnimState::tick(float32 delta) {}

void SpriteAnimState::render(RenderSystem* rs, const AnimatorComponent& component) {
	rs->getSpriteRenderer()->renderSprite(&component.getParentObject().getTransform(), m_texture, component.isHorizontallyFlipped(), component.isVerticallyFlipped());
}