#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include "Core.h"
#include "Texture.h"
#include "ResourceManager.h"

class RawImage;
class Buffer;

class TextureManager : public ResourceManager<Texture> {
public:
	TextureManager();
	virtual ~TextureManager();

	Texture* createTexture2D(const string& name, Texture::Filter filtering, Texture::Wrap textureWrapS = Texture::Wrap::CLAMP_TO_EDGE, Texture::Wrap textureWrapT = Texture::Wrap::CLAMP_TO_EDGE);
	Texture* createTexture2DArray(const string& name, int32 margin, int32 spacing, int32 tileWidth, int32 tileHeight, Texture::Filter filtering, Texture::Wrap textureWrapS = Texture::Wrap::CLAMP_TO_EDGE, Texture::Wrap textureWrapT = Texture::Wrap::CLAMP_TO_EDGE);
	Texture* createTextureBuffer(const string& name, Buffer* buffer);
};

#endif

