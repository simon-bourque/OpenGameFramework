#include "FontManager.h"

#include "Core/Resource/Resources.h"

#include "Core/Graphics/GraphicsContext.h"
#include "Core/Graphics/TextureManager.h"

FontManager::FontManager() {}

FontManager::~FontManager() {}

Font* FontManager::createFont(const string& name) {
	string img = name;
	string desc = name;
	img.append(".tx");
	desc.append(".fnt");

	Texture* tex = getGraphicsContextInstance()->getTextureManager()->createTexture2D(img, Texture::Filter::LINEAR);

	Glyph invalidCharacter;
	uint32 size = 0;
	std::pair<char, Glyph>* map = loadFont(desc, size, invalidCharacter);
	
	Font* font = new Font(tex, invalidCharacter, map, size);
	delete[] map;

	m_loadedResources[name] = font;

	return font;
}