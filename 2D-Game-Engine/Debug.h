#ifdef DEBUG_BUILD


#ifndef DEBUG_H
#define DEBUG_H

#include "Core.h"

#include <memory>

class Game;
class Font;
class Text;

class Debug {
private:
	Game* m_game;

	Text* m_fpsText;
	Text* m_debugOnText;

	bool m_debugMode;
	bool m_renderPerf;

	bool m_zoomIn;
	bool m_zoomOut;

	void renderPerf() const;
	void onKeyPress(int32 key, int32 scancode, int32 action, int32 mods);
public:
	Debug(Game* game);
	virtual ~Debug();

	void tick();
	void render();

	Debug(const Debug&) = delete;
	Debug& operator=(const Debug&) = delete;
};

#endif
#endif