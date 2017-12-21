#include "Background.h"

#include "2D/Graphics/Graphics2D.h"
#include "2D/Graphics/Renderer/SceneRenderer.h"

Background::Background(Texture* texture, float32 xOffset) : m_texture(texture), m_xOffset(xOffset) {}


Background::~Background() {}

void Background::render() {
	getGraphics2DInstance()->getSceneRenderer().renderBackground(*this);
}