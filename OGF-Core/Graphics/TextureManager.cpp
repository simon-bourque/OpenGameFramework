#include "TextureManager.h"

#include "Resource/RawImage.h"
#include "Resource/Resources.h"

#include "Graphics/Memory/Buffer.h"
#include "Graphics/Color.h"

TextureManager::TextureManager() {
	const Color& red = Color::RED;
	const Color& grey = Color::GREY;
	float32 data[16] = {
		red.r, red.g, red.b, red.a,
		grey.r, grey.g, grey.b, grey.a,
		grey.r, grey.g, grey.b, grey.a,
		red.r, red.g, red.b, red.a};

	Texture::Filter filtering = Texture::Filter::NEAREST_NEIGHBOR;
	Texture::Wrap textureWrapS = Texture::Wrap::CLAMP_TO_EDGE;
	Texture::Wrap textureWrapT = Texture::Wrap::CLAMP_TO_EDGE;

	Texture* tex = new Texture(Texture::Target::TEXTURE_2D);
	tex->bind(Texture::Unit::UNIT_0);

	glTexImage2D(static_cast<GLenum>(tex->m_target), 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_FLOAT, data);
	glTexParameteri(static_cast<GLenum>(tex->m_target), GL_TEXTURE_MIN_FILTER, static_cast<GLint>(filtering));
	glTexParameteri(static_cast<GLenum>(tex->m_target), GL_TEXTURE_MAG_FILTER, static_cast<GLint>(filtering));
	glTexParameteri(static_cast<GLenum>(tex->m_target), GL_TEXTURE_WRAP_S, static_cast<GLint>(textureWrapS));
	glTexParameteri(static_cast<GLenum>(tex->m_target), GL_TEXTURE_WRAP_T, static_cast<GLint>(textureWrapT));

	tex->unbind();

	m_defaultTexture.reset(tex);
}


TextureManager::~TextureManager() {}

Texture* TextureManager::createTexture2D(const string& name, Texture::Filter filtering, Texture::Wrap textureWrapS, Texture::Wrap textureWrapT) {	

	//RawImage* img = nullptr;
	uint8* data = nullptr;
	uint32 width = 0;
	uint32 height = 0;
	uint8 channels = 0;
	try {
		//img = loadImage(name);
		data = loadTexture(name, 0x1, width, height, channels);
	}
	catch (std::runtime_error& ex) {
		DEBUG_LOG(ex.what());
		return m_defaultTexture.get();
	}

	Texture* tex = new Texture(Texture::Target::TEXTURE_2D);
	//Texture tex(name, Texture::TEXTURE_2D, Texture::UNIT_0);
	tex->bind(Texture::Unit::UNIT_0);
	//glBindTexture(Texture::TEXTURE_2D, name);
	
	//glTexImage2D(static_cast<GLenum>(tex->m_target), 0, GL_RGBA, img->getWidth(), img->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, img->getData());
	glTexImage2D(static_cast<GLenum>(tex->m_target), 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glTexParameteri(static_cast<GLenum>(tex->m_target), GL_TEXTURE_MIN_FILTER, static_cast<GLint>(filtering));
	glTexParameteri(static_cast<GLenum>(tex->m_target), GL_TEXTURE_MAG_FILTER, static_cast<GLint>(filtering));
	glTexParameteri(static_cast<GLenum>(tex->m_target), GL_TEXTURE_WRAP_S, static_cast<GLint>(textureWrapS));
	glTexParameteri(static_cast<GLenum>(tex->m_target), GL_TEXTURE_WRAP_T, static_cast<GLint>(textureWrapT));

	//glBindTexture(Texture::Target::TEXTURE_2D, 0);
	tex->unbind();
	//Texture tex(name, Texture::TEXTURE_2D, Texture::UNIT_0);
	//m_loadedTextures.push_back(tex);
	//m_textures.push_back(tex);
	m_loadedResources[name] = tex;

	//delete img;
	delete[] data;
	return tex;
}

Texture* TextureManager::createTexture2DArray(const string& name, Texture::Filter filtering, Texture::Wrap textureWrapS, Texture::Wrap textureWrapT) {
	uint8* data = nullptr;
	uint32 width = 0;
	uint32 height = 0;
	uint8 channels = 0;
	uint32 depth = 0;
	try {
		//img = loadImage(name);
		data = loadTexture(name, 0x2, width, height, channels, depth);
	}
	catch (std::runtime_error& ex) {
		DEBUG_LOG(ex.what());
		// TODO this doesnt work properly
		return m_defaultTexture.get();
	}

	Texture* tex = new Texture(Texture::Target::TEXTURE_2D_ARRAY);
	//Texture tex(name, Texture::Target::TEXTURE_2D_ARRAY, Texture::UNIT_0);

	tex->bind(Texture::Unit::UNIT_0);
	glTexImage3D(static_cast<GLenum>(tex->m_target), 0, GL_RGBA, width, height, depth, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	glTexParameteri(static_cast<GLenum>(tex->m_target), GL_TEXTURE_MIN_FILTER, static_cast<GLint>(filtering));
	glTexParameteri(static_cast<GLenum>(tex->m_target), GL_TEXTURE_MAG_FILTER, static_cast<GLint>(filtering));
	glTexParameteri(static_cast<GLenum>(tex->m_target), GL_TEXTURE_WRAP_S, static_cast<GLint>(textureWrapS));
	glTexParameteri(static_cast<GLenum>(tex->m_target), GL_TEXTURE_WRAP_T, static_cast<GLint>(textureWrapT));
	tex->unbind();

	delete[] data;
	//m_textures.push_back(tex);
	m_loadedResources[name] = tex;

	return tex;
}

Texture* TextureManager::createTextureBuffer(const string& name, Buffer* buffer) {
	Texture* tex = new Texture(Texture::Target::TEXTURE_BUFFER);
	tex->bind(Texture::Unit::UNIT_0);
	glTexBuffer(static_cast<GLenum>(tex->m_target), GL_R32F, buffer->getHandle());
	tex->unbind();

	m_loadedResources[name] = tex;

	return tex;
}
