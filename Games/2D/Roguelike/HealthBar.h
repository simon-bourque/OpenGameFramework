#pragma once
#if 0
#include "UI/UIComponent.h"

class Texture;
class RoguePlayer;

class HealthBar : public UIComponent {
	Texture* m_fullTex;
	Texture* m_emptyTex;

	RoguePlayer* m_player;
public:
	HealthBar(RoguePlayer* player);
	virtual ~HealthBar();

	virtual void render() override;
};

#endif