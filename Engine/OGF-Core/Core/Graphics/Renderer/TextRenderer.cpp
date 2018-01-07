#include "TextRenderer.h"

#include "Core/Graphics/GraphicsContext.h"
#include "Core/Graphics/Shader/ShaderProgram.h"
#include "Core/Graphics/Text/Text.h"
#include "Core/Graphics/Text/Font.h"
#include "Core/Graphics/Memory/VertexArrayObject.h"
#include "Core/Graphics/Texture.h"
#include "Core/Graphics/Color.h"

#include "Core/Graphics/TextureCache.h"

#include "Core/Resource/Resources.h"

#include "Core/Math/Matrix3f.h"

#include <GL/glew.h>

TextRenderer::TextRenderer() {
	m_textShaderProgram.reset(new ShaderProgram(loadSrc("text_vert.shader"), loadSrc("text_frag.shader")));
}

TextRenderer::~TextRenderer() {}

void TextRenderer::renderText(const Text* text, float32 x, float32 y, const Color& color) const {
	glUseProgram(m_textShaderProgram->getProgramId());

	text->getTextVAO().bind();

	Matrix3f modelMatrix = Matrix3f::translation(x, y) * Matrix3f::scale(text->getScale(), text->getScale());

	TextureRef textureRef = text->getFont().getBitmap();
	getGraphicsContextInstance()->getTextureCache()->getTexture(textureRef)->bind(Texture::Unit::UNIT_0);

	glUniformMatrix3fv(m_textShaderProgram->getUniform("modelMatrix").getLocation(), 1, true, modelMatrix.values);
	glUniform1i(m_textShaderProgram->getUniform("diffuseTexture").getLocation(), 0);
	glUniform4fv(m_textShaderProgram->getUniform("textColor").getLocation(), 1, color.values);

	glDrawArrays(GL_TRIANGLES, 0, 6 * text->getNumCharacters());
}