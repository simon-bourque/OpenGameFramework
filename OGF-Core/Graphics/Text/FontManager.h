#pragma once

#include "Core/Core.h"

#include "Resource/ResourceManager.h"

#include "Graphics/Text/Font.h"

class FontManager : public ResourceManager<Font> {
public:
	FontManager();
	virtual ~FontManager();

	Font* createFont(const string& name);
};