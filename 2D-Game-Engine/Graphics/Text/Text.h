#pragma once
#ifndef TEXT_H
#define TEXT_H

#include "Core/Core.h"

#include <GL/glew.h>

#include <memory>

class Font;
class VertexArrayObject;
class Buffer;

class Text {
public:
	enum class Usage : GLenum {
		STREAM = GL_STREAM_DRAW,
		STATIC = GL_STATIC_DRAW,
		DYNAMIC = GL_DYNAMIC_DRAW,
	};
private:
	uint32 m_numCharacters;
	
	Font* m_font;

	std::unique_ptr<VertexArrayObject> m_textVAO;
	Buffer* m_vertexBuffer;
	Buffer* m_uvBuffer;

	Usage m_usage;

	float32 m_scale;

	void buildBuffers(const string& text);
public:
	Text(const string& text, Font* font, Usage usage);
	virtual ~Text();

	void setText(const string& text);

	Text(const Text&) = delete;
	Text& operator=(const Text&) = delete;

	const Font& getFont() const { return *m_font; };
	const VertexArrayObject& getTextVAO() const { return *m_textVAO; };
	uint32 getNumCharacters() const { return m_numCharacters; };
	
	float32 getScale() const { return m_scale; };
	void setScale(float32 scale) { m_scale = scale; };
};

#endif