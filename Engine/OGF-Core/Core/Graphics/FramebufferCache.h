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

	// The generated framebuffers will use these dimensions
	uint32 _width;
	uint32 _height;

public:
	FramebufferCache(uint32 width, uint32 height);
	~FramebufferCache();

	FbRef genFramebuffer(const std::string& name, Framebuffer::Attachment att);
	void destroyFramebuffer(FbRef framebufferRef);
	//std::vector<FbRef> genFramebuffer(std::vector<std::string> names);

	bool isValid(FbRef framebufferRef) const;

	uint32 getSize() const noexcept { return _size; }
	Framebuffer* getFramebuffer(FbRef reference) const;
};