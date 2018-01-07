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

	Texture* loadTexture2DFromFile(FileReader& input, Texture::Filter filtering, Texture::Wrap textureWrapS, Texture::Wrap textureWrapT) const;
	Texture* loadTexture2DArrayFromFile(FileReader& input, Texture::Filter filtering, Texture::Wrap textureWrapS, Texture::Wrap textureWrapT) const;
public:
	TextureCache();
	~TextureCache();

	TextureRef loadTexture(const string& path, Texture::Filter filtering, Texture::Wrap textureWrapS = Texture::Wrap::CLAMP_TO_EDGE, Texture::Wrap textureWrapT = Texture::Wrap::CLAMP_TO_EDGE);

	Texture* getTexture(TextureRef reference) const;

};
