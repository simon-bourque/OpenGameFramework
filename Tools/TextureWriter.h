#pragma once

#include "Core/Graphics/Texture.h"
#include <string>

class TextureWriter {
private:
	std::string m_path;
	std::string m_newPath;

	Texture::Target m_target;
	Texture::Filter m_filter;
	Texture::Wrap m_textureWrapS;
	Texture::Wrap m_textureWrapT;
public:
	TextureWriter(const std::string& path, const std::string& newPath, Texture::Target target, Texture::Filter filter, Texture::Wrap textureWrapS, Texture::Wrap textureWrapT);
	virtual ~TextureWriter();
	
	void convert();
};

