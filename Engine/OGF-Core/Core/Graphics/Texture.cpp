#include "Texture.h"

Texture::Texture(Target target) 
	: _target(target)
	, _texid(0)
{
	glGenTextures(1, &_texid);
}

Texture::~Texture() {
	glDeleteTextures(1, &_texid);
	_texid = 0;
}

void Texture::bind(Unit unit) const {
	glActiveTexture(static_cast<GLenum>(unit));
	glBindTexture(static_cast<GLenum>(_target), _texid);
}

void Texture::unbind() const {
	glBindTexture(static_cast<GLenum>(_target), 0);
}