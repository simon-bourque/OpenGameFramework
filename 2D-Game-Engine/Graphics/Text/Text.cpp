#include "Text.h"

#include "Graphics/Memory/VertexArrayObject.h"
#include "Graphics/Memory/Buffer.h"

#include "Graphics/Text/Font.h"

Text::Text(const string& text, Font* font, Usage usage) : m_font(font), m_usage(usage), m_scale(1.0f) {
	m_numCharacters = text.length();

	m_textVAO.reset(new VertexArrayObject());
	m_textVAO->bind();

	m_vertexBuffer = new Buffer(Buffer::Target::ARRAY_BUFFER);
	m_uvBuffer = new Buffer(Buffer::Target::ARRAY_BUFFER);

	buildBuffers(text);

	m_vertexBuffer->bind();
	m_vertexBuffer->vertexAttributePointer(0, 2, GL_FLOAT);

	m_uvBuffer->bind();
	m_uvBuffer->vertexAttributePointer(1, 2, GL_FLOAT);

	m_uvBuffer->unbind();

#ifdef DEBUG_BUILD
	static uint32 labelCount = 0;

	string vertexLabel("TEXT_VERTEX_BUFFER_");
	string uvLabel("TEXT_UV_BUFFER_");

	vertexLabel.append(std::to_string(labelCount));
	uvLabel.append(std::to_string(labelCount));

	m_vertexBuffer->setDebugLabel(vertexLabel);
	m_uvBuffer->setDebugLabel(uvLabel);

	labelCount++;
#endif

	m_textVAO->enableAttribute(0);
	m_textVAO->enableAttribute(1);
	m_textVAO->addArrayBuffer(m_vertexBuffer);
	m_textVAO->addArrayBuffer(m_uvBuffer);
	m_textVAO->unbind();
}


Text::~Text() {}

void Text::buildBuffers(const string& text) {
	uint32 numData = m_numCharacters * 6 * 2;

	float32* vertexData = new float32[numData];
	float32* textCoordData = new float32[numData];

	float32 cursorX = 0;
	for (uint32 i = 0; i < m_numCharacters; i++) {
		const Glyph& glyph = m_font->getCharacterMapping(text[i]);

		float32 finalXOffset = cursorX + glyph.xOffset;
		float32 finalYOffset = -glyph.yOffset;

		vertexData[(i * 12)] = glyph.vertices[0] + finalXOffset;
		vertexData[(i * 12) + 1] = glyph.vertices[1] + finalYOffset;
		vertexData[(i * 12) + 2] = glyph.vertices[2] + finalXOffset;
		vertexData[(i * 12) + 3] = glyph.vertices[3] + finalYOffset;
		vertexData[(i * 12) + 4] = glyph.vertices[4] + finalXOffset;
		vertexData[(i * 12) + 5] = glyph.vertices[5] + finalYOffset;
		vertexData[(i * 12) + 6] = glyph.vertices[6] + finalXOffset;
		vertexData[(i * 12) + 7] = glyph.vertices[7] + finalYOffset;
		vertexData[(i * 12) + 8] = glyph.vertices[8] + finalXOffset;
		vertexData[(i * 12) + 9] = glyph.vertices[9] + finalYOffset;
		vertexData[(i * 12) + 10] = glyph.vertices[10] + finalXOffset;
		vertexData[(i * 12) + 11] = glyph.vertices[11] + finalYOffset;

		for (int32 j = 0; j < 12; j++) {
			textCoordData[(i * 12) + j] = glyph.textCoords[j];
		}
		cursorX += glyph.xAdvance;
	}

	m_vertexBuffer->bind();
	m_vertexBuffer->bufferData(vertexData, sizeof(float32) * numData, static_cast<Buffer::Usage>(m_usage));

	m_uvBuffer->bind();
	m_uvBuffer->bufferData(textCoordData, sizeof(float32) * numData, static_cast<Buffer::Usage>(m_usage));

	m_uvBuffer->unbind();

	delete[] vertexData;
	delete[] textCoordData;
}

void Text::setText(const string& text) {
	m_numCharacters = text.length();

	buildBuffers(text);
}