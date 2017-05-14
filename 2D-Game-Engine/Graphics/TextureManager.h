#pragma once
#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include "Core/Core.h"

#include "Graphics/Texture.h"

#include "Resource/ResourceManager.h"

#include <memory>

class RawImage;
class Buffer;

class TextureManager : public ResourceManager<Texture> {
private:
	std::unique_ptr<Texture> m_defaultTexture;
public:
	TextureManager();
	virtual ~TextureManager();

	Texture* createTexture2D(const string& name, Texture::Filter filtering, Texture::Wrap textureWrapS = Texture::Wrap::CLAMP_TO_EDGE, Texture::Wrap textureWrapT = Texture::Wrap::CLAMP_TO_EDGE);
	Texture* createTexture2DArray(const string& name, const string& infoName, uint32* frames, uint32 framesSize, Texture::Filter filtering, Texture::Wrap textureWrapS = Texture::Wrap::CLAMP_TO_EDGE, Texture::Wrap textureWrapT = Texture::Wrap::CLAMP_TO_EDGE);
	Texture* createTexture2DArray(const string& name, int32 margin, int32 spacing, int32 tileWidth, int32 tileHeight, Texture::Filter filtering, Texture::Wrap textureWrapS = Texture::Wrap::CLAMP_TO_EDGE, Texture::Wrap textureWrapT = Texture::Wrap::CLAMP_TO_EDGE);
	Texture* createTexture2DArray(const string& name, Texture::Filter filtering, Texture::Wrap textureWrapS = Texture::Wrap::CLAMP_TO_EDGE, Texture::Wrap textureWrapT = Texture::Wrap::CLAMP_TO_EDGE);
	Texture* createTextureBuffer(const string& name, Buffer* buffer);
};

#endif

