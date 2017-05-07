#pragma once
#ifndef SPRITESEQUENCEANIMSTATE_H
#define SPRITESEQUENCEANIMSTATE_H

#include "Core.h"
#include "AnimState.h"
#include "Animation.h"

class SpriteSequenceAnimState : public AnimState {
private:
	Animation m_animation;
public:
	SpriteSequenceAnimState(Texture* texture, const Animation& animation);
	virtual ~SpriteSequenceAnimState();

	virtual void onEnter() override;
	virtual void tick(float32 delta) override;
	virtual void render(RenderSystem* rs, const AnimatorComponent& component) override;
};

#endif