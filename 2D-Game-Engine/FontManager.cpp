#include "FontManager.h"

#include "Resources.h"
#include "RenderSystem.h"
#include "TextureManager.h"
#include "RawImage.h"

FontManager::FontManager(RenderSystem* rs) : ResourceManager(rs) {}

FontManager::~FontManager() {}

Font* FontManager::createFont(const string& name) {
	string img = name;
	string desc = name;
	img.append(".png");
	desc.append(".fnt");

	Texture* tex = m_rs->getTextureManager()->createTexture2D(img, Texture::Filter::LINEAR);

	Glyph invalidCharacter;
	uint32 size = 0;
	std::pair<char, Glyph>* map = loadFont(desc, size, invalidCharacter);
	
	Font* font = new Font(tex, invalidCharacter, map, size);
	delete[] map;

	m_loadedResources[name] = font;

	return font;
}