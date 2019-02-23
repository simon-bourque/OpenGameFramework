#include "TextureCache.h"

#include "Core/Assert.h"

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

	Texture* tex = new Texture(Texture::Target::TEXTURE_2D, Texture::InternalFormat::RGBA, Texture::Format::RGBA, Texture::PixelDataType::FLOAT);
	tex->bind(Texture::Unit::UNIT_0);

	glTexImage2D(
		static_cast<GLenum>(tex->_target),
		0,
		static_cast<GLenum>(tex->_internalFormat),
		2,
		2,
		0,
		static_cast<GLenum>(tex->_format),
		static_cast<GLenum>(tex->_type),
		data);

	glTexParameteri(static_cast<GLenum>(tex->_target), GL_TEXTURE_MIN_FILTER, static_cast<GLint>(filtering));
	glTexParameteri(static_cast<GLenum>(tex->_target), GL_TEXTURE_MAG_FILTER, static_cast<GLint>(filtering));
	glTexParameteri(static_cast<GLenum>(tex->_target), GL_TEXTURE_WRAP_S, static_cast<GLint>(textureWrapS));
	glTexParameteri(static_cast<GLenum>(tex->_target), GL_TEXTURE_WRAP_T, static_cast<GLint>(textureWrapT));

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

TextureRef TextureCache::genTexture(
	const string& name,
	const int32 width,
	const int32 height,
	Texture::Target target,
	Texture::InternalFormat internalFormat,
	Texture::Format format,
	Texture::PixelDataType type) 
{
	uint64 nameHash = SpookyHash::Hash64(name.c_str(), name.length(), HASH_SEED);

	// Check if texture with same name exists
	auto iter = m_loadedTextures.find(nameHash);
	if (iter != m_loadedTextures.end()) {
		// TODO Add a warning message here
		return nameHash;
	}

	Texture* tex = new Texture(target, internalFormat, format, type);

	tex->bind(Texture::Unit::UNIT_0);

	// Allocating GPU memory for texture
	glTexImage2D(
		static_cast<GLenum>(tex->_target),
		0,
		static_cast<GLenum>(tex->_internalFormat),
		width,
		height,
		0,
		static_cast<GLenum>(tex->_format),
		static_cast<GLenum>(tex->_type),
		0);

	glTexParameteri(static_cast<GLenum>(tex->_target), GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(static_cast<GLenum>(tex->_target), GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(static_cast<GLenum>(tex->_target), GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(static_cast<GLenum>(tex->_target), GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	tex->unbind();

	m_loadedTextures[nameHash] = tex;

	return nameHash;
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

	Texture* tex = new Texture(Texture::Target::TEXTURE_2D, Texture::InternalFormat::RGBA, Texture::Format::RGBA, Texture::PixelDataType::UBYTE);

	tex->bind(Texture::Unit::UNIT_0);

	// Allocating GPU memory for texture
	glTexImage2D(
		static_cast<GLenum>(tex->_target),
		0,
		static_cast<GLenum>(tex->_internalFormat),
		width,
		height,
		0,
		static_cast<GLenum>(tex->_format),
		static_cast<GLenum>(tex->_type),
		data);

	glTexParameteri(static_cast<GLenum>(tex->_target), GL_TEXTURE_MIN_FILTER, static_cast<GLint>(filter));
	glTexParameteri(static_cast<GLenum>(tex->_target), GL_TEXTURE_MAG_FILTER, static_cast<GLint>(filter));
	glTexParameteri(static_cast<GLenum>(tex->_target), GL_TEXTURE_WRAP_S, static_cast<GLint>(wrapS));
	glTexParameteri(static_cast<GLenum>(tex->_target), GL_TEXTURE_WRAP_T, static_cast<GLint>(wrapT));

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

	Texture* tex = new Texture(Texture::Target::TEXTURE_2D_ARRAY, Texture::InternalFormat::RGBA, Texture::Format::RGBA, Texture::PixelDataType::UBYTE);
	
	tex->bind(Texture::Unit::UNIT_0);

	// Allocating GPU memory for texture
	glTexImage3D(
		static_cast<GLenum>(tex->_target),
		0,
		static_cast<GLenum>(tex->_internalFormat),
		width,
		height,
		depth,
		0,
		static_cast<GLenum>(tex->_format),
		static_cast<GLenum>(tex->_type),
		data);

	glTexParameteri(static_cast<GLenum>(tex->_target), GL_TEXTURE_MIN_FILTER, static_cast<GLint>(filter));
	glTexParameteri(static_cast<GLenum>(tex->_target), GL_TEXTURE_MAG_FILTER, static_cast<GLint>(filter));
	glTexParameteri(static_cast<GLenum>(tex->_target), GL_TEXTURE_WRAP_S, static_cast<GLint>(wrapS));
	glTexParameteri(static_cast<GLenum>(tex->_target), GL_TEXTURE_WRAP_T, static_cast<GLint>(wrapT));
	
	tex->unbind();

	delete[] data;

	return tex;
}

void TextureCache::destroyTexture(TextureRef ref) {
	auto iter = m_loadedTextures.find(ref);

	if (iter != m_loadedTextures.end()) {
		m_loadedTextures.erase(ref);
	}
}