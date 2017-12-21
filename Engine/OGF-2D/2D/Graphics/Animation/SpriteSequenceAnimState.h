#pragma once

#include "Core/Core.h"

#include "2D/Graphics/Animation/AnimState.h"
#include "2D/Graphics/Animation/Animation.h"

class SpriteSequenceAnimState : public AnimState {
private:
	Animation m_animation;
public:
	SpriteSequenceAnimState(Texture* texture, const Animation& animation);
	virtual ~SpriteSequenceAnimState();

	virtual void onEnter() override;
	virtual void tick(float32 delta) override;
	virtual void render(const AnimatorComponent& component) override;
};