#include "WaterRenderer.h"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "3D/Graphics/Renderer/RenderingContext.h"
#include "3D/Graphics/Shader/ShaderProgramOld.h"
#include "Core/Graphics/Texture.h"
#include "Core/Graphics/GraphicsContext.h"

WaterRenderer* WaterRenderer::s_instance = nullptr;

WaterRenderer::WaterRenderer() : 
	m_y(100.0f),
	m_textureTileFactor(1.0f)
{
	m_waterShader = RenderingContext::get()->shaderCache.loadShaderProgram("water_shader", "Resources/Shaders/water_vert.glsl", "Resources/Shaders/water_frag.glsl");
	m_waterNormal = RenderingContext::get()->textureCache.loadTexture2D("water_normal", "Resources/Images/water_normal.png");
	m_waterNormal2 = RenderingContext::get()->textureCache.loadTexture2D("water_normal2", "Resources/Images/water_normal2.png");
	m_waterDuDv = RenderingContext::get()->textureCache.loadTexture2D("water_dudv", "Resources/Images/water_dudv.png");

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	const static uint32 NUM_VERTICES = 4;
	const static float32 vertices[NUM_VERTICES * 3] = {
		-0.5f,0.0f,0.5f,
		-0.5f,0.0f,-0.5f,
		0.5f,0.0f,-0.5f,
		0.5,0.0f,0.5f
	};

	const static uint32 NUM_UV = 8;
	const static float32 uvs[] = {
		0.0f,0.0f,
		0.0f,1.0f,
		1.0f,1.0f,
		1.0f,0.0f
	};

	const static uint32 NUM_INDICES = 6;
	const static uint32 indices[NUM_INDICES] = {
		0,2,1,
		0,3,2
	};

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float32) * NUM_VERTICES * 3, vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, nullptr);
	m_vbos.push_back(vbo);

	GLuint uvVbo;
	glGenBuffers(1, &uvVbo);
	glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float32) * NUM_UV, uvs, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, false, 0, nullptr);
	m_vbos.push_back(uvVbo);

	GLuint ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32) * NUM_INDICES, indices, GL_STATIC_DRAW);
	m_vbos.push_back(ebo);
	m_numElements = NUM_INDICES;

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}


WaterRenderer::~WaterRenderer() {
	glDeleteBuffers(m_vbos.size(), m_vbos.data());
	glDeleteVertexArrays(1, &m_vao);

	getGraphicsContextInstance()->getFramebufferCache()->destroyFramebuffer(m_refractionFBO);
	getGraphicsContextInstance()->getTextureCache()->destroyTexture(m_refractionColorTexture);
}

void WaterRenderer::buildFBO(uint32 width, uint32 height) {
	m_refractionColorTexture = getGraphicsContextInstance()->getTextureCache()->genTexture(
		"WaterRefractionTexture",
		width,
		height,
		Texture::Target::TEXTURE_2D,
		Texture::InternalFormat::RGB,
		Texture::Format::RGB,
		Texture::PixelDataType::FLOAT
	);

	m_refractionFBO = getGraphicsContextInstance()->getFramebufferCache()->genFramebuffer(
		"WaterRefractionFramebuffer"
	);

	Framebuffer* fb = getGraphicsContextInstance()->getFramebufferCache()->getFramebuffer(m_refractionFBO);
	if (fb) {
		fb->attachRenderBuffer(Texture::InternalFormat::DEPTH_COMP, Framebuffer::Attachment::DEPTH_ATTACH, width, height);
		Texture* tex = getGraphicsContextInstance()->getTextureCache()->getTexture(m_refractionColorTexture);
		if (tex) {
			fb->attachTex2D(*tex, Framebuffer::Attachment::COLOR_ATTACH);
		}
	}
}

void WaterRenderer::bindFBO() const {
	Framebuffer* fb = getGraphicsContextInstance()->getFramebufferCache()->getFramebuffer(m_refractionFBO);
	if (fb) {
		fb->bind();
	}
}

void WaterRenderer::prepare() {
	m_waterShader->use();
	glBindVertexArray(m_vao);
	m_waterNormal->bind(Texture::Unit::UNIT_0);
	m_waterNormal2->bind(Texture::Unit::UNIT_1);
	m_waterDuDv->bind(Texture::Unit::UNIT_2);

	Texture* tex = getGraphicsContextInstance()->getTextureCache()->getTexture(m_refractionColorTexture);
	if (tex) {
		tex->bind(Texture::Unit::UNIT_3);
	}
}

void WaterRenderer::render(float32 x, float32 z, float32 scale) {

	glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(x, m_y, z)) * glm::scale(glm::mat4(1.0f), glm::vec3(scale, 0.0f, scale));

	m_waterShader->setUniform("modelMatrix", modelMatrix);
	m_waterShader->setUniform("vpMatrix", RenderingContext::get()->camera.getViewProjectionMatrix());
	glDrawElements(GL_TRIANGLES, m_numElements, GL_UNSIGNED_INT, nullptr);
}

void WaterRenderer::setLightUniforms(const LightSource& light) {
	m_waterShader->use();
	m_waterShader->setUniform("lightDirection", light.getDirection());
	m_waterShader->setUniform("lightColor", light.getColor());
}
