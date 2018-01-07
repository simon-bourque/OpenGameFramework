#include "SpriteAnimState.h"

#include "2D/Object/Component/AnimatorComponent.h"
#include "2D/Object/GameObject.h"
#include "2D/Object/Transform.h"

#include "2D/Graphics/Graphics2D.h"
#include "2D/Graphics/Renderer/SpriteRenderer.h"

SpriteAnimState::SpriteAnimState(TextureRef textureRef) : AnimState(textureRef) {}


SpriteAnimState::~SpriteAnimState() {}

// Do nothing
void SpriteAnimState::onEnter() {}
void SpriteAnimState::tick(float32 delta) {}

void SpriteAnimState::render(const AnimatorComponent& component) {
	getGraphics2DInstance()->getSpriteRenderer().renderSprite(&component.getParentObject()->transform, m_textureRef, component.isHorizontallyFlipped(), component.isVerticallyFlipped());
}