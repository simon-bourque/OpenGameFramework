#pragma once

#include "Core/Core.h"
#include "Core/Graphics/TextureCache.h"

class AnimatorComponent;

class AnimState {
protected:
	TextureRef m_textureRef;
public:
	AnimState(TextureRef textureRef);
	virtual ~AnimState();

	virtual void onEnter() = 0;
	virtual void tick(float32 delta) = 0;
	virtual void render(const AnimatorComponent& component) = 0;
};