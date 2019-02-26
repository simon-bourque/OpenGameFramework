#pragma once

#include "Powerup.h"

#include "Core/Math/Vector2f.h"

#include "2D/Object/GameObject.h"

#include <vector>
#include <string>

class Brick;
class Powerup;

class Level : public GameObject
{
public:

	enum class Template {
		LEVEL0,
		LEVEL1,
		LEVEL2,
		LEVEL3
	};

	Level();
	Level(Template level);
	~Level();
	Level(const Level& other) = delete;
	Level(Level&& other) = delete;
	Level& operator=(const Level& other) = delete;
	Level& operator=(Level&& other) = delete;

	virtual void render() override;

	// Tell the level it should spawn a powerup at a certain position
	void spawnPowerup(Vector2f position);

	bool attemptPowerupConsume(); 
	Powerup::PowerType getPowerupType() const { return m_powerup->getType(); }

	// Set new current level and rebuild level
	void changeLevel(Template level);

	// Called when paddle catches rebuild powerup
	void resetLevel();

private:
	// Level resource path and names
	const static std::string LEVEL_PATH;
	const static std::string LEVEL1_NAME;
	const static std::string LEVEL2_NAME;
	const static std::string LEVEL3_NAME;

	// Fill bricks vector depending on current level
	void populate();

	Template m_currentLevel;
	std::vector<Brick*> m_bricksInLevel;
	Powerup* m_powerup = nullptr;

};

