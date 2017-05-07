#pragma once
#ifndef TEXTMANAGER_H
#define TEXTMANAGER_H

#include "Core/Core.h"

#include "Resource/ResourceManager.h"

#include "Graphics/Text/Text.h"

class RenderSystem;

class TextManager : public ResourceManager<Text> {
public:
	TextManager(RenderSystem* rs);
	virtual ~TextManager();

	Text* createText(const string& name, const string& text, Font* font, Text::Usage usage);
};

#endif