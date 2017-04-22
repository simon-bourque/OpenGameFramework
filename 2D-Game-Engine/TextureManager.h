#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include "Core.h"
#include "Texture.h"
#include <vector>

class RawImage;

class TextureManager
{
private:
	vector<Texture*> m_textures;
public:
	TextureManager();
	virtual ~TextureManager();

	uint32 getNumberOfLoadedTextures() { return m_textures.size(); };

	Texture* createTexture2D(const RawImage& img, Texture::Filter filtering, Texture::Wrap textureWrapS = Texture::Wrap::CLAMP_TO_EDGE, Texture::Wrap textureWrapT = Texture::Wrap::CLAMP_TO_EDGE);
	Texture* createTexture2DArray(RawImage* imgs, uint32 layers, Texture::Filter filtering, Texture::Wrap textureWrapS = Texture::Wrap::CLAMP_TO_EDGE, Texture::Wrap textureWrapT = Texture::Wrap::CLAMP_TO_EDGE);
};

#endif

