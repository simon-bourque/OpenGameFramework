#include "Game.h"

#include "Graphics/RenderSystem.h"
#include "Graphics/Camera.h"

#include "Window/Window.h"

#include "Math/Geometry/Rectangle.h"

#include "Input/Input.h"

#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Physics/Collision/CollisionSystem.h"

#include "Console/Console.h"

#include "Core/Platform.h"

#include "Core/Time.h"

#include "Core/Singleton.h"

#include <gl/glew.h>

#ifdef DEBUG_BUILD
#include "Core/Debug.h"
#endif

Game::Game(const string& title, int32 width, int32 height, const geo::Rectangle& viewPort) : m_shutdown(false), m_fps(0) {
	Singleton<Console>::init();

	DEBUG_LOG("Initializing game...");
	printSystemInfo();

	DEBUG_LOG("Initializing window...");
	//Window::init(title, width, height);
	Singleton<Window>::init(title, width, height);
	Singleton<Input>::init();

	DEBUG_LOG("Initializing render system...");
	Singleton<RenderSystem>::init(Camera(viewPort));

	DEBUG_LOG("Initializing scene manager...");
	Singleton<SceneManager>::init();

	//DEBUG_LOG("Initializing sound engine...");
	//SoundEngine::init();

#ifdef DEBUG_BUILD
	Singleton<Debug>::init();
#endif
}


Game::~Game() {
	DEBUG_LOG("Destroying game");
#ifdef DEBUG_BUILD
	Singleton<Debug>::destroy();
#endif
	//SoundEngine::destroy();
	Singleton<SceneManager>::destroy();
	Singleton<RenderSystem>::destroy();
	Singleton<Input>::destroy();
	Singleton<Window>::destroy();
	Singleton<Console>::destroy();
}

void Game::run() {
	
	init();

	int32 frames = 0;
	float64 counter = 0;
	float64 delta = 0;
	float64 currentTime = 0;

	DEBUG_LOG("Entering loop");

	while (!getWindowInstance()->shouldClose() && !m_shutdown) {


		getWindowInstance()->pollEvents();
		getConsoleInstance()->pollEvents();

		currentTime = getCurrentTime();

		tick((float32)delta);

		render();


		getWindowInstance()->swapBuffers();
		delta = getCurrentTime() - currentTime;

		counter += delta;
		if (counter >= 1) {
			counter = 0;
			m_fps = frames;
			frames = 0;
		}
		else {
			frames++;
		}
	}
	DEBUG_LOG("Shutting down");
}

void Game::tick(float32 delta) {
	getSceneManagerInstance()->tickCurrentScene(delta);
	getSceneManagerInstance()->getCurrentScene().getCollisionSystem()->narrowScan();
#ifdef DEBUG_BUILD
	getDebugInstance()->tick(m_fps);
#endif
}

void Game::render() {
	glClear(GL_COLOR_BUFFER_BIT);
	getRenderSystemInstance()->getCamera().updateViewProjectionMatrix();
	getSceneManagerInstance()->renderCurrentScene();

#ifdef DEBUG_BUILD
	getDebugInstance()->render();
#endif
}