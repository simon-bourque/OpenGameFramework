#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include "Core.h"
#include "Texture.h"
#include "ResourceManager.h"

#include <memory>

class RenderSystem;
class RawImage;
class Buffer;

class TextureManager : public ResourceManager<Texture> {
private:
	std::unique_ptr<Texture> m_defaultTexture;
public:
	TextureManager(RenderSystem* rs);
	virtual ~TextureManager();

	Texture* createTexture2D(const string& name, Texture::Filter filtering, Texture::Wrap textureWrapS = Texture::Wrap::CLAMP_TO_EDGE, Texture::Wrap textureWrapT = Texture::Wrap::CLAMP_TO_EDGE);
	Texture* createTexture2DArray(const string& name, const string& infoName, uint32* frames, uint32 framesSize, Texture::Filter filtering, Texture::Wrap textureWrapS = Texture::Wrap::CLAMP_TO_EDGE, Texture::Wrap textureWrapT = Texture::Wrap::CLAMP_TO_EDGE);
	Texture* createTexture2DArray(const string& name, int32 margin, int32 spacing, int32 tileWidth, int32 tileHeight, Texture::Filter filtering, Texture::Wrap textureWrapS = Texture::Wrap::CLAMP_TO_EDGE, Texture::Wrap textureWrapT = Texture::Wrap::CLAMP_TO_EDGE);
	Texture* createTextureBuffer(const string& name, Buffer* buffer);
};

#endif

