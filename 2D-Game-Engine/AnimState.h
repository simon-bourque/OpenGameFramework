#pragma once
#ifndef ANIMSTATE_H
#define ANIMSTATE_H

#include "Core.h"

class Texture;
class RenderSystem;

class AnimState {
protected:
	Texture* m_texture;
public:
	AnimState(Texture* texture);
	virtual ~AnimState();

	virtual void onEnter() = 0;
	virtual void tick(float32 delta) = 0;
	virtual void render(RenderSystem* rs) = 0;
};

#endif