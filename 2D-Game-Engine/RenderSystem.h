#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "Core.h"
#include "Camera.h"

#include <memory>

class SpriteRenderer;
class SceneRenderer;
class RawImage;
class TextureManager;

class RenderSystem
{
private:
	Camera m_camera;
	
	unique_ptr<SpriteRenderer> m_spriteRenderer;
	unique_ptr<SceneRenderer> m_sceneRenderer;

	unique_ptr<TextureManager> m_textureManager;
public:
	explicit RenderSystem(const Camera& camera);
	~RenderSystem();

	Camera& getCamera() { return m_camera; };

	SpriteRenderer* getSpriteRenderer() const { return m_spriteRenderer.get(); };
	SceneRenderer* getSceneRenderer() const { return m_sceneRenderer.get(); };
	TextureManager* getTextureManager() const { return m_textureManager.get(); };
};

#endif