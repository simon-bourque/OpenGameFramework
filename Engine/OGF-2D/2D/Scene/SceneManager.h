#pragma once

#include "Core/Core.h"
#include "Core/EngineAssert.h"
#include "Core/Singleton.h"

#include "2D/Scene/Scene.h"

#include <memory>

class Game;

class SceneManager {
	SINGLETON_DECLARATION(SceneManager)
private:
	std::unique_ptr<Scene> m_currentScene;

	bool m_paused;
public:
	SceneManager();
	virtual ~SceneManager();

	void loadTileLevel(const string& file);
	void tickCurrentScene(float32 delta);
	void renderCurrentScene();

	const Scene& getCurrentScene() const { return *m_currentScene; };
	Scene& getCurrentScene() { return *m_currentScene; };

	bool isPaused() const { return m_paused; };
	void setPaused(bool paused) { m_paused = paused; };
};

SINGLETON_ACCESSOR(SceneManager)