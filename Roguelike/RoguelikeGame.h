#pragma once

#include "Core/Game.h"

#include "Sound/SoundEngine.h"

class Font;
class Text;
class UserInterface;
class RoguePlayer;

class RoguelikeGame : public Game {
private:
	Font* m_mainFont;
	Text* m_testText;
	UserInterface* m_ui;

	RoguePlayer* m_player;
public:
	RoguelikeGame();
	virtual ~RoguelikeGame();

	virtual void init() override;
	virtual void render() override;
	virtual void tick(float32 delta) override;

	SoundEngine* soundEngine;

	void onKey(int32 key, int32 scancode, int32 action, int32 mods);
};

