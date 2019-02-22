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

private:
	uint32 _fbid;
	Framebuffer::Attachment _attachment;
	const Texture* _attachedTex;
	std::string _name;

public:
	Framebuffer(const std::string& name, Framebuffer::Attachment att);

	// Prevent copying of framebuffers
	Framebuffer(const Framebuffer&) = delete;
	Framebuffer(Framebuffer&&) = default;
	Framebuffer& operator=(const Framebuffer&) & = delete;
	Framebuffer& operator=(Framebuffer&&) & = default;

	void bind() const;
	void bindAsRead() const;
	void bindAsDraw() const;

	void attachTex2D(const Texture& tex);

	void unbind() const;

	uint32 getId() const noexcept { return _fbid; }
	Framebuffer::Attachment getAttachment() const noexcept { return _attachment;  }
	std::string getName() const noexcept { return _name; }
	const Texture* getTex() const noexcept { return _attachedTex; }

	virtual ~Framebuffer();
}; 