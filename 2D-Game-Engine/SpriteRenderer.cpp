#include "SpriteRenderer.h"

#include "RenderSystem.h"
#include "ShaderProgram.h"
#include "Resources.h"
#include "VertexArrayObject.h"
#include "Buffer.h"
#include "Transform.h"
#include "Texture.h"

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
	m_spriteShaderProgram = new ShaderProgram(loadSrc("../res/shader/default_vert.shader"), loadSrc("../res/shader/sprite_frag.shader"));
	m_animSpriteShaderProgram = new ShaderProgram(loadSrc("../res/shader/default_vert.shader"), loadSrc("../res/shader/animsprite_frag.shader"));
	m_spriteBatchShaderProgram = new ShaderProgram(loadSrc("../res/shader/instance_vert.shader"), loadSrc("../res/shader/instance_frag.shader"));

	m_spriteVAO = new VertexArrayObject();
	m_spriteVAO->bind();
	m_spriteVAO->addArrayBuffer(0, (void*)SPRITE_VERTS, sizeof(float32) * 8, 2, GL_FLOAT, Buffer::Usage::STATIC_DRAW);
	m_spriteVAO->addArrayBuffer(1, (void*)SPRITE_UV, sizeof(float32) * 8, 2, GL_FLOAT, Buffer::Usage::STATIC_DRAW);
	m_spriteVAO->unbind();
}


SpriteRenderer::~SpriteRenderer() {
	delete m_spriteShaderProgram;
	delete m_animSpriteShaderProgram;
	delete m_spriteBatchShaderProgram;
	delete m_spriteVAO;
}

void SpriteRenderer::renderSprite(const Transform* transform, const Texture* texture, bool hFlip, bool vFlip) const {
	glUseProgram(m_spriteShaderProgram->getProgramId());

	m_spriteVAO->bind();

	texture->bind();

	Matrix3f finalMatrix = m_rs->getCamera().getViewProjectionMatrix() * transform->toMatrix();

	float32 matrixData[] = {
		finalMatrix.m00,
		finalMatrix.m10,
		finalMatrix.m20,
		finalMatrix.m01,
		finalMatrix.m11,
		finalMatrix.m21,
		finalMatrix.m02,
		finalMatrix.m12,
		finalMatrix.m22
	};

	glUniformMatrix3fv(m_spriteShaderProgram->getUniform("mvpMatrix").getLocation(), 1, false, matrixData);
	glUniform1i(m_spriteShaderProgram->getUniform("horizontalFlip").getLocation(), (hFlip) ? GL_TRUE : GL_FALSE);
	glUniform1i(m_spriteShaderProgram->getUniform("verticalFlip").getLocation(), (vFlip) ? GL_TRUE : GL_FALSE);
	glUniform1i(m_spriteShaderProgram->getUniform("sprite").getLocation(), 0);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, SPRITE_NUM_VERTICES);

}
