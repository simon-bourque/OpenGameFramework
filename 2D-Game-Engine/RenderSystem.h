#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "Core.h"
#include "Camera.h"

class SpriteRenderer;
class RawImage;
class TextureManager;

class RenderSystem
{
private:
	Camera m_camera;
	
	SpriteRenderer* m_spriteRenderer;

	TextureManager* m_textureManager;
public:
	explicit RenderSystem(const Camera& camera);
	~RenderSystem();

	Camera& getCamera() { return m_camera; };

	SpriteRenderer* getSpriteRenderer() const { return m_spriteRenderer; };
	TextureManager* getTextureManager() const { return m_textureManager; };
};

#endif