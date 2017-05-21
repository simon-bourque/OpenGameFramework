#include "InventoryUI.h"

#include "Graphics/RenderSystem.h"
#include "Graphics/TextureManager.h"
#include "Graphics/Renderer/SpriteRenderer.h"
#include "Graphics/Texture.h"

#include "Math/Geometry/Rectangle.h"

InventoryUI::InventoryUI() : UIComponent(Rectangle(1,1)), m_width(4), m_height(3) {
	//Equipment
	m_ui_helm = RenderSystem::get()->getTextureManager()->createTexture2D("ui_helm.tx", Texture::Filter::NEAREST_NEIGHBOR);
	m_ui_chest = RenderSystem::get()->getTextureManager()->createTexture2D("ui_chest.tx", Texture::Filter::NEAREST_NEIGHBOR);
	m_ui_glove = RenderSystem::get()->getTextureManager()->createTexture2D("ui_glove.tx", Texture::Filter::NEAREST_NEIGHBOR);
	m_ui_empty = RenderSystem::get()->getTextureManager()->createTexture2D("ui_empty.tx", Texture::Filter::NEAREST_NEIGHBOR);
	m_ui_boot = RenderSystem::get()->getTextureManager()->createTexture2D("ui_boot.tx", Texture::Filter::NEAREST_NEIGHBOR);
	m_ui_amulet = RenderSystem::get()->getTextureManager()->createTexture2D("ui_amulet.tx", Texture::Filter::NEAREST_NEIGHBOR);
	m_ui_bag = RenderSystem::get()->getTextureManager()->createTexture2D("ui_bag.tx", Texture::Filter::NEAREST_NEIGHBOR);

	//Inventory
	m_ui_tl = RenderSystem::get()->getTextureManager()->createTexture2D("ui_tl.tx", Texture::Filter::NEAREST_NEIGHBOR);
	m_ui_t = RenderSystem::get()->getTextureManager()->createTexture2D("ui_t.tx", Texture::Filter::NEAREST_NEIGHBOR);
	m_ui_tr = RenderSystem::get()->getTextureManager()->createTexture2D("ui_tr.tx", Texture::Filter::NEAREST_NEIGHBOR);
	m_ui_cl = RenderSystem::get()->getTextureManager()->createTexture2D("ui_cl.tx", Texture::Filter::NEAREST_NEIGHBOR);
	m_ui_c = RenderSystem::get()->getTextureManager()->createTexture2D("ui_c.tx", Texture::Filter::NEAREST_NEIGHBOR);
	m_ui_cr = RenderSystem::get()->getTextureManager()->createTexture2D("ui_cr.tx", Texture::Filter::NEAREST_NEIGHBOR);
	m_ui_bl = RenderSystem::get()->getTextureManager()->createTexture2D("ui_bl.tx", Texture::Filter::NEAREST_NEIGHBOR);
	m_ui_b = RenderSystem::get()->getTextureManager()->createTexture2D("ui_b.tx", Texture::Filter::NEAREST_NEIGHBOR);
	m_ui_br = RenderSystem::get()->getTextureManager()->createTexture2D("ui_br.tx", Texture::Filter::NEAREST_NEIGHBOR);
}


InventoryUI::~InventoryUI() {}

void InventoryUI::render() {
	const static float32 EQUIPSIZE = 0.1875f;
	const static float32 SIZE = 0.25f;

	float32 xi = ((SIZE * m_width) / -2.0f) + (SIZE / 2.0f);
	float32 yi = ((SIZE * m_height) / 2.0f) - (SIZE / 2.0f);

	for (uint32 row = 0; row < m_height; row++) {
		for (uint32 column = 0; column < m_width; column++) {
			if (row == 0) {
				if (column == 0) {
					RenderSystem::get()->getSpriteRenderer()->renderSpriteUI(Rectangle(xi + (column * SIZE), yi + (row * -SIZE), SIZE, SIZE), m_ui_tl, false, false);
				}
				else if (column == m_width - 1) {
					RenderSystem::get()->getSpriteRenderer()->renderSpriteUI(Rectangle(xi + (column * SIZE), yi + (row * -SIZE), SIZE, SIZE), m_ui_tr, false, false);
				}
				else {
					RenderSystem::get()->getSpriteRenderer()->renderSpriteUI(Rectangle(xi + (column * SIZE), yi + (row * -SIZE), SIZE, SIZE), m_ui_t, false, false);
				}
			}
			else if (row == m_height - 1) {
				if (column == 0) {
					RenderSystem::get()->getSpriteRenderer()->renderSpriteUI(Rectangle(xi + (column * SIZE), yi + (row * -SIZE), SIZE, SIZE), m_ui_bl, false, false);
				}
				else if (column == m_width - 1) {
					RenderSystem::get()->getSpriteRenderer()->renderSpriteUI(Rectangle(xi + (column * SIZE), yi + (row * -SIZE), SIZE, SIZE), m_ui_br, false, false);
				}
				else {
					RenderSystem::get()->getSpriteRenderer()->renderSpriteUI(Rectangle(xi + (column * SIZE), yi + (row * -SIZE), SIZE, SIZE), m_ui_b, false, false);
				}
			}
			else {
				if (column == 0) {
					RenderSystem::get()->getSpriteRenderer()->renderSpriteUI(Rectangle(xi + (column * SIZE), yi + (row * -SIZE), SIZE, SIZE), m_ui_cl, false, false);
				}
				else if (column == m_width - 1) {
					RenderSystem::get()->getSpriteRenderer()->renderSpriteUI(Rectangle(xi + (column * SIZE), yi + (row * -SIZE), SIZE, SIZE), m_ui_cr, false, false);
				}
				else {
					RenderSystem::get()->getSpriteRenderer()->renderSpriteUI(Rectangle(xi + (column * SIZE), yi + (row * -SIZE), SIZE, SIZE), m_ui_c, false, false);
				}
			}
		}
	}

	RenderSystem::get()->getSpriteRenderer()->renderSpriteUI(Rectangle(xi - ((SIZE + EQUIPSIZE) / 2.0f), yi + ((SIZE - EQUIPSIZE) / 2.0f), EQUIPSIZE, EQUIPSIZE), m_ui_helm, false, false);
	RenderSystem::get()->getSpriteRenderer()->renderSpriteUI(Rectangle(xi - ((SIZE + EQUIPSIZE) / 2.0f), yi + ((SIZE - EQUIPSIZE) / 2.0f) - EQUIPSIZE, EQUIPSIZE, EQUIPSIZE), m_ui_chest, false, false);
	RenderSystem::get()->getSpriteRenderer()->renderSpriteUI(Rectangle(xi - ((SIZE + EQUIPSIZE) / 2.0f), yi + ((SIZE - EQUIPSIZE) / 2.0f) - 2*EQUIPSIZE, EQUIPSIZE, EQUIPSIZE), m_ui_glove, false, false);
	RenderSystem::get()->getSpriteRenderer()->renderSpriteUI(Rectangle(xi - ((SIZE + EQUIPSIZE) / 2.0f), yi + ((SIZE - EQUIPSIZE) / 2.0f) - 3*EQUIPSIZE, EQUIPSIZE, EQUIPSIZE), m_ui_boot, false, false);
}