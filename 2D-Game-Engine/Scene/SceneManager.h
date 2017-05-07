#pragma once
#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "Core/Core.h"

#include "Scene/Scene.h"

#include <memory>

class Game;

class SceneManager
{
private:

	std::unique_ptr<Scene> m_currentScene;

public:
	SceneManager();
	virtual ~SceneManager();

	void loadTileLevel(const string& file, Game* game);
	void tickCurrentScene(float32 delta, Game* game);
	void renderCurrentScene(RenderSystem* rs);

	const Scene& getCurrentScene() const { return *m_currentScene; };
	Scene& getCurrentScene() { return *m_currentScene; };
};

#endif