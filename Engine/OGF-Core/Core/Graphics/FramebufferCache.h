#pragma once

#include "Core/Core.h"

#include "Core/Graphics/Framebuffer.h"

#include <unordered_map>

typedef uint64 FramebufRef;

class FramebufferCache
{
private:
	class FramebufRefHash
	{
	public:
		size_t operator()(const FramebufRef& k) const { return k; }
	};

	std::unordered_map<FramebufRef, Framebuffer*, FramebufRefHash> _loadedFbs;

	// Current number of framebuffers in the cache
	uint32 _size;

	// The regenerated framebuffers will use these dimensions
	int32 _width;
	int32 _height;

public:
	FramebufferCache();
	~FramebufferCache();

	FramebufRef genFramebuffer(const std::string& name);
	void destroyFramebuffer(FramebufRef ref);

	bool isValid(FramebufRef ref) const;

	uint32 getSize() const noexcept { return _size; }

	// Callback on window resize. Will resize all textures linked to a framebuffer present in the
	// framebuffer cache. Only textures generated at/during runtime can be resized this way.
	void resizeAll(const int32 width, const int32 height);

	Framebuffer* getFramebuffer(FramebufRef ref) const;
};