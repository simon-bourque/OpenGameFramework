#pragma once

#include "Core/Core.h"
#include "Core/Graphics/Texture.h"

#include <GL/glew.h>

#include <string>

class Framebuffer
{
	friend class FramebufferCache;
private:
	uint32 _fbid;
	Texture::Attachment _attachment;
	const Texture* _attachedTex;
	std::string _name;

public:
	Framebuffer(const std::string& name, Texture::Attachment att);

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
	Texture::Attachment getAttachment() const noexcept { return _attachment;  }
	std::string getName() const noexcept { return _name; }
	const Texture* getTex() const noexcept { return _attachedTex; }

	virtual ~Framebuffer();
}; 