#ifndef SCENERENDERER_H
#define SCENERENDERER_H

#include "Renderer.h"

#include "Core.h"

#include <memory>

class RenderSystem;
class ShaderProgram;
class VertexArrayObject;
class Texture;

class SceneRenderer : public Renderer {
private:
	unique_ptr<ShaderProgram> m_backgroundShaderProgram;
	unique_ptr<ShaderProgram> m_tileShaderProgram;

	unique_ptr<VertexArrayObject> m_backgroundVAO;
public:
	
	static const float32 BACKGROUND_VERTS[8];
	static const float32 BACKGROUND_UV[8];
	static const int32 BACKGROUND_NUM_VERTICES;

	SceneRenderer(RenderSystem* rs);
	virtual ~SceneRenderer();

	void renderTiles(const VertexArrayObject* tileVAO, const Texture* tileSheet, int32 numberOfTiles) const;
};

#endif