#pragma once

#include "Core/Core.h"

#include "Core/Resource/ResourceManager.h"

#include "Core/Graphics/Text/Font.h"

class FontManager : public ResourceManager<Font> {
public:
	FontManager();
	virtual ~FontManager();

	Font* createFont(const string& name);
};