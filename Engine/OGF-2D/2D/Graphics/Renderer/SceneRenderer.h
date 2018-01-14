#pragma once

#include "Core/Core.h"

#include "Core/Graphics/TextureCache.h"

#include <memory>

class ShaderProgram;
class VertexArrayObject;
class Texture;
class Background;

class SceneRenderer {
private:
	std::unique_ptr<ShaderProgram> m_backgroundShaderProgram;
	std::unique_ptr<ShaderProgram> m_tileShaderProgram;

	std::unique_ptr<VertexArrayObject> m_backgroundVAO;
public:
	
	static const float32 BACKGROUND_VERTS[8];
	static const float32 BACKGROUND_UV[8];
	static const int32 BACKGROUND_NUM_VERTICES;

	SceneRenderer();
	virtual ~SceneRenderer();

	void renderBackground(const Background& bg) const;
	void renderTiles(const VertexArrayObject* tileVAO, TextureRef tileSheet, int32 numberOfTiles) const;
};