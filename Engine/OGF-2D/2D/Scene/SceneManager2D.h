#pragma once

#include "Core/Core.h"
#include "Core/Assert.h"
#include "Core/Singleton.h"

#include "2D/Scene/Scene2D.h"

#include <memory>

class SceneManager2D {
	SINGLETON_DECLARATION(SceneManager2D)
private:
	std::unique_ptr<Scene2D> m_currentScene;

	bool m_paused;
public:
	SceneManager2D();
	virtual ~SceneManager2D();

	void loadTileLevel(const string& file);
	void tickCurrentScene(float32 delta);
	void renderCurrentScene();

	const Scene2D& getCurrentScene() const { return *m_currentScene; };
	Scene2D& getCurrentScene() { return *m_currentScene; };

	bool isPaused() const { return m_paused; };
	void setPaused(bool paused) { m_paused = paused; };
};

SINGLETON_ACCESSOR(SceneManager2D)