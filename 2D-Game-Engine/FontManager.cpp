#include "FontManager.h"

#include "Resources.h"
#include "Game.h"
#include "RenderSystem.h"
#include "TextureManager.h"
#include "RawImage.h"

FontManager::FontManager() {}

FontManager::~FontManager() {}

Font* FontManager::createFont(const string& name, Game* game) {
	string img = name;
	string desc = name;
	img.append(".png");
	desc.append(".fnt");

	Texture* tex = game->getRenderSystem().getTextureManager()->createTexture2D(img, Texture::Filter::LINEAR);

	Glyph invalidCharacter;
	uint32 size = 0;
	std::pair<char, Glyph>* map = loadFont(desc, size, invalidCharacter);
	
	Font* font = new Font(tex, invalidCharacter, map, size);
	delete[] map;

	m_loadedResources[name] = font;

	return font;
}