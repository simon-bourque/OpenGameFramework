#ifndef TEXTRENDERER_H
#define TEXTRENDERER_H

#include "Renderer.h"

#include "Core.h"

#include <memory>

class RenderSystem;
class ShaderProgram;
class Text;
struct Color;

class TextRenderer : public Renderer {
private:
	std::unique_ptr<ShaderProgram> m_textShaderProgram;
public:
	TextRenderer(RenderSystem* rs);
	virtual ~TextRenderer();

	void renderText(const Text* text, float32 x, float32 y, const Color& color) const;
};

#endif