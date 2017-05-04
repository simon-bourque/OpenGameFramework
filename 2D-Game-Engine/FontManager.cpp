#include "FontManager.h"

#include "Resources.h"


FontManager::FontManager() {}

FontManager::~FontManager() {}

Font* FontManager::createFont(const string& name, Game* game) {
	string img = name;
	string desc = name;
	img.append(".png");
	desc.append(".fnt");

	Font* font = loadFont(img, desc, game);
	m_loadedResources[name] = font;

	return font;
}