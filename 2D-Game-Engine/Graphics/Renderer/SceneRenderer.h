#pragma once
#ifndef SCENERENDERER_H
#define SCENERENDERER_H

#include "Graphics/Renderer/Renderer.h"

#include "Core/Core.h"

#include <memory>

class RenderSystem;
class ShaderProgram;
class VertexArrayObject;
class Texture;
class Background;

class SceneRenderer : public Renderer {
private:
	std::unique_ptr<ShaderProgram> m_backgroundShaderProgram;
	std::unique_ptr<ShaderProgram> m_tileShaderProgram;

	std::unique_ptr<VertexArrayObject> m_backgroundVAO;
public:
	
	static const float32 BACKGROUND_VERTS[8];
	static const float32 BACKGROUND_UV[8];
	static const int32 BACKGROUND_NUM_VERTICES;

	SceneRenderer(RenderSystem* rs);
	virtual ~SceneRenderer();

	void renderBackground(const Background& bg) const;
	void renderTiles(const VertexArrayObject* tileVAO, const Texture* tileSheet, int32 numberOfTiles) const;
};

#endif