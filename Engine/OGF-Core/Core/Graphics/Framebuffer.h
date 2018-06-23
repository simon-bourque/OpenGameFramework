#pragma once

#include "Core/Core.h"

#include <GL/glew.h>

#include <string>

class Framebuffer
{
	friend class FramebufferCache;
private:
	uint32 _fbid;
	std::string _name;

public:
	Framebuffer(const std::string& name);

	// Prevent copying of framebuffers
	Framebuffer(const Framebuffer&) = delete;
	Framebuffer(Framebuffer&&) = default;
	Framebuffer& operator=(const Framebuffer&) & = delete;
	Framebuffer& operator=(Framebuffer&&) & = default;

	void bind() const;
	void unbind() const;

	uint32 getId() const noexcept { return _fbid; }
	std::string getName() const noexcept { return _name; }

	virtual ~Framebuffer() {  };
};