#pragma once

#include "2D/Game2D.h"
#include "Math/Vector2f.h"

class Texture;
class TileScene;
class AnimatorComponent;
class Player;

class PlatformerGame : public Game2D
{
private:
	bool up;
	bool down;
	bool left;
	bool right;

	Player* m_player;

	Vector2f m_playerSpawn;
public:
	PlatformerGame();
	virtual ~PlatformerGame();

	virtual void init() override;
	virtual void render() override;
	virtual void tick(float32 delta) override;

	void resetPlayer();

	void onKeyPress(int32 key, int32 scancode, int32 action, int32 mods);
};

