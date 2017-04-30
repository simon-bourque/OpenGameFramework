#include "TextRenderer.h"

#include "ShaderProgram.h"
#include "Resources.h"
#include "Text.h"
#include "Font.h"
#include "Texture.h"
#include "VertexArrayObject.h"
#include "Matrix3f.h"
#include "Color.h"

#include <GL/glew.h>

TextRenderer::TextRenderer(RenderSystem* rs) : Renderer(rs) {
	m_textShaderProgram.reset(new ShaderProgram(loadSrc("text_vert.shader"), loadSrc("text_frag.shader")));
}

TextRenderer::~TextRenderer() {}

void TextRenderer::renderText(const Text* text, float32 x, float32 y, const Color& color) const {
	glUseProgram(m_textShaderProgram->getProgramId());

	text->getTextVAO().bind();

	Matrix3f modelMatrix = Matrix3f::translation(x, y) * Matrix3f::scale(1, 1);

	text->getFont().getBitmap()->bind();

	glUniformMatrix3fv(m_textShaderProgram->getUniform("modelMatrix").getLocation(), 1, true, modelMatrix.values);
	glUniform1i(m_textShaderProgram->getUniform("diffuseTexture").getLocation(), 0);
	glUniform4fv(m_textShaderProgram->getUniform("textColor").getLocation(), 1, color.values);

	glDrawArrays(GL_TRIANGLES, 0, 6 * text->getNumCharacters());
}