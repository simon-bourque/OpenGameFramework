#pragma once
#ifndef FONTMANAGER_H
#define FONTMANAGER_H

#include "Core/Core.h"

#include "Resource/ResourceManager.h"

#include "Graphics/Text/Font.h"

class RenderSystem;

class FontManager : public ResourceManager<Font> {
public:
	FontManager(RenderSystem* rs);
	virtual ~FontManager();

	Font* createFont(const string& name);
};

#endif