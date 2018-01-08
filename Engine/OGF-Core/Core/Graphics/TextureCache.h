#pragma once

#include "Core/Core.h"

#include "Core/Graphics/Texture.h"

#include <unordered_map>
#include <memory>

class FileReader;

typedef uint64 TextureRef;

class TextureCache {
private:
	class TextureReferenceHash {
	public:
		size_t operator()(const TextureRef& k) const { return k; }
	};
	
	std::unordered_map<TextureRef, Texture*, TextureReferenceHash> m_loadedTextures;
	std::unique_ptr<Texture> m_defaultTexture;

	Texture* loadTexture2DFromFile(FileReader& input) const;
	Texture* loadTexture2DArrayFromFile(FileReader& input) const;
public:
	TextureCache();
	~TextureCache();

	TextureRef loadTexture(const string& path);

	Texture* getTexture(TextureRef reference) const;

};
