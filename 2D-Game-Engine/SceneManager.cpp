#include "SceneManager.h"

#include "Resources.h"
#include "TileScene.h"

SceneManager::SceneManager() {
	m_currentScene.reset(new Scene(Rectangle(100,100)));
}


SceneManager::~SceneManager() {}

void SceneManager::loadTileLevel(const string& file, Game* game) {
	TileScene* scene = ::loadTileLevel(file, game);
	m_currentScene.reset(static_cast<Scene*>(scene));
}

void SceneManager::tickCurrentScene(float32 delta, Game* game) {
	m_currentScene->tick(delta, game);
}

void SceneManager::renderCurrentScene(const RenderSystem& rs) {
	m_currentScene->render(rs);
}
