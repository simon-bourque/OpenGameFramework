#ifndef FONT_H
#define FONT_H

#include "Core.h"

#include <map>
#include <utility>
#include "Glyph.h"

class Texture;

class Font {
private:
	Texture* m_bitmap;
	std::map<char, Glyph> m_glyphMap;
	Glyph m_invalidCharacter;
public:
	Font(Texture* bitmap, const Glyph& invalidCharacter);
	Font(Texture* bitmap, const Glyph& invalidCharacter, std::pair<char, Glyph>* characterMap, uint32 characterMapSize);

	Texture* getBitmap() const { return m_bitmap; };

	void addCharacterMapping(char c, const Glyph& glyph);
	const Glyph& getCharacterMapping(char c) const;

	Font(const Font&) = delete;
	Font& operator=(const Font&) = delete;
};

#endif