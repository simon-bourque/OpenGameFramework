#include "SpriteSequenceAnimState.h"

#include "2D/Object/Component/AnimatorComponent.h"
#include "2D/Object/GameObject.h"
#include "2D/Object/Transform.h"

#include "2D/Graphics/Graphics2D.h"
#include "2D/Graphics/Renderer/SpriteRenderer.h"

SpriteSequenceAnimState::SpriteSequenceAnimState(TextureRef textureRef, const Animation& animation) : AnimState(textureRef), m_animation(animation) {}

SpriteSequenceAnimState::~SpriteSequenceAnimState() {}

void SpriteSequenceAnimState::onEnter() {
	m_animation.reset();
}

void SpriteSequenceAnimState::tick(float32 delta) {
	m_animation.tick(delta);
}

void SpriteSequenceAnimState::render(const AnimatorComponent& component) {
	getGraphics2DInstance()->getSpriteRenderer().renderAnimationFrame(component.getParentObject()->transform, m_animation.getCurrentFrame(), m_textureRef, component.isHorizontallyFlipped(), component.isVerticallyFlipped());
}