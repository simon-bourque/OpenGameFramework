#include "Texture.h"

Texture::Texture(Target target) : m_target(target) {
	GLuint name = 0;
	glGenTextures(1, &name);
	m_name = name;
}

Texture::~Texture() {
	glDeleteTextures(1, &m_name);
	m_name = 0;
}

void Texture::bind(Unit unit) const {
	glActiveTexture(static_cast<GLenum>(unit));
	glBindTexture(static_cast<GLenum>(m_target), m_name);
}

void Texture::unbind() const {
	glBindTexture(static_cast<GLenum>(m_target), 0);
}