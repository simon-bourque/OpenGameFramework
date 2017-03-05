#include "Texture.h"

#include "RawImage.h"

Texture::Texture(Target target, GLenum unit) : m_target(target), m_unit(unit) {
	GLuint* name = 0;
	glGenTextures(1, name);
	m_name = *name;
}

Texture::~Texture() {
	glDeleteTextures(1, &m_name);
}

void Texture::bind() const {
	glActiveTexture(m_unit);
	glBindTexture(m_target, m_name);
}

void Texture::unbind() const {
	glBindTexture(m_target, 0);
}

Texture* Texture::createTexture2D(const RawImage& img, Texture::Filter filtering, Texture::Wrap textureWrapS, Texture::Wrap textureWrapT) {
	Texture* tex = new Texture(Texture::Target::TEXTURE_2D, GL_TEXTURE0);

	tex->bind();
	glTexImage2D(tex->m_target, 0, GL_RGBA, img.getWidth(), img.getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, img.getData());

	glTexParameteri(tex->m_target, GL_TEXTURE_MIN_FILTER, filtering);
	glTexParameteri(tex->m_target, GL_TEXTURE_MAG_FILTER, filtering);
	glTexParameteri(tex->m_target, GL_TEXTURE_WRAP_S, textureWrapS);
	glTexParameteri(tex->m_target, GL_TEXTURE_WRAP_T, textureWrapT);
	
	tex->unbind();

	return tex;
}

Texture* Texture::createTexture2DArray(RawImage* imgs, uint32 layers, Texture::Filter filtering, Texture::Wrap textureWrapS, Texture::Wrap textureWrapT) {

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

	tex->bind();
	glTexImage3D(tex->m_target, 0, GL_RGBA, width, height, layers, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	glTexParameteri(tex->m_target, GL_TEXTURE_MIN_FILTER, filtering);
	glTexParameteri(tex->m_target, GL_TEXTURE_MAG_FILTER, filtering);
	glTexParameteri(tex->m_target, GL_TEXTURE_WRAP_S, textureWrapS);
	glTexParameteri(tex->m_target, GL_TEXTURE_WRAP_T, textureWrapT);
	tex->unbind();

	return tex;
}