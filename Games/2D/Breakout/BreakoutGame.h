#pragma once

#include "2D/Game2D.h"

class Ball;
class Powerup;
class Level;
class Paddle;
class Font;
class Text;

class BreakoutGame : public Game2D
{
public:
	BreakoutGame();
	~BreakoutGame();

	virtual void init() override;
	virtual void render() override;
	virtual void tick(float32 delta) override;

	void onKeyPress(int32 key, int32 scancode, int32 action, int32 mods);

private:

	Text* m_startText = nullptr;
	Text* m_optionsText = nullptr;

	Font* m_font = nullptr;
	Paddle* m_paddle = nullptr;
	Ball* m_ball = nullptr;
	Powerup* m_powerup = nullptr;
	Level* m_level = nullptr;
};

