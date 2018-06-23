#include "FramebufferCache.h"

#include "Core/Assert.h"

#include "3rdparty/SpookyHash/SpookyV2.h"

#include <vector>

#define HASH_SEED 0xCCFA2B3C4D5E6FFF

FramebufferCache::FramebufferCache()
{
	// Default null framebuffer. Used when returning errors
	_loadedFbs[0] = nullptr;
}

FbRef FramebufferCache::genFramebuffer(std::string name)
{
	uint64 nameHash = SpookyHash::Hash64(name.c_str(), name.length(), HASH_SEED);

	auto iter = _loadedFbs.find(nameHash);
	if (iter != _loadedFbs.end())
	{
		// Framebuffer of same name already exists
		return 0;
	}

	uint32 fbid;
	glGenFramebuffers(1, &fbid);

	Framebuffer* framebuffer = new Framebuffer(name, fbid);

	_loadedFbs[nameHash] = framebuffer;

	return nameHash;
}

bool FramebufferCache::isValid(FbRef framebufferRef) const
{
	auto iter = _loadedFbs.find(framebufferRef);

	return (iter != _loadedFbs.end());
}

Framebuffer* FramebufferCache::getFramebuffer(FbRef reference) const
{
	auto iter = _loadedFbs.find(reference);

	if (iter != _loadedFbs.end())
	{
		return iter->second;
	}

	return nullptr;
}

FramebufferCache::~FramebufferCache()
{
	for (const auto& pair : _loadedFbs)
	{
		delete pair.second;
	}
}