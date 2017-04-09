#include "Texture.h"

#include "RawImage.h"

Texture::Texture(Target target, Unit unit) : m_target(target), m_unit(unit) {
	GLuint name = 0;
	glGenTextures(1, &name);
	m_name = name;
}

Texture::~Texture() {
	glDeleteTextures(1, &m_name);
	m_name = 0;
}

void Texture::bind() const {
	glActiveTexture(static_cast<GLenum>(m_unit));
	glBindTexture(static_cast<GLenum>(m_target), m_name);
}

void Texture::unbind() const {
	glBindTexture(static_cast<GLenum>(m_target), 0);
}