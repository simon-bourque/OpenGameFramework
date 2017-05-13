#ifdef DEBUG_BUILD

#pragma once
#ifndef DEBUG_H
#define DEBUG_H

#include "Core/Core.h"
#include "Core/EngineAssert.h"

#include <memory>

class Game;
class Font;
class Text;

class Debug {
private:
	static Debug* s_instance;

	Text* m_fpsText;
	Text* m_debugOnText;

	bool m_renderColliders;

	bool m_debugMode;
	bool m_renderPerf;
	bool m_renderBounds;
	bool m_renderQuadTree;
	bool m_renderGrid;

	bool m_zoomIn;
	bool m_zoomOut;

	void renderPerf() const;
	void renderBounds() const;
	void renderQuadTree() const;
	void renderGrid() const;
	void onKeyPress(int32 key, int32 scancode, int32 action, int32 mods);

	Debug();
public:
	virtual ~Debug();

	void tick(int32 fps);
	void render();

	bool shouldRenderColliders() const { return m_renderColliders && m_debugMode; };

	Debug(const Debug&) = delete;
	Debug& operator=(const Debug&) = delete;

	static Debug* get() {
		ASSERT(s_instance, "Debug must be initialized before use.");
		return s_instance;
	};

	static void init() {
		ASSERT(!s_instance, "Debug is already initialized.");
		s_instance = new Debug();
	}

	static void destroy() {
		delete s_instance;
	}
};

#endif
#endif