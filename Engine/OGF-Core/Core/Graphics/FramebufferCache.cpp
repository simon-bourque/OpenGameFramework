#include "FramebufferCache.h"

#include "Core/Assert.h"
#include "Core/Window/Window.h"

#include "3rdparty/SpookyHash/SpookyV2.h"

#include <vector>

#define HASH_SEED 0xCCFA2B3C4D5E6FFF

FramebufferCache::FramebufferCache()
{
	if (Singleton<Window>::isInitialized()) {
		_width = getWindowInstance()->getWidth();
		_height = getWindowInstance()->getHeight();
	}
}

FramebufRef FramebufferCache::genFramebuffer(const std::string& name) {
	uint64 nameHash = SpookyHash::Hash64(name.c_str(), name.length(), HASH_SEED);

	auto iter = _loadedFbs.find(nameHash);
	if (iter != _loadedFbs.end()) {
		// Framebuffer of same name already exists
		return 0;
	}

	Framebuffer* framebuffer = new Framebuffer(name);

	_loadedFbs[nameHash] = framebuffer;

	return nameHash;
}

void FramebufferCache::destroyFramebuffer(FramebufRef ref) {
	auto iter = _loadedFbs.find(ref);

	if (iter != _loadedFbs.end()) {
		_loadedFbs.erase(ref);
	}
}

bool FramebufferCache::isValid(FramebufRef ref) const {
	auto iter = _loadedFbs.find(ref);

	return (iter != _loadedFbs.end());
}

void FramebufferCache::resizeAll(const int32 width, const int32 height) {
	_width = width;
	_height = height;

	for (auto& pair : _loadedFbs) {
		pair.second->resize(_width, _height);
	}
}

Framebuffer* FramebufferCache::getFramebuffer(FramebufRef ref) const {
	auto iter = _loadedFbs.find(ref);

	if (iter != _loadedFbs.end()) {
		return iter->second;
	}

	return nullptr;
}

FramebufferCache::~FramebufferCache() {
	for (const auto& pair : _loadedFbs) {
		delete pair.second;
	}
}