#ifndef TEXTMANAGER_H
#define TEXTMANAGER_H

#include "Core.h"
#include "ResourceManager.h"
#include "Text.h"

class RenderSystem;

class TextManager : public ResourceManager<Text> {
public:
	TextManager(RenderSystem* rs);
	virtual ~TextManager();

	Text* createText(const string& name, const string& text, Font* font, Text::Usage usage);
};

#endif