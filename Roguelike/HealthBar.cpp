#include "HealthBar.h"

#include "Graphics/RenderSystem.h"
#include "Graphics/TextureManager.h"
#include "Graphics/Renderer/SpriteRenderer.h"

#include "RoguePlayer.h"

HealthBar::HealthBar(RoguePlayer* player) : UIComponent(geo::Rectangle(-0.93f, 0.93f, 0.1f, 0.1f)), m_player(player) {
	m_fullTex = RenderSystem::get()->getTextureManager()->createTexture2D("heart_full.tx", Texture::Filter::NEAREST_NEIGHBOR);
	m_emptyTex = RenderSystem::get()->getTextureManager()->createTexture2D("heart_empty.tx", Texture::Filter::NEAREST_NEIGHBOR);
}


HealthBar::~HealthBar() {}

void HealthBar::render() {
	int32 maxHealth = m_player->getMaxHealth();
	int32 currentHealth = m_player->getCurrentHealth();

	for (uint32 i = 0; i < maxHealth; i++) {
		geo::Rectangle bounds = m_bounds;
		bounds.setX(bounds.getX() + (i * 0.1f));

		RenderSystem::get()->getSpriteRenderer()->renderSpriteUI(bounds, (i < currentHealth) ? m_fullTex : m_emptyTex, false, false);
	}
}
