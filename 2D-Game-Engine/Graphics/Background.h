#pragma once
#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "Core/Core.h"

class Texture;

class Background {
private:
	Texture* m_texture;
	
	float32 m_xOffset;
public:
	Background(Texture* texture, float32 xOffset = 0.0f);
	virtual ~Background();

	const Texture* getTexture() const { return m_texture; };
	float32 getXOffset() const { return m_xOffset; };

	void setXOffset(float32 xOffset) { m_xOffset = xOffset; };

	void render();
};

#endif