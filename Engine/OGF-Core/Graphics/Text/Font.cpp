#include "Font.h"


Font::Font(Texture* bitmap, const Glyph& invalidCharacter) : m_bitmap(bitmap), m_invalidCharacter(invalidCharacter) {}

Font::Font(Texture* bitmap, const Glyph& invalidCharacter, std::pair<char, Glyph>* characterMap, uint32 characterMapSize) : m_bitmap(bitmap), m_invalidCharacter(invalidCharacter) {
	for (uint32 i = 0; i < characterMapSize; i++) {
		m_glyphMap[characterMap[i].first] = characterMap[i].second;
	}
}

void Font::addCharacterMapping(char c, const Glyph& glyph) {
	m_glyphMap[c] = glyph;
}

const Glyph& Font::getCharacterMapping(char c) const {
	auto iter = m_glyphMap.find(c);

	return (iter == m_glyphMap.end()) ? m_invalidCharacter : iter->second;
}