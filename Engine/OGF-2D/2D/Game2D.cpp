#include "Game2D.h"

#include "Core/Singleton.h"
#include "Core/Platform.h"

#include "Console/Console.h"

#include "Window/Window.h"

#include "Input/Input.h"

#include "Graphics/GraphicsContext.h"
#include "2D/Graphics/Graphics2D.h"

#include "2D/Scene/SceneManager.h"

#include "2D/Physics/Collision/CollisionSystem.h"

#include "2D/Debug.h"

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
	Singleton<SceneManager>::init();
//
//	//DEBUG_LOG("Initializing sound engine...");
//	//SoundEngine::init();
//
#ifdef DEBUG_BUILD
	Singleton<Debug>::init();
#endif
}

Game2D::~Game2D() {
	DEBUG_LOG("Destroying game");
#ifdef DEBUG_BUILD
	Singleton<Debug>::destroy();
#endif
//	//SoundEngine::destroy();
	Singleton<SceneManager>::destroy();
	Singleton<Graphics2D>::destroy();
	Singleton<GraphicsContext>::destroy();
	Singleton<Input>::destroy();
	Singleton<Window>::destroy();
	Singleton<Console>::destroy();
}

void Game2D::tick(float32 deltaSeconds) {
	getSceneManagerInstance()->tickCurrentScene(deltaSeconds);
	getSceneManagerInstance()->getCurrentScene().getCollisionSystem()->narrowScan();
#ifdef DEBUG_BUILD
	getDebugInstance()->tick(m_fps);
#endif
}

void Game2D::render() {
	glClear(GL_COLOR_BUFFER_BIT);
	getGraphics2DInstance()->getCamera().updateViewProjectionMatrix();
	getSceneManagerInstance()->renderCurrentScene();

#ifdef DEBUG_BUILD
	getDebugInstance()->render();
#endif
}