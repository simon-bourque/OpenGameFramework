#include "Level.h"
#include "Brick.h"
#include "Powerup.h"

#include "Core/Assert.h"
#include "Core/Threads/ThreadPool.h"

#include "2D/Scene/SceneManager2D.h"

#include <fstream>
#include <future>

const std::string Level::LEVEL_PATH = "res/levels/";
const std::string Level::LEVEL1_NAME = "level1.txt";
const std::string Level::LEVEL2_NAME = "level2.txt";
const std::string Level::LEVEL3_NAME = "level3.txt";

Level::Level()
	: m_currentLevel(Template::LEVEL0)
{
	// Only one powerup per scene at a time
	m_powerup = new Powerup();
	getSceneManager2DInstance()->getCurrentScene().addGameObject(m_powerup);

	populate();
}

Level::Level(Template level)
	: m_currentLevel(level)
{
	// Only one powerup per scene at a time
	m_powerup = new Powerup();
	getSceneManager2DInstance()->getCurrentScene().addGameObject(m_powerup);

	populate();
}

void Level::changeLevel(Template level)
{
	m_bricksInLevel.clear();
	m_currentLevel = level;
	
	populate();
}

void Level::resetLevel()
{
	std::vector<std::future<void>> futures;
	for (auto& brick : m_bricksInLevel)
	{
		// Submit reset job for each brick
		futures.push_back(Singleton<ThreadPool>::get()->submit([brick]() { brick->reset(); }));
	}

	// Make sure all jobs are done
	for (auto& future : futures) future.get();
}

void Level::spawnPowerup(Vector2f position)
{
	m_powerup->spawnRandomType(position);
}

bool Level::attemptPowerupConsume()
{
	return m_powerup->consume();
}

void Level::populate()
{
	std::fstream levelFile;

	switch (m_currentLevel)
	{
	case Template::LEVEL0: // Empty level
		break;
	case Template::LEVEL1:
		levelFile.open(LEVEL_PATH + LEVEL1_NAME);
		OGF_ASSERT(levelFile.is_open(), "Level 1 could not be found");
		break;
	case Template::LEVEL2:
		levelFile.open(LEVEL_PATH + LEVEL2_NAME);
		OGF_ASSERT(levelFile.is_open(), "Level 2 could not be found");
		break;
	case Template::LEVEL3:
		levelFile.open(LEVEL_PATH + LEVEL3_NAME);
		OGF_ASSERT(levelFile.is_open(), "Level 3 could not be found");
		break;
	}

	while (levelFile.peek() != EOF)
	{
		float32 xPos, yPos = 0;
		levelFile >> xPos;
		levelFile >> yPos;

		xPos += Brick::BRICK_WIDTH / 2; // Offsetting based on brick dimensions
		yPos -= Brick::BRICK_HEIGHT / 2;

		Brick* brick = new Brick(this, Brick::BrickType::NORMAL, { xPos, yPos });
		getSceneManager2DInstance()->getCurrentScene().addGameObject(brick);

		m_bricksInLevel.push_back(brick);
	}

	levelFile.close();
}

void Level::render()
{
	for (auto& brick : m_bricksInLevel)
	{
		if (brick && brick->isVisible())
		{
			brick->render();
		}
	}
}


Level::~Level()
{
}
