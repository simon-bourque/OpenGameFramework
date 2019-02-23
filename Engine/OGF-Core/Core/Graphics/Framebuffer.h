#pragma once

#include "Core/Core.h"
#include "Core/Graphics/Texture.h"

#include <GL/glew.h>

#include <string>

class Framebuffer
{
	friend class FramebufferCache;

public:

	enum class Attachment : GLenum {
		COLOR_ATTACH = GL_COLOR_ATTACHMENT0,
		DEPTH_ATTACH = GL_DEPTH_ATTACHMENT,
		STENCIL_ATTACH = GL_STENCIL_ATTACHMENT
	};

	struct RenderBuffer {
		uint32 id;
		Texture::InternalFormat internalFormat;
		Framebuffer::Attachment attachment;
	};

private:
	uint32 _fbid;
	RenderBuffer _renderbuffer;
	Texture* _attachedTex;
	std::string _name;

public:
	Framebuffer(const std::string& name);

	// Prevent copying of framebuffers
	Framebuffer(const Framebuffer&) = delete;
	Framebuffer(Framebuffer&&) = default;
	Framebuffer& operator=(const Framebuffer&) & = delete;
	Framebuffer& operator=(Framebuffer&&) & = default;

	void bind() const;
	void bindAsRead() const;
	void bindAsDraw() const;

	void attachTex2D(Texture& tex, Framebuffer::Attachment attachment);
	void attachRenderBuffer(Texture::InternalFormat internalFormat, Framebuffer::Attachment attachment, const int32 width, const int32 height);

	void unbind() const;

	uint32 getId() const noexcept { return _fbid; }
	std::string getName() const noexcept { return _name; }
	const Texture* getTex() const noexcept { return _attachedTex; }
	void resize(const int32 width, const int32 height); // Will resize the texture attached to this framebuffer

	virtual ~Framebuffer();
}; 