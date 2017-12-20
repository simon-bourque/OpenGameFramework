#pragma once

#include "Core/Core.h"

#include <memory>

class GraphicsContext;
class ShaderProgram;
class Text;
struct Color;

class TextRenderer {
private:
	std::unique_ptr<ShaderProgram> m_textShaderProgram;
public:
	TextRenderer();
	virtual ~TextRenderer();

	void renderText(const Text* text, float32 x, float32 y, const Color& color) const;
};