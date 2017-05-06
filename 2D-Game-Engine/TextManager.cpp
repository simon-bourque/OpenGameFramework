#include "TextManager.h"



TextManager::TextManager(RenderSystem* rs) : ResourceManager(rs) {}

TextManager::~TextManager() {}

Text* TextManager::createText(const string& name, const string& text, Font* font, Text::Usage usage) {
	Text* textPtr = new Text(text, font, usage);
	m_loadedResources[name] = textPtr;
	return textPtr;
}