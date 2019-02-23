#include "Framebuffer.h"

Framebuffer::Framebuffer(const std::string& name)
	: _attachedTex(nullptr)
	, _name(name)
{
	glGenFramebuffers(1, &_fbid);
}

Framebuffer::~Framebuffer() {
	glDeleteRenderbuffers(1, &_renderbuffer.id);
	_renderbuffer.id = 0;
	glDeleteFramebuffers(1, &_fbid);
	_fbid = 0;

}

void Framebuffer::bind() const {
	glBindFramebuffer(GL_FRAMEBUFFER, _fbid);
}

void Framebuffer::bindAsRead() const {
	glBindFramebuffer(GL_READ_FRAMEBUFFER, _fbid);
}

void Framebuffer::bindAsDraw() const {
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _fbid);
}

void Framebuffer::attachTex2D(Texture& tex, Framebuffer::Attachment att) {
	glBindFramebuffer(GL_FRAMEBUFFER, _fbid);
	glFramebufferTexture2D(GL_FRAMEBUFFER, static_cast<GLenum>(att), GL_TEXTURE_2D, tex.getId(), 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	_attachedTex = &tex;
}

void Framebuffer::attachRenderBuffer(Texture::InternalFormat internalFormat, Framebuffer::Attachment att, const int32 width, const int32 height) {
	_renderbuffer.internalFormat = internalFormat;
	_renderbuffer.attachment = att;

	glGenRenderbuffers(1, &_renderbuffer.id);
	glBindRenderbuffer(GL_RENDERBUFFER, _renderbuffer.id);
	glRenderbufferStorage(GL_RENDERBUFFER, static_cast<GLenum>(_renderbuffer.internalFormat), width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, _fbid);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, static_cast<GLenum>(_renderbuffer.attachment), GL_RENDERBUFFER, _renderbuffer.id);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::unbind() const {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::resize(const int32 width, const int32 height) {
	// Resizing attached render buffer
	if (_renderbuffer.id) {
		glBindRenderbuffer(GL_RENDERBUFFER, _renderbuffer.id);
		glRenderbufferStorage(GL_RENDERBUFFER, static_cast<GLenum>(_renderbuffer.internalFormat), width, height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}

	// Resizing attached texture
	if (_attachedTex){
		_attachedTex->resize(width, height);
	}
}