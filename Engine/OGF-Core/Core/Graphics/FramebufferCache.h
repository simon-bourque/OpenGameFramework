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

	uint32 _size;

public:
	FramebufferCache();
	~FramebufferCache();

	FbRef genFramebuffer(std::string name);
	//std::vector<FbRef> genFramebuffer(std::vector<std::string> names);

	bool isValid(FbRef framebufferRef) const;

	uint32 getSize() const noexcept { return _size; }
	Framebuffer* getFramebuffer(FbRef reference) const noexcept;
};