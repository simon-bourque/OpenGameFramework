#pragma once

#include "Core/Core.h"

#include "Graphics/Renderer/Renderer.h"

#include <memory>

class RenderSystem;
class ShaderProgram;
class VertexArrayObject;
class Buffer;
class Rectangle;
struct Vector2f;
struct Color;

class ShapeRenderer : Renderer {
public:
	static const uint32 CIRCLE_DIVS;
	static const float32 FILLED_RECTANGLE_VERTS[8];
	static const float32 OUTLINE_RECTANGLE_VERTS[8];
private:
	std::unique_ptr<ShaderProgram> m_shapeShaderProgram;

	std::unique_ptr<VertexArrayObject> m_lineVAO;
	Buffer* m_lineBuffer;

	std::unique_ptr<VertexArrayObject> m_filledRectVAO;
	std::unique_ptr<VertexArrayObject> m_outlineRectVAO;

	std::unique_ptr<VertexArrayObject> m_ellipseVAO;

	void initLineBuffers();
	void initRectangleBuffers();
	void initEllipseBuffers();
	float32* generateCircleVertices(float32 radius, uint32 divs, uint32& dataSize) const;
	void loadLineData(float32 xi, float32 yi, float32 xf, float32 yf);
public:
	ShapeRenderer(RenderSystem* rs);
	virtual ~ShapeRenderer();

	void drawVector(float32 x, float32 y, const Vector2f& vector, const Color& color);
	void drawLine(float32 xi, float32 yi, float32 xf, float32 yf, const Color& color);

	void drawRectangle(const Rectangle& rect, const Color& color, bool filled);
	void drawRectangle(float32 x, float32 y, float32 width, float32 height, const Color& color, bool filled);

	void drawCircle(float32 x, float32 y, float32 radius, const Color& color, bool filled);
	void drawEllipse(float32 x, float32 y, float32 width, float32 height, const Color& color, bool filled);
};