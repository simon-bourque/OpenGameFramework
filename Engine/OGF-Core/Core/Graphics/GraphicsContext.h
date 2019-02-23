#pragma once

#include "Core/Core.h"
#include "Core/Singleton.h"

#include <memory>

class TextRenderer;

class RawImage;

class TextureCache;
class FramebufferCache;

class FontManager;
class TextManager;

struct Color;

class GraphicsContext {
	SINGLETON_DECLARATION(GraphicsContext)
private:
	
	std::unique_ptr<TextRenderer> m_textRenderer;

	std::unique_ptr<TextureCache> m_textureCache;
	std::unique_ptr<FramebufferCache> m_framebufferCache;
	std::unique_ptr<FontManager> m_fontManager;
	std::unique_ptr<TextManager> m_textManager;

	explicit GraphicsContext();
public:
	~GraphicsContext();

	TextRenderer* getTextRenderer() const { return m_textRenderer.get(); }

	FramebufferCache* getFramebufferCache() const { return m_framebufferCache.get(); }
	TextureCache* getTextureCache() const { return m_textureCache.get(); }
	FontManager* getFontManager() const { return m_fontManager.get(); }
	TextManager* getTextManager() const { return m_textManager.get(); }

	void setClearColor(const Color& color);
	void setClearColor(float32 r, float32 g, float32 b, float32 a = 1.0f);
};

SINGLETON_ACCESSOR(GraphicsContext)