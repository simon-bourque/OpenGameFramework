#ifndef FONTMANAGER_H
#define FONTMANAGER_H

#include "Core.h"

#include "ResourceManager.h"
#include "Font.h"

class RenderSystem;

class FontManager : public ResourceManager<Font> {
public:
	FontManager(RenderSystem* rs);
	virtual ~FontManager();

	Font* createFont(const string& name);
};

#endif