#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <memory>
#include "Scene.h"

#include "Core.h"

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
	void renderCurrentScene(const RenderSystem& rs);

	const Scene& getCurrentScene() const { return *m_currentScene; };
	Scene& getCurrentScene() { return *m_currentScene; };
};

#endif