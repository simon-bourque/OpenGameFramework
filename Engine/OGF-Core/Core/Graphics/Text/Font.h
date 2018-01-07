#pragma once

#include "Core/Core.h"

#include "Core/Graphics/Text/Glyph.h"
#include "Core/Graphics/TextureCache.h"

#include <map>
#include <utility>

class Font {
private:
	TextureRef m_bitmap;
	std::map<char, Glyph> m_glyphMap;
	Glyph m_invalidCharacter;
public:
	Font(TextureRef bitmap, const Glyph& invalidCharacter);
	Font(TextureRef bitmap, const Glyph& invalidCharacter, std::pair<char, Glyph>* characterMap, uint32 characterMapSize);

	TextureRef getBitmap() const { return m_bitmap; };

	void addCharacterMapping(char c, const Glyph& glyph);
	const Glyph& getCharacterMapping(char c) const;

	Font(const Font&) = delete;
	Font& operator=(const Font&) = delete;
};