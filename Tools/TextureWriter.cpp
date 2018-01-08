#include "TextureWriter.h"



TextureWriter::TextureWriter(const std::string& path, const std::string& newPath, Texture::Target target, Texture::Filter filter, Texture::Wrap textureWrapS, Texture::Wrap textureWrapT) :
	m_path(path),
	m_newPath(newPath),
	m_target(target),
	m_filter(filter),
	m_textureWrapS(textureWrapS),
	m_textureWrapT(textureWrapT)
{}

TextureWriter::~TextureWriter() {}
