#include "Game2D.h"

#include "Core/Singleton.h"
#include "Core/Platform.h"

#include "Core/Console/Console.h"

#include "Core/Window/Window.h"

#include "Core/Input/Input.h"

#include "Core/Threads/ThreadPool.h"

#include "Core/Graphics/GraphicsContext.h"
#include "2D/Graphics/Graphics2D.h"

#include "2D/Scene/SceneManager2D.h"

#include "2D/Physics/Collision/CollisionSystem.h"

#include "2D/DebugOLD.h"

#include <GL/glew.h>

Game2D::Game2D(const string& title, int32 width, int32 height, const geo::Rectangle& viewPort) {
	Singleton<Console>::init();

	DEBUG_LOG("Initializing game...");
	printSystemInfo();

	DEBUG_LOG("Initializing window...");
	Singleton<Window>::init(title, width, height);
	Singleton<Input>::init();

	DEBUG_LOG("Initializing graphics...");
	Singleton<GraphicsContext>::init();
	Singleton<Graphics2D>::init(Camera(viewPort));

	DEBUG_LOG("Initializing scene manager...");
	Singleton<SceneManager2D>::init();

	DEBUG_LOG("Initializing thread pool...");
	Singleton<ThreadPool>::init();
//
//	//DEBUG_LOG("Initializing sound engine...");
//	//SoundEngine::init();
//
#ifdef DEBUG_BUILD
	Singleton<DebugOLD>::init();
#endif
}

Game2D::~Game2D() {
	DEBUG_LOG("Destroying game");
#ifdef DEBUG_BUILD
	Singleton<DebugOLD>::destroy();
#endif
//	//SoundEngine::destroy();
	Singleton<SceneManager2D>::destroy();
	Singleton<Graphics2D>::destroy();
	Singleton<GraphicsContext>::destroy();
	Singleton<Input>::destroy();
	Singleton<Window>::destroy();
	Singleton<Console>::destroy();
}

void Game2D::tick(float32 deltaSeconds) {
	getSceneManager2DInstance()->tickCurrentScene(deltaSeconds);
	getSceneManager2DInstance()->getCurrentScene().getCollisionSystem()->narrowScan();
#ifdef DEBUG_BUILD
	getDebugOLDInstance()->tick(m_fps);
#endif
}

void Game2D::render() {
	glClear(GL_COLOR_BUFFER_BIT);
	getGraphics2DInstance()->getCamera().updateViewProjectionMatrix();
	getSceneManager2DInstance()->renderCurrentScene();

#ifdef DEBUG_BUILD
	getDebugOLDInstance()->render();
#endif
}