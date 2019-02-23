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
	
	TextureRef genTexture(
		const string& name,
		const int32 width,
		const int32 height,
		Texture::Target target,
		Texture::InternalFormat internalFormat,
		Texture::Format format,
		Texture::PixelDataType type
	);

	TextureRef loadTexture(const string& path);
	bool isValid(TextureRef textureRef) const;

	Texture* getTexture(TextureRef reference) const;
	void destroyTexture(TextureRef reference);

};
