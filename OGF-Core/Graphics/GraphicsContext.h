#pragma once

#include "Core/Core.h"
#include "Core/Singleton.h"

#include <memory>

class TextRenderer;

class RawImage;

class TextureManager;
class FontManager;
class TextManager;

struct Color;

class GraphicsContext {
	SINGLETON_DECLARATION(GraphicsContext)
private:
	
	std::unique_ptr<TextRenderer> m_textRenderer;

	std::unique_ptr<TextureManager> m_textureManager;
	std::unique_ptr<FontManager> m_fontManager;
	std::unique_ptr<TextManager> m_textManager;

	explicit GraphicsContext();
public:
	~GraphicsContext();

	TextRenderer* getTextRenderer() const { return m_textRenderer.get(); };

	TextureManager* getTextureManager() const { return m_textureManager.get(); };
	FontManager* getFontManager() const { return m_fontManager.get(); };
	TextManager* getTextManager() const { return m_textManager.get(); };

	void setClearColor(const Color& color);
	void setClearColor(float32 r, float32 g, float32 b, float32 a = 1.0f);
};

SINGLETON_ACCESSOR(GraphicsContext)