#include "SpriteAnimState.h"

#include "Object/Component/AnimatorComponent.h"
#include "Object/GameObject.h"
#include "Object/Transform.h"

#include "Graphics/RenderSystem.h"
#include "Graphics/Renderer/SpriteRenderer.h"

SpriteAnimState::SpriteAnimState(Texture* texture) : AnimState(texture) {}


SpriteAnimState::~SpriteAnimState() {}

// Do nothing
void SpriteAnimState::onEnter() {}
void SpriteAnimState::tick(float32 delta) {}

void SpriteAnimState::render(RenderSystem* rs, const AnimatorComponent& component) {
	rs->getSpriteRenderer()->renderSprite(&component.getParentObject()->getTransform(), m_texture, component.isHorizontallyFlipped(), component.isVerticallyFlipped());
}