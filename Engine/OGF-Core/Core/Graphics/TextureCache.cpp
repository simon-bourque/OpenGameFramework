#include "TextureCache.h"

#include "Core/EngineAssert.h"

#include "Core/Resource/File/FileReader.h"

#include "Core/Graphics/Color.h"

#include "3rdparty/SpookyHash/SpookyV2.h"

#define HASH_SEED 0xFF1A2B3C4D5E6FFF

const static string TEXTURE_PATH = "res/texture/";

TextureCache::TextureCache() {
	const Color& red = Color::RED;
	const Color& grey = Color::GREY;
	float32 data[16] = {
		red.r, red.g, red.b, red.a,
		grey.r, grey.g, grey.b, grey.a,
		grey.r, grey.g, grey.b, grey.a,
		red.r, red.g, red.b, red.a
	};

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

TextureCache::~TextureCache() {
	for (const auto& pair : m_loadedTextures) {
		delete pair.second;
	}
}

TextureRef TextureCache::loadTexture(const string& path) {

	uint64 pathHash = SpookyHash::Hash64(path.c_str(), path.length(), HASH_SEED);

	// Check if texture is already loaded
	auto iter = m_loadedTextures.find(pathHash);
	if (iter != m_loadedTextures.end()) {
		// TODO Add a warning message here
		return pathHash;
	}

	Texture* texture = nullptr;
	
	try {
		FileReader input(TEXTURE_PATH + path);
	
		Texture::Target type = static_cast<Texture::Target>(input.read<uint32>());

		switch (type) {
		case Texture::Target::TEXTURE_2D:
			texture = loadTexture2DFromFile(input);
			break;
		case Texture::Target::TEXTURE_2D_ARRAY:
			texture = loadTexture2DArrayFromFile(input);
			break;
		default:
			input.close();
			throw std::runtime_error("Failed to load texture " + path + ": Failed to identify texture type.");
		}
		
		input.close();
	}
	catch (std::runtime_error& ex) {
		DEBUG_LOG(ex.what());
		return pathHash;
	}

	m_loadedTextures[pathHash] = texture;


	return pathHash;
}

bool TextureCache::isValid(TextureRef textureRef) const {
	auto iter = m_loadedTextures.find(textureRef);

	return (iter != m_loadedTextures.end());
}

Texture* TextureCache::getTexture(TextureRef reference) const {
	auto iter = m_loadedTextures.find(reference);

	if (iter != m_loadedTextures.end()) {
		return iter->second;
	}

	//ASSERT(false, "WOOPS")
	return m_defaultTexture.get();
}

Texture* TextureCache::loadTexture2DFromFile(FileReader& input) const {
	Texture::Filter filter = static_cast<Texture::Filter>(input.read<uint32>());
	Texture::Wrap wrapS = static_cast<Texture::Wrap>(input.read<uint32>());
	Texture::Wrap wrapT = static_cast<Texture::Wrap>(input.read<uint32>());

	uint8 channels = input.read<uint8>();
	uint32 width = input.read<uint32>();
	uint32 height = input.read<uint32>();

	uint32 numBytes = width * height * channels;
	uint8* data = new uint8[numBytes];

	uint32 bytesRead = 0;
	input.read(data, numBytes, bytesRead);

	Texture* tex = new Texture(Texture::Target::TEXTURE_2D);

	tex->bind(Texture::Unit::UNIT_0);

	glTexImage2D(static_cast<GLenum>(tex->m_target), 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glTexParameteri(static_cast<GLenum>(tex->m_target), GL_TEXTURE_MIN_FILTER, static_cast<GLint>(filter));
	glTexParameteri(static_cast<GLenum>(tex->m_target), GL_TEXTURE_MAG_FILTER, static_cast<GLint>(filter));
	glTexParameteri(static_cast<GLenum>(tex->m_target), GL_TEXTURE_WRAP_S, static_cast<GLint>(wrapS));
	glTexParameteri(static_cast<GLenum>(tex->m_target), GL_TEXTURE_WRAP_T, static_cast<GLint>(wrapT));

	tex->unbind();

	delete[] data;

	return tex;
}

Texture* TextureCache::loadTexture2DArrayFromFile(FileReader& input) const {
	Texture::Filter filter = static_cast<Texture::Filter>(input.read<uint32>());
	Texture::Wrap wrapS = static_cast<Texture::Wrap>(input.read<uint32>());
	Texture::Wrap wrapT = static_cast<Texture::Wrap>(input.read<uint32>());

	uint8 channels = input.read<uint8>();
	uint32 width = input.read<uint32>();
	uint32 height = input.read<uint32>();
	uint32 depth = input.read<uint32>();

	uint32 numBytes = width * height * channels * depth;
	uint8* data = new uint8[numBytes];

	uint32 bytesRead = 0;
	input.read(data, numBytes, bytesRead);

	Texture* tex = new Texture(Texture::Target::TEXTURE_2D_ARRAY);
	
	tex->bind(Texture::Unit::UNIT_0);

	glTexImage3D(static_cast<GLenum>(tex->m_target), 0, GL_RGBA, width, height, depth, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glTexParameteri(static_cast<GLenum>(tex->m_target), GL_TEXTURE_MIN_FILTER, static_cast<GLint>(filter));
	glTexParameteri(static_cast<GLenum>(tex->m_target), GL_TEXTURE_MAG_FILTER, static_cast<GLint>(filter));
	glTexParameteri(static_cast<GLenum>(tex->m_target), GL_TEXTURE_WRAP_S, static_cast<GLint>(wrapS));
	glTexParameteri(static_cast<GLenum>(tex->m_target), GL_TEXTURE_WRAP_T, static_cast<GLint>(wrapT));
	
	tex->unbind();

	delete[] data;

	return tex;
}