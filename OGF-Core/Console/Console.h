#pragma once

#include "Core/Core.h"
#include "Core/Delegate.h"
#include "Core/EngineAssert.h"
#include "Core/Platform.h"
#include "Core/Singleton.h"

#include <unordered_map>
#include <vector>

#ifdef USING_WIN32_CONSOLE
#include "Windows.h"
#endif

typedef Delegate<const std::vector<string>&> CommandDelegate;

class Console {
	SINGLETON_DECLARATION(Console)
private:
#ifdef USING_WIN32_CONSOLE
	HANDLE m_inputBuffer;
	HANDLE m_screenBuffer;
#endif

	char* m_cmdBuffer;
	uint8 m_charsInCmdBuffer;

	std::unordered_map<string, CommandDelegate> m_cmds;

	Console();

	void erase(uint32 numChars);
	void parseCommandBuffer();
public:
	virtual ~Console();

	void write(const string& msg);
	void write(char c);
	void newLine();

	void pollEvents();

	void addCommand(const string& command, const CommandDelegate& del);
	void executeCommand(const string& command, const std::vector<string>& args);

	Console& operator<<(const string& msg);
	Console& operator<<(char c);
};

SINGLETON_ACCESSOR(Console)