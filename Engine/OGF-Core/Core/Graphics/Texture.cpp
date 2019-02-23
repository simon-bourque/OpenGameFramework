#include "Texture.h"

Texture::Texture(Target target, InternalFormat internalFormat, Format format, PixelDataType type) 
	: _target(target)
	, _internalFormat(internalFormat)
	, _format(format)
	, _type(type)
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

void Texture::resize(const int32 width, const int32 height) {
	glBindTexture(static_cast<GLenum>(_target), _texid);

	glTexImage2D(
		static_cast<GLenum>(_target),
		0,
		static_cast<GLenum>(_internalFormat),
		width,
		height,
		0,
		static_cast<GLenum>(_format),
		static_cast<GLenum>(_type),
		0);

	glBindTexture(static_cast<GLenum>(_target), 0);
}