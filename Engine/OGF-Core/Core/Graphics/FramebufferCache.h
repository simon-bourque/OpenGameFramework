#pragma once

#include "Core/Core.h"

#include "Core/Graphics/Framebuffer.h"

#include <unordered_map>

typedef uint64 FbRef;

class FramebufferCache
{
private:
	class FbReferenceHash
	{
	public:
		size_t operator()(const FbRef& k) const { return k; }
	};

	std::unordered_map<FbRef, Framebuffer*, FbReferenceHash> _loadedFbs;

	// Current number of framebuffers in the cache
	uint32 _size;

	// The regenerated framebuffers will use these dimensions
	int32 _width;
	int32 _height;

public:
	FramebufferCache(uint32 width, uint32 height);
	~FramebufferCache();

	FbRef genFramebuffer(const std::string& name, Framebuffer::Attachment att);
	void destroyFramebuffer(FbRef framebufferRef);

	bool isValid(FbRef framebufferRef) const;

	uint32 getSize() const noexcept { return _size; }

	// Callback on window resize. Will resize all textures linked to a framebuffer present in the
	// framebuffer cache. Only textures generated at/during runtime can be resized this way.
	void resizeAll(const int32 width, const int32 height);

	Framebuffer* getFramebuffer(FbRef reference) const;
};