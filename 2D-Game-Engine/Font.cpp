#include "Font.h"



Font::Font(Texture* bitmap, const Glyph& invalidCharacter) : m_bitmap(bitmap), m_invalidCharacter(invalidCharacter) {}

Font::Font(Texture* bitmap, const Glyph& invalidCharacter, const std::map<char, Glyph>& glyphMap) : m_bitmap(bitmap), m_invalidCharacter(invalidCharacter), m_glyphMap(glyphMap) {}

void Font::addCharacterMapping(char c, const Glyph& glyph) {
	m_glyphMap[c] = glyph;
}

const Glyph& Font::getCharacterMapping(char c) const {
	auto iter = m_glyphMap.find(c);

	return (iter == m_glyphMap.end()) ? m_invalidCharacter : iter->second;
}