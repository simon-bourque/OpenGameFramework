#include "Framebuffer.h"

Framebuffer::Framebuffer(const std::string& name, Texture::Attachment att)
	: _attachment(att)
	, _attachedTex(nullptr)
	, _name(name)
{
	glGenFramebuffers(1, &_fbid);
}

Framebuffer::~Framebuffer()
{
	glDeleteFramebuffers(1, &_fbid);
	_fbid = -1;
}

void Framebuffer::bind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, _fbid);
}

void Framebuffer::bindAsRead() const
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, _fbid);
}

void Framebuffer::bindAsDraw() const
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _fbid);
}

void Framebuffer::attachTex2D( const Texture& tex )
{
	glFramebufferTexture2D(GL_FRAMEBUFFER, static_cast<GLenum>(_attachment), GL_TEXTURE_2D, tex.getId(), 0);
	_attachedTex = &tex;
}

void Framebuffer::unbind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}