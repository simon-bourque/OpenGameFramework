#include "FontManager.h"

#include "Resource/Resources.h"

#include "Graphics/GraphicsContext.h"
#include "Graphics/TextureManager.h"

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