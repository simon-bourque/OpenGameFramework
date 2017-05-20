#include "SpriteRenderer.h"

#include "Graphics/RenderSystem.h"
#include "Graphics/Texture.h"
#include "Graphics/Shader/ShaderProgram.h"
#include "Graphics/Memory/VertexArrayObject.h"
#include "Graphics/Memory/Buffer.h"

#include "Math/Geometry/Rectangle.h"

#include "Object/Transform.h"

#include "Resource/Resources.h"

#include <GL/glew.h>

const float32 SpriteRenderer::SPRITE_VERTS[8] = {
	-0.5f,-0.5f,
	-0.5f,0.5f,
	0.5f,-0.5f,
	0.5f,0.5f
};

const float32 SpriteRenderer::SPRITE_UV[8] = { 
	0.0f,1.0f,
	0.0f,0.0f,
	1.0f,1.0f,
	1.0f,0.0f
};

const int32 SpriteRenderer::SPRITE_NUM_VERTICES = 4;

SpriteRenderer::SpriteRenderer(RenderSystem* rs) : Renderer(rs) {
	m_spriteShaderProgram.reset(new ShaderProgram(loadSrc("default_vert.shader"), loadSrc("sprite_frag.shader")));
	m_animSpriteShaderProgram.reset(new ShaderProgram(loadSrc("default_vert.shader"), loadSrc("animsprite_frag.shader")));
	m_spriteBatchShaderProgram.reset(new ShaderProgram(loadSrc("instance_vert.shader"), loadSrc("instance_frag.shader")));

	m_spriteVAO.reset(new VertexArrayObject());
	m_spriteVAO->bind();
	m_spriteVAO->addArrayBuffer(0, SPRITE_VERTS, sizeof(float32) * 8, 2, GL_FLOAT, Buffer::Usage::STATIC_DRAW);
	m_spriteVAO->addArrayBuffer(1, SPRITE_UV, sizeof(float32) * 8, 2, GL_FLOAT, Buffer::Usage::STATIC_DRAW);
	m_spriteVAO->unbind();
}


SpriteRenderer::~SpriteRenderer() {}

void SpriteRenderer::renderSprite(const Transform* transform, const Texture* texture, bool hFlip, bool vFlip) const {
	glUseProgram(m_spriteShaderProgram->getProgramId());

	m_spriteVAO->bind();

	texture->bind(Texture::Unit::UNIT_0);

	Matrix3f finalMatrix = m_rs->getCamera().getViewProjectionMatrix() * transform->toMatrix();

	glUniformMatrix3fv(m_spriteShaderProgram->getUniform("mvpMatrix").getLocation(), 1, true, finalMatrix.values);
	glUniform1i(m_spriteShaderProgram->getUniform("horizontalFlip").getLocation(), (hFlip) ? GL_TRUE : GL_FALSE);
	glUniform1i(m_spriteShaderProgram->getUniform("verticalFlip").getLocation(), (vFlip) ? GL_TRUE : GL_FALSE);
	glUniform1i(m_spriteShaderProgram->getUniform("sprite").getLocation(), 0);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, SPRITE_NUM_VERTICES);

}

void SpriteRenderer::renderSpriteUI(const Rectangle& bounds, const Texture* texture, bool hFlip, bool vFlip) const {
	glUseProgram(m_spriteShaderProgram->getProgramId());

	m_spriteVAO->bind();

	texture->bind(Texture::Unit::UNIT_0);

	Matrix3f finalMatrix = Matrix3f::translation(bounds.getX(), bounds.getY()) * Matrix3f::scale(bounds.getWidth(), bounds.getHeight());
	//Matrix3f finalMatrix = m_rs->getCamera().getViewProjectionMatrix() * transform->toMatrix();

	glUniformMatrix3fv(m_spriteShaderProgram->getUniform("mvpMatrix").getLocation(), 1, true, finalMatrix.values);
	glUniform1i(m_spriteShaderProgram->getUniform("horizontalFlip").getLocation(), (hFlip) ? GL_TRUE : GL_FALSE);
	glUniform1i(m_spriteShaderProgram->getUniform("verticalFlip").getLocation(), (vFlip) ? GL_TRUE : GL_FALSE);
	glUniform1i(m_spriteShaderProgram->getUniform("sprite").getLocation(), 0);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, SPRITE_NUM_VERTICES);

}

void SpriteRenderer::renderAnimationFrame(const Transform& transform, uint32 frame, const Texture& texture, bool hFlip, bool vFlip) const {
	glUseProgram(m_animSpriteShaderProgram->getProgramId());

	m_spriteVAO->bind();

	Matrix3f finalMatrix = m_rs->getCamera().getViewProjectionMatrix() * transform.toMatrix();

	// Diffuse Texture
	texture.bind(Texture::Unit::UNIT_0);

	glUniformMatrix3fv(m_animSpriteShaderProgram->getUniform("mvpMatrix").getLocation(), 1, true, finalMatrix.values);
	glUniform1i(m_animSpriteShaderProgram->getUniform("diffuseTextureAtlas").getLocation(), 0);
	glUniform1i(m_animSpriteShaderProgram->getUniform("currentFrame").getLocation(), frame);
	glUniform1i(m_animSpriteShaderProgram->getUniform("horizontalFlip").getLocation(), (hFlip) ? GL_TRUE : GL_FALSE);
	glUniform1i(m_animSpriteShaderProgram->getUniform("verticalFlip").getLocation(), (vFlip) ? GL_TRUE : GL_FALSE);


	glDrawArrays(GL_TRIANGLE_STRIP, 0, SPRITE_NUM_VERTICES);
}
