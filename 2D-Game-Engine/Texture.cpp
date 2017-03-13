#include "Texture.h"

#include "RawImage.h"

Texture::Texture(Target target, GLenum unit) : m_target(target), m_unit(unit) {
	GLuint name = 0;
	glGenTextures(1, &name);
	m_name = name;
}

Texture::~Texture() {
	glDeleteTextures(1, &m_name);
	m_name = 0;
}

void Texture::bind() const {
	glActiveTexture(m_unit);
	glBindTexture(m_target, m_name);
}

void Texture::unbind() const {
	glBindTexture(m_target, 0);
}