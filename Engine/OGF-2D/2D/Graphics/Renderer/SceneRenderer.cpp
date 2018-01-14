#include "SceneRenderer.h"

#include "Core/Graphics/Shader/ShaderProgram.h"
#include "Core/Graphics/Memory/VertexArrayObject.h"
#include "Core/Graphics/GraphicsContext.h"
#include "Core/Graphics/Texture.h"

#include "2D/Graphics/Graphics2D.h"

#include "2D/Scene/Background.h"

#include "Core/Resource/Resources.h"

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

SceneRenderer::SceneRenderer() {
	m_backgroundShaderProgram.reset(new ShaderProgram(loadSrc("bg_vert.shader"), loadSrc("bg_frag.shader")));
	m_tileShaderProgram.reset(new ShaderProgram(loadSrc("tile_vert.shader"), loadSrc("tile_frag.shader")));

	m_backgroundVAO.reset(new VertexArrayObject());
	m_backgroundVAO->bind();
	m_backgroundVAO->addArrayBuffer(0, BACKGROUND_VERTS, sizeof(float32) * 8, 2, GL_FLOAT, Buffer::Usage::STATIC_DRAW);
	m_backgroundVAO->addArrayBuffer(1, BACKGROUND_UV, sizeof(float32) * 8, 2, GL_FLOAT, Buffer::Usage::STATIC_DRAW);
	m_backgroundVAO->unbind();
}


SceneRenderer::~SceneRenderer() {}

void SceneRenderer::renderBackground(const Background& bg) const {
	glUseProgram(m_backgroundShaderProgram->getProgramId());

	m_backgroundVAO->bind();

	getGraphicsContextInstance()->getTextureCache()->getTexture(bg.getTexture())->bind(Texture::Unit::UNIT_0);

	glUniform1i(m_backgroundShaderProgram->getUniform("diffuseTexture").getLocation(), 0);
	glUniform1f(m_backgroundShaderProgram->getUniform("xOffset").getLocation(), bg.getXOffset());

	glDrawArrays(GL_TRIANGLE_STRIP, 0, BACKGROUND_NUM_VERTICES);
}

void SceneRenderer::renderTiles(const VertexArrayObject* tileVAO, TextureRef tileSheet, int32 numberOfTiles) const {
	glUseProgram(m_tileShaderProgram->getProgramId());

	tileVAO->bind();

	getGraphicsContextInstance()->getTextureCache()->getTexture(tileSheet)->bind(Texture::Unit::UNIT_0);

	glUniform1i(m_tileShaderProgram->getUniform("tilesheet").getLocation(), 0);
	glUniformMatrix3fv(m_tileShaderProgram->getUniform("vpMatrix").getLocation(), 1, true, getGraphics2DInstance()->getCamera().getViewProjectionMatrix().values);

	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, numberOfTiles); // TODO fix using magic number 4
}
