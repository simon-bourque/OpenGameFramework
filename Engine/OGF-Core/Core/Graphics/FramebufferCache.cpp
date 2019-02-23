#include "FramebufferCache.h"

#include "Core/Assert.h"

#include "3rdparty/SpookyHash/SpookyV2.h"

#include <vector>

#define HASH_SEED 0xCCFA2B3C4D5E6FFF

FramebufferCache::FramebufferCache(uint32 width, uint32 height)
	: _width(width)
	, _height(height)
{
	// Default null framebuffer. Used when returning errors
	_loadedFbs[0] = nullptr;
}

FbRef FramebufferCache::genFramebuffer(const std::string& name, Framebuffer::Attachment att) {
	uint64 nameHash = SpookyHash::Hash64(name.c_str(), name.length(), HASH_SEED);

	auto iter = _loadedFbs.find(nameHash);
	if (iter != _loadedFbs.end())
	{
		// Framebuffer of same name already exists
		return 0;
	}

	uint32 fbid;
	glGenFramebuffers(1, &fbid);

	Framebuffer* framebuffer = new Framebuffer(name, att);

	_loadedFbs[nameHash] = framebuffer;

	_size++;

	return nameHash;
}

void FramebufferCache::destroyFramebuffer(FbRef framebufferRef) {
	auto iter = _loadedFbs.find(framebufferRef);

	if (iter != _loadedFbs.end())
	{
		_loadedFbs.erase(framebufferRef);
	}
}

bool FramebufferCache::isValid(FbRef framebufferRef) const {
	auto iter = _loadedFbs.find(framebufferRef);

	return (iter != _loadedFbs.end());
}

void FramebufferCache::resizeAll(const int32 width, const int32 height) {
	_width = width;
	_height = height;

	for (auto& pair : _loadedFbs)
	{
		pair.second->resize(_width, _height);
	}
}

Framebuffer* FramebufferCache::getFramebuffer(FbRef reference) const {
	auto iter = _loadedFbs.find(reference);

	if (iter != _loadedFbs.end())
	{
		return iter->second;
	}

	return nullptr;
}

FramebufferCache::~FramebufferCache() {
	for (const auto& pair : _loadedFbs)
	{
		delete pair.second;
	}
}