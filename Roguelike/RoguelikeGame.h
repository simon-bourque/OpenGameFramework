#pragma once

#include "Core/Game.h"

class Font;
class Text;

class RoguelikeGame : public Game {
private:
	Font* m_mainFont;
	Text* m_testText;
public:
	RoguelikeGame();
	virtual ~RoguelikeGame();

	virtual void init() override;
	virtual void render() override;
	virtual void tick(float32 delta) override;
};

