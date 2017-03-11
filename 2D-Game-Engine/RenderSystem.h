#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "Core.h"
#include "Camera.h"

class SpriteRenderer;

class RenderSystem
{
private:
	Camera m_camera;
	
	SpriteRenderer* m_spriteRenderer;
public:
	RenderSystem(const Camera& camera);
	~RenderSystem();

	Camera& getCamera() { return m_camera; };

	SpriteRenderer* getSpriteRenderer() const { return m_spriteRenderer; };
};

#endif