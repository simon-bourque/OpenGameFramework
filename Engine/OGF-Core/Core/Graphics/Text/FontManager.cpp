#include "FontManager.h"

#include "Core/Resource/Resources.h"

#include "Core/Graphics/GraphicsContext.h"
#include "Core/Graphics/TextureCache.h"

FontManager::FontManager() {}

FontManager::~FontManager() {}

Font* FontManager::createFont(const string& name) {
	string img = name;
	string desc = name;
	img.append(".tx");
	desc.append(".fnt");

	TextureRef textureRef = getGraphicsContextInstance()->getTextureCache()->loadTexture(img, Texture::Filter::LINEAR);

	Glyph invalidCharacter;
	uint32 size = 0;
	std::pair<char, Glyph>* map = loadFont(desc, size, invalidCharacter);
	
	Font* font = new Font(textureRef, invalidCharacter, map, size);
	delete[] map;

	m_loadedResources[name] = font;

	return font;
}