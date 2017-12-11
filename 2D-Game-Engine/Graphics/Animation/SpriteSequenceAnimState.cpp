#include "SpriteSequenceAnimState.h"

#include "Object/Component/AnimatorComponent.h"
#include "Object/GameObject.h"
#include "Object/Transform.h"

#include "Graphics/RenderSystem.h"
#include "Graphics/Renderer/SpriteRenderer.h"

SpriteSequenceAnimState::SpriteSequenceAnimState(Texture* texture, const Animation& animation) : AnimState(texture), m_animation(animation) {}

SpriteSequenceAnimState::~SpriteSequenceAnimState() {}

void SpriteSequenceAnimState::onEnter() {
	m_animation.reset();
}

void SpriteSequenceAnimState::tick(float32 delta) {
	m_animation.tick(delta);
}

void SpriteSequenceAnimState::render(const AnimatorComponent& component) {
	getRenderSystemInstance()->getSpriteRenderer()->renderAnimationFrame(component.getParentObject()->transform, m_animation.getCurrentFrame(), *m_texture, component.isHorizontallyFlipped(), component.isVerticallyFlipped());
}