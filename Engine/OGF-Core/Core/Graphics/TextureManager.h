#pragma once

#include "Core/Core.h"

#include "Core/Graphics/Texture.h"

#include "Core/Resource/ResourceManager.h"

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
	Texture* createTexture2DArray(const string& name, Texture::Filter filtering, Texture::Wrap textureWrapS = Texture::Wrap::CLAMP_TO_EDGE, Texture::Wrap textureWrapT = Texture::Wrap::CLAMP_TO_EDGE);
	Texture* createTextureBuffer(const string& name, Buffer* buffer);
};