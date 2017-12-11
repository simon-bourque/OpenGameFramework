#include "Background.h"

#include "Graphics/RenderSystem.h"
#include "Graphics/Renderer/SceneRenderer.h"

Background::Background(Texture* texture, float32 xOffset) : m_texture(texture), m_xOffset(xOffset) {}


Background::~Background() {}

void Background::render() {
	getRenderSystemInstance()->getSceneRenderer()->renderBackground(*this);
}