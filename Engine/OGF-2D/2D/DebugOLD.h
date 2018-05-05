#ifdef DEBUG_BUILD

#pragma once

#include "Core/Core.h"
#include "Core/Assert.h"
#include "Core/Singleton.h"

#include <memory>
#include <unordered_map>

class Game;
class Font;
class Text;

class DebugOLD {
	SINGLETON_DECLARATION(DebugOLD)
private:
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

	DebugOLD();

	void listFlagCommand(const std::vector<string>& args);
	void setFlagCommand(const std::vector<string>& args);
public:
	virtual ~DebugOLD();

	void tick(int32 fps);
	void render();

	bool flag(const string& flag) const;
};

SINGLETON_ACCESSOR(DebugOLD)

#endif