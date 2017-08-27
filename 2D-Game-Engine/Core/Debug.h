#ifdef DEBUG_BUILD

#pragma once

#include "Core/Core.h"
#include "Core/EngineAssert.h"

#include <memory>
#include <unordered_map>

class Game;
class Font;
class Text;

class Debug {
private:
	static Debug* s_instance;

	std::unordered_map<string, bool> m_debugFlags;

	Text* m_fpsText;

	bool m_zoomIn;
	bool m_zoomOut;

	void renderPerf() const;
	void renderBounds() const;
	void renderQuadTree() const;
	void renderGrid() const;

	void onKeyPress(int32 key, int32 scancode, int32 action, int32 mods);
	void onMouseMove(float64 xPos, float64 yPos);
	void onMousePress(int32 button, int32 action, int32 mods);
	void onMouseScroll(float64 xOffset, float64 yOffset);

	Debug();

	void listFlagCommand(string* args, uint32 numArgs);
	void setFlagCommand(string* args, uint32 numArgs);
public:
	virtual ~Debug();

	void tick(int32 fps);
	void render();

	bool flag(const string& flag) const;

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