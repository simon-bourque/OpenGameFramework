#pragma once

#include "Core/Core.h"
#include "Core/Delegate.h"
#include "Core/EngineAssert.h"
#include "Core/Platform.h"

#include <unordered_map>
#include <vector>

#ifdef USING_WIN32_CONSOLE
#include "Windows.h"
#endif

typedef Delegate<const std::vector<string>&> CommandDelegate;

class Console {
private:
	static Console* s_instance;
	
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
	~Console();

	// Prevent copying of the console
	Console(const Console&) = delete;
	Console& operator=(const Console&) = delete;

	void write(const string& msg);
	void write(char c);
	void newLine();

	void pollEvents();

	void addCommand(const string& command, const CommandDelegate& del);
	void executeCommand(const string& command, const std::vector<string>& args);

	Console& operator<<(const string& msg);
	Console& operator<<(char c);

	static Console* get() {
		ASSERT(s_instance, "Console must be initialized before use.");
		return s_instance;
	};

	static void init() {
		ASSERT(!s_instance, "Console is already initialized.");
		s_instance = new Console();
	}

	static bool isInitialized() {
		return (s_instance != nullptr);
	}

	static void destroy() {
		delete s_instance;
	}
};

