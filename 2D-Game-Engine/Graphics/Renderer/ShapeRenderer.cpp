#include "ShapeRenderer.h"

#include "Graphics/Shader/ShaderProgram.h"
#include "Graphics/Memory/VertexArrayObject.h"
#include "Graphics/Memory/Buffer.h"
#include "Graphics/Color.h"
#include "Graphics/RenderSystem.h"

#include "Resource/Resources.h"

#include "Math/Matrix3f.h"
#include "Math/Vector2f.h"

#include <cmath>

#include "Math/Constants.h"

const uint32 ShapeRenderer::CIRCLE_DIVS = 128;

const float32 ShapeRenderer::FILLED_RECTANGLE_VERTS[8] = {
	-0.5f,-0.5f,
	-0.5f,0.5f,
	0.5f,-0.5f,
	0.5f,0.5f
};

const float32 ShapeRenderer::OUTLINE_RECTANGLE_VERTS[8] = {
	-0.5f,-0.5f,
	-0.5f,0.5f,
	0.5f,0.5f,
	0.5f,-0.5f
};

ShapeRenderer::ShapeRenderer(RenderSystem* rs) : Renderer(rs) {
	m_shapeShaderProgram.reset(new ShaderProgram(loadSrc("vertex/mvp_no_uv_vert.shader"), loadSrc("fragment/unlit_color_frag.shader")));

	initLineBuffers();
	initRectangleBuffers();
	initEllipseBuffers();
}

ShapeRenderer::~ShapeRenderer() {}

void ShapeRenderer::initLineBuffers() {
	m_lineVAO.reset(new VertexArrayObject());
	m_lineVAO->bind();

	m_lineBuffer = new Buffer(Buffer::Target::ARRAY_BUFFER);
	m_lineBuffer->bind();
	m_lineBuffer->bufferData(sizeof(float32) * 2 * 2, Buffer::Usage::DYNAMIC_DRAW); // 2 vertices * 2 floats per vertex * 4 bytes per float = 16 bytes
#ifdef DEBUG_BUILD
	m_lineBuffer->setDebugLabel("LINE_BUFFER");
#endif

	m_lineVAO->enableAttribute(0);
	m_lineBuffer->vertexAttributePointer(0, 2, GL_FLOAT);

	m_lineVAO->addArrayBuffer(m_lineBuffer);

	m_lineBuffer->unbind();
	m_lineVAO->unbind();
}

void ShapeRenderer::initRectangleBuffers() {
	// Filled Rectangle
	m_filledRectVAO.reset(new VertexArrayObject());
	m_filledRectVAO->bind();

	m_filledRectVAO->addArrayBuffer(0, FILLED_RECTANGLE_VERTS, sizeof(float32) * 8, 2, GL_FLOAT, Buffer::Usage::STATIC_DRAW);
	m_filledRectVAO->unbind();

	// Outline Rectangle
	m_outlineRectVAO.reset(new VertexArrayObject());
	m_outlineRectVAO->bind();

	m_outlineRectVAO->addArrayBuffer(0, OUTLINE_RECTANGLE_VERTS, sizeof(float32) * 8, 2, GL_FLOAT, Buffer::Usage::STATIC_DRAW);
	m_outlineRectVAO->unbind();

#ifdef DEBUG_BUILD
	m_filledRectVAO->getBuffer(0)->setDebugLabel("FILLED_RECT_BUFFER");
	m_outlineRectVAO->getBuffer(0)->setDebugLabel("OUTLINE_RECT_BUFFER");
#endif
}

void ShapeRenderer::initEllipseBuffers() {
	uint32 dataSize = 0;
	float32* data = generateCircleVertices(0.5f, CIRCLE_DIVS, dataSize);

	float32* buffer = new float32[dataSize + 4];
	buffer[0] = 0.0f;
	buffer[1] = 0.0f;

	for (uint32 i = 0; i < dataSize; i++) {
		buffer[2 + i] = data[i];
	}

	buffer[dataSize + 2] = data[0];
	buffer[dataSize + 3] = data[1];

	m_ellipseVAO.reset(new VertexArrayObject());
	m_ellipseVAO->bind();
	m_ellipseVAO->addArrayBuffer(0, buffer, sizeof(float32) * (dataSize + 4), 2, GL_FLOAT, Buffer::Usage::STATIC_DRAW);
	
#ifdef DEBUG_BUILD	
	m_ellipseVAO->getBuffer(0)->setDebugLabel("ELLIPSE_BUFFER");
#endif

	delete[] data;
	delete[] buffer;
	m_ellipseVAO->unbind();
}

float32* ShapeRenderer::generateCircleVertices(float32 radius, uint32 divs, uint32& dataSize) const {
	dataSize = divs * 2;
	float32* data = new float32[dataSize];

	for (uint32 i = 0; i < divs; i++) {
		float32 angle = (float32)((2 * PI * i) / ((float32)divs));

		float32 x = (float32)(radius * cos(angle));
		float32 y = (float32)(radius * sin(angle));

		data[i * 2] = x;
		data[(i * 2) + 1] = y;

	}

	return data;
}

void ShapeRenderer::loadLineData(float32 xi, float32 yi, float32 xf, float32 yf) {
	float32 verts[4] = { xi, yi, xf, yf };
	m_lineBuffer->bind();
	m_lineBuffer->bufferSubData(0, sizeof(float32) * 4, verts);
	m_lineBuffer->unbind();
}

void ShapeRenderer::drawVector(float32 x, float32 y, const Vector2f& vector, const Color& color) {
	drawLine(x, y, vector.x + x, vector.y + y, color);
}

void ShapeRenderer::drawLine(float32 xi, float32 yi, float32 xf, float32 yf, const Color& color) {
	glUseProgram(m_shapeShaderProgram->getProgramId());

	m_lineVAO->bind();

	// load line info into buffer
	loadLineData(xi, yi, xf, yf);

	// Set color uniform
	glUniform4fv(m_shapeShaderProgram->getUniform("color").getLocation(), 1, color.values);

	// Set mvpMatrix uniform
	glUniformMatrix3fv(m_shapeShaderProgram->getUniform("mvpMatrix").getLocation(), 1, true, m_rs->getCamera().getViewProjectionMatrix().values);

	// Draw line
	glDrawArrays(GL_LINES, 0, 2);
}

void ShapeRenderer::drawRectangle(const geo::Rectangle& rect, const Color& color, bool filled) {
	drawRectangle(rect.getX(), rect.getY(), rect.getWidth(), rect.getHeight(), color, filled);
}

void ShapeRenderer::drawRectangle(float32 x, float32 y, float32 width, float32 height, const Color& color, bool filled) {
	glUseProgram(m_shapeShaderProgram->getProgramId());

	if (filled) {
		m_filledRectVAO->bind();
	}
	else {
		m_outlineRectVAO->bind();
	}


	// Set color uniform
	glUniform4fv(m_shapeShaderProgram->getUniform("color").getLocation(), 1, color.values);

	Matrix3f finalMatrix = m_rs->getCamera().getViewProjectionMatrix() * Matrix3f::translation(x, y) * Matrix3f::scale(width, height);
	
	glUniformMatrix3fv(m_shapeShaderProgram->getUniform("mvpMatrix").getLocation(), 1, true, finalMatrix.values);

	glDrawArrays((filled) ? GL_TRIANGLE_STRIP : GL_LINE_LOOP, 0, 4); // Both outline and filled rectangle have same number of vertices
}

void ShapeRenderer::drawCircle(float32 x, float32 y, float32 radius, const Color& color, bool filled) {
	drawEllipse(x, y, 2 * radius, 2 * radius, color, filled);
}

void ShapeRenderer::drawEllipse(float32 x, float32 y, float32 width, float32 height, const Color& color, bool filled) {
	glUseProgram(m_shapeShaderProgram->getProgramId());

	m_ellipseVAO->bind();

	// Set color uniform
	glUniform4fv(m_shapeShaderProgram->getUniform("color").getLocation(), 1, color.values);

	// Set mvpMatrix uniform
	Matrix3f finalMatrix = m_rs->getCamera().getViewProjectionMatrix() * Matrix3f::translation(x, y) * Matrix3f::scale(width, height);

	glUniformMatrix3fv(m_shapeShaderProgram->getUniform("mvpMatrix").getLocation(), 1, true, finalMatrix.values);

	if (filled) {
		glDrawArrays(GL_TRIANGLE_FAN, 0, CIRCLE_DIVS + 2);
	}
	else {
		glDrawArrays(GL_LINE_STRIP, 1, CIRCLE_DIVS + 1);
	}
}