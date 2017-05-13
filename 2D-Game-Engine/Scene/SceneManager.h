#pragma once
#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "Core/Core.h"
#include "Core/EngineAssert.h"

#include "Scene/Scene.h"

#include <memory>

class Game;

class SceneManager
{
private:
	static SceneManager* s_instance;

	std::unique_ptr<Scene> m_currentScene;

public:
	SceneManager();
	virtual ~SceneManager();

	void loadTileLevel(const string& file);
	void tickCurrentScene(float32 delta);
	void renderCurrentScene();

	const Scene& getCurrentScene() const { return *m_currentScene; };
	Scene& getCurrentScene() { return *m_currentScene; };

	static SceneManager* get() {
		ASSERT(s_instance, "SceneManager must be initialized before use.");
		return s_instance;
	};

	static void init() {
		ASSERT(!s_instance, "SceneManager is already initialized.");
		s_instance = new SceneManager();
	}

	static void destroy() {
		delete s_instance;
	}
};

#endif