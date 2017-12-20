#include "Game.h"

#include "Window/Window.h"

#include "Console/Console.h"

#include "Core/Time.h"

Game::Game() : m_shutdown(false), m_fps(0) {}


Game::~Game() {}

void Game::start() {
	init();
	startLoop();
}

void Game::startLoop() {
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