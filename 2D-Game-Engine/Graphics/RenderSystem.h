#pragma once
#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "Core/Core.h"

#include "Graphics/Camera.h"

#include <memory>

class SpriteRenderer;
class SceneRenderer;
class TextRenderer;
class ShapeRenderer;

class RawImage;

class TextureManager;
class FontManager;
class TextManager;

class RenderSystem
{
private:
	Camera m_camera;
	
	std::unique_ptr<SpriteRenderer> m_spriteRenderer;
	std::unique_ptr<SceneRenderer> m_sceneRenderer;
	std::unique_ptr<TextRenderer> m_textRenderer;
	std::unique_ptr<ShapeRenderer> m_shapeRenderer;

	std::unique_ptr<TextureManager> m_textureManager;
	std::unique_ptr<FontManager> m_fontManager;
	std::unique_ptr<TextManager> m_textManager;
public:
	explicit RenderSystem(const Camera& camera);
	~RenderSystem();

	Camera& getCamera() { return m_camera; };

	SpriteRenderer* getSpriteRenderer() const { return m_spriteRenderer.get(); };
	SceneRenderer* getSceneRenderer() const { return m_sceneRenderer.get(); };
	TextRenderer* getTextRenderer() const { return m_textRenderer.get(); };
	ShapeRenderer* getShapeRenderer() const { return m_shapeRenderer.get(); };

	TextureManager* getTextureManager() const { return m_textureManager.get(); };
	FontManager* getFontManager() const { return m_fontManager.get(); };
	TextManager* getTextManager() const { return m_textManager.get(); };
};

#endif