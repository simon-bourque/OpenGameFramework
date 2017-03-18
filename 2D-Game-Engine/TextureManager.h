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

	Texture* createTexture2D(const RawImage& img, Texture::Filter filtering, Texture::Wrap textureWrapS, Texture::Wrap textureWrapT);
	Texture* createTexture2DArray(RawImage* imgs, uint32 layers, Texture::Filter filtering, Texture::Wrap textureWrapS, Texture::Wrap textureWrapT);
};

#endif

