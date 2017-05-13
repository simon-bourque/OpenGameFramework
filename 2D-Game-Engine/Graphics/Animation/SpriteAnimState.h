#pragma once
#ifndef SPRITEANIMSTATE_H
#define SPRITEANIMSTATE_H

#include "Core/Core.h"

#include "Graphics/Animation/AnimState.h"

class Texture;
class AnimatorComponent;

class SpriteAnimState : public AnimState {
public:
	SpriteAnimState(Texture* texture);
	virtual ~SpriteAnimState();

	virtual void onEnter() override;
	virtual void tick(float32 delta) override;
	virtual void render(const AnimatorComponent& component) override;
};

#endif