#include "UIImage.h"

#include "Graphics/RenderSystem.h" 
#include "Graphics/Renderer/SpriteRenderer.h"


UIImage::UIImage(const Rectangle& bounds, Texture* texture) : UIComponent(bounds), m_texture(texture) {}
 
UIImage::~UIImage() {}

void UIImage::render() {
	RenderSystem::get()->getSpriteRenderer()->renderSpriteUI(m_bounds, m_texture, false, false);
}