#include "EmitterRenderer.h"

#include "Core/Graphics/Memory/VertexArrayObject.h"
#include "Core/Graphics/Shader/ShaderProgram.h"
#include "Core/Graphics/GraphicsContext.h"

#include "2D/Graphics/Graphics2D.h"

const float32 EmitterRenderer::QUAD_VERTS[8] = {
	-0.5f,-0.5f,
	-0.5f,0.5f,
	0.5f,-0.5f,
	0.5f,0.5f
};

const float32 EmitterRenderer::QUAD_UV[8] = {
	0.0f,1.0f,
	0.0f,0.0f,
	1.0f,1.0f,
	1.0f,0.0f
};

const int32 EmitterRenderer::QUAD_NUM_VERTICES = 4;

EmitterRenderer::EmitterRenderer() {
	m_particlesVAO.reset(new VertexArrayObject());
	m_particlesVAO->bind();
	m_particlesVAO->addArrayBuffer(0, QUAD_VERTS, sizeof(float32) * 8, 2, GL_FLOAT, Buffer::Usage::STATIC_DRAW);
	m_particlesVAO->addArrayBuffer(1, QUAD_UV, sizeof(float32) * 8, 2, GL_FLOAT, Buffer::Usage::STATIC_DRAW);
	m_particlesVAO->unbind();
}

EmitterRenderer::~EmitterRenderer() {}

void EmitterRenderer::renderParticles(const ShaderProgram* shader, TextureRef textureRef, const std::vector<Particle2D>& particles) {
	if (!shader)
	{
		return;
	}

	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glUseProgram(shader->getProgramId());

	m_particlesVAO->bind();
	Texture* texture = getGraphicsContextInstance()->getTextureCache()->getTexture(textureRef);
	texture->bind(Texture::Unit::UNIT_0);

	for (const auto& particle : particles) {
		glUniform2f(
			shader->getUniform("offset").getLocation(),
			particle.position.x,
			particle.position.y
		);
		glUniform4f(
			shader->getUniform("color").getLocation(),
			particle.color.r,
			particle.color.g,
			particle.color.b,
			particle.color.a
		);
		glUniformMatrix3fv(
			shader->getUniform("vpMatrix").getLocation(),
			1,
			true,
			getGraphics2DInstance()->getCamera().getViewProjectionMatrix().values
		);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, QUAD_NUM_VERTICES);
	}

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_particlesVAO->unbind();
	texture->unbind();
}