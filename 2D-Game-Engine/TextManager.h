#ifndef TEXTMANAGER_H
#define TEXTMANAGER_H

#include "Core.h"
#include "ResourceManager.h"
#include "Text.h"

class TextManager : public ResourceManager<Text> {
public:
	TextManager();
	virtual ~TextManager();

	Text* createText(const string& name, const string& text, Font* font, Text::Usage usage);
};

#endif