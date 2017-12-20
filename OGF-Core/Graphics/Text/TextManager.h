#pragma once

#include "Core/Core.h"

#include "Resource/ResourceManager.h"

#include "Graphics/Text/Text.h"

class TextManager : public ResourceManager<Text> {
public:
	TextManager();
	virtual ~TextManager();

	Text* createText(const string& name, const string& text, Font* font, Text::Usage usage);
};