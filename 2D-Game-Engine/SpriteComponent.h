#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include "RenderableComponent.h"

#include "Core.h"

class GameObject;
class Game;
class Texture;
class RenderSystem;

class SpriteComponent : public RenderableComponent {
private:
	Texture* m_texture;

	bool m_horizontalFlip;
	bool m_verticalFlip;
public:
	SpriteComponent(GameObject* parentObject, Texture* texture);
	virtual ~SpriteComponent();

	bool isHorizontallyFlipped() const { return m_horizontalFlip; };
	bool isVerticallyFlipped() const { return m_verticalFlip; };
	void setHorizontalFlip(bool horizontalFlip) { m_horizontalFlip = horizontalFlip; }
	void setVerticalFlip(bool verticalFlip) { m_verticalFlip = verticalFlip; }

	virtual void tick(float32 delta, Game* game) override;
	virtual void render(RenderSystem* rs) override;
};

#endif