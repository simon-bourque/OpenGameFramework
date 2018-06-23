#include "Framebuffer.h"

Framebuffer::Framebuffer(const std::string& name)
	: _name(name)
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

void Framebuffer::unbind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}