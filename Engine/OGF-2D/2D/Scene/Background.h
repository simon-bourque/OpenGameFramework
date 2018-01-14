#pragma once

#include "Core/Core.h"

#include "Core/Graphics/TextureCache.h"

class Background {
private:
	TextureRef m_textureRef;
	
	float32 m_xOffset;
public:
	Background(TextureRef textureRef, float32 xOffset = 0.0f);
	virtual ~Background();

	TextureRef getTexture() const { return m_textureRef; };
	float32 getXOffset() const { return m_xOffset; };

	void setXOffset(float32 xOffset) { m_xOffset = xOffset; };

	void render();
};