#include "TextureManager.h"

#include "RawImage.h"

TextureManager::TextureManager() {}


TextureManager::~TextureManager() {
	for (Texture* tex : m_textures) {
		delete tex;
	}
}

Texture* TextureManager::createTexture2D(const RawImage& img, Texture::Filter filtering, Texture::Wrap textureWrapS, Texture::Wrap textureWrapT) {	
	
	Texture* tex = new Texture(Texture::Target::TEXTURE_2D, GL_TEXTURE0);
	//Texture tex(name, Texture::TEXTURE_2D, Texture::UNIT_0);
	tex->bind();
	//glBindTexture(Texture::TEXTURE_2D, name);
	
	glTexImage2D(tex->m_target, 0, GL_RGBA, img.getWidth(), img.getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, img.getData());
	glTexParameteri(tex->m_target, GL_TEXTURE_MIN_FILTER, filtering);
	glTexParameteri(tex->m_target, GL_TEXTURE_MAG_FILTER, filtering);
	glTexParameteri(tex->m_target, GL_TEXTURE_WRAP_S, textureWrapS);
	glTexParameteri(tex->m_target, GL_TEXTURE_WRAP_T, textureWrapT);

	//glBindTexture(Texture::Target::TEXTURE_2D, 0);
	tex->unbind();
	//Texture tex(name, Texture::TEXTURE_2D, Texture::UNIT_0);
	//m_loadedTextures.push_back(tex);
	m_textures.push_back(tex);

	return tex;
}

Texture* TextureManager::createTexture2DArray(RawImage* imgs, uint32 layers, Texture::Filter filtering, Texture::Wrap textureWrapS, Texture::Wrap textureWrapT) {

	uint32 width = imgs[0].getWidth();
	uint32 height = imgs[0].getHeight();
	uint32 subImgSize = width * height * imgs[0].getChannels();

	uint8* data = new uint8[subImgSize * layers];

	// TODO Ensure all images are the same size
	for (uint32 i = 0; i < layers; i++) {
		uint8* subImgData = imgs[layers].getData();
		for (uint32 j = 0; j < subImgSize; j++) {
			data[(i * subImgSize) + j] = subImgData[j];
		}
	}
	
	Texture* tex = new Texture(Texture::Target::TEXTURE_2D_ARRAY, GL_TEXTURE0);
	//Texture tex(name, Texture::Target::TEXTURE_2D_ARRAY, Texture::UNIT_0);

	tex->bind();
	glTexImage3D(tex->m_target, 0, GL_RGBA, width, height, layers, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	glTexParameteri(tex->m_target, GL_TEXTURE_MIN_FILTER, filtering);
	glTexParameteri(tex->m_target, GL_TEXTURE_MAG_FILTER, filtering);
	glTexParameteri(tex->m_target, GL_TEXTURE_WRAP_S, textureWrapS);
	glTexParameteri(tex->m_target, GL_TEXTURE_WRAP_T, textureWrapT);
	tex->unbind();

	m_textures.push_back(tex);

	return tex;
}