#include "SceneRenderer.h"

#include "Graphics/Shader/ShaderProgram.h"
#include "Graphics/Memory/VertexArrayObject.h"
#include "Graphics/Texture.h"
#include "Graphics/RenderSystem.h"

#include "Resource/Resources.h"

#include <GL/glew.h>

const float32 SceneRenderer::BACKGROUND_VERTS[8] = {
	-1.0f,-1.0f,
	-1.0f,1.0f,
	1.0f,-1.0f,
	1.0f,1.0f
};
const float32 SceneRenderer::BACKGROUND_UV[8] = {
	0.0f,1.0f,
	0.0f,0.0f,
	1.0f,1.0f,
	1.0f,0.0f
};

const int32 SceneRenderer::BACKGROUND_NUM_VERTICES = 4;

SceneRenderer::SceneRenderer(RenderSystem* rs) : Renderer(rs) {
	m_backgroundShaderProgram.reset(new ShaderProgram(loadSrc("bg_vert.shader"), loadSrc("bg_frag.shader")));
	m_tileShaderProgram.reset(new ShaderProgram(loadSrc("tile_vert.shader"), loadSrc("tile_frag.shader")));

	m_backgroundVAO.reset(new VertexArrayObject());
	m_backgroundVAO->bind();
	m_backgroundVAO->addArrayBuffer(0, BACKGROUND_VERTS, sizeof(float32) * 8, 2, GL_FLOAT, Buffer::Usage::STATIC_DRAW);
	m_backgroundVAO->addArrayBuffer(1, BACKGROUND_UV, sizeof(float32) * 8, 2, GL_FLOAT, Buffer::Usage::STATIC_DRAW);
	m_backgroundVAO->unbind();
}


SceneRenderer::~SceneRenderer() {}

void SceneRenderer::renderTiles(const VertexArrayObject* tileVAO, const Texture* tileSheet, int32 numberOfTiles) const {
	glUseProgram(m_tileShaderProgram->getProgramId());

	tileVAO->bind();
	tileSheet->bind();

	glUniform1i(m_tileShaderProgram->getUniform("tilesheet").getLocation(), 0);
	glUniformMatrix3fv(m_tileShaderProgram->getUniform("vpMatrix").getLocation(), 1, true, m_rs->getCamera().getViewProjectionMatrix().values);

	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, numberOfTiles); // TODO fix using magic number 4
}
