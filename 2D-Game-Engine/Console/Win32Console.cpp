#include "Console.h"

#ifdef USING_WIN32_CONSOLE

#include <vector>
#include <cctype>

#define MAX_CMD_LENGTH 255

Console* Console::s_instance = nullptr;

// Test commands
static void pingCommand(const std::vector<string>& args) {
	Console::get()->write("pong");
	Console::get()->newLine();
}

Console::Console() : m_charsInCmdBuffer(0) {

	// If we don't already have a console then alloc a new one
	if (!GetConsoleWindow()) {
		if (!AllocConsole()) {
			throw std::runtime_error("Failed to allocate console. (" + std::to_string(GetLastError()) + ")");
		}
	}


	m_inputBuffer = GetStdHandle(STD_INPUT_HANDLE);
	if (m_inputBuffer == INVALID_HANDLE_VALUE) {
		throw std::runtime_error("Failed to get standard input handle. (" + std::to_string(GetLastError()) + ")");
	}

	m_screenBuffer = GetStdHandle(STD_OUTPUT_HANDLE);
	if (m_screenBuffer == INVALID_HANDLE_VALUE) {
		throw std::runtime_error("Failed to get standard output handle. (" + std::to_string(GetLastError()) + ")");
	}


	m_cmdBuffer = new char[MAX_CMD_LENGTH];

	// Add test command
	addCommand("ping", CommandDelegate::create<&pingCommand>());
}

Console::~Console() {
	delete[] m_cmdBuffer;
	FreeConsole();
}

void Console::write(const string& msg) {
	// TODO error checking
	DWORD charsWritten = 0;
	WriteConsole(m_screenBuffer, msg.c_str(), msg.length(), &charsWritten, NULL);
}

void Console::write(char c) {
	// TODO error checking
	DWORD charsWritten = 0;
	WriteConsole(m_screenBuffer, &c, 1, &charsWritten, NULL);
}

void Console::newLine() {
	// TODO error checking
	CONSOLE_SCREEN_BUFFER_INFO cinfo;
	GetConsoleScreenBufferInfo(m_screenBuffer, &cinfo);

	cinfo.dwCursorPosition.X = 0;
	cinfo.dwCursorPosition.Y += 1;

	SetConsoleCursorPosition(m_screenBuffer, cinfo.dwCursorPosition);
}

void Console::pollEvents() {
	// TODO error checking
	DWORD numInputEvents = 0;
	GetNumberOfConsoleInputEvents(m_inputBuffer, &numInputEvents);

	// If there are no events then return
	if (!numInputEvents) {
		return;
	}

	PINPUT_RECORD inputEvents = new INPUT_RECORD[numInputEvents];

	DWORD numInputEventsRead = 0;
	ReadConsoleInput(m_inputBuffer, inputEvents, numInputEvents, &numInputEventsRead);

	for (int32 i = 0; i < numInputEventsRead; i++) {
		INPUT_RECORD inputEvent = inputEvents[i];

		if (inputEvent.EventType == KEY_EVENT) {
			KEY_EVENT_RECORD keyEvent = inputEvent.Event.KeyEvent;
			if (keyEvent.bKeyDown == TRUE) {
				if (keyEvent.uChar.AsciiChar == '\r') {
					if (m_charsInCmdBuffer > 0) {
						erase(m_charsInCmdBuffer);
						parseCommandBuffer();
						m_charsInCmdBuffer = 0;
					}
				}
				else if (keyEvent.uChar.AsciiChar == '\b') {
					if (m_charsInCmdBuffer > 0) {
						--m_charsInCmdBuffer;
						erase(1);
					}
				}
				else {
					if (m_charsInCmdBuffer < MAX_CMD_LENGTH && isprint(keyEvent.uChar.AsciiChar)) {
						m_cmdBuffer[m_charsInCmdBuffer++] = keyEvent.uChar.AsciiChar;
						write(keyEvent.uChar.AsciiChar);
					}
				}
			}
		}
	}

	delete[] inputEvents;
}

void Console::addCommand(const string& command, const CommandDelegate& del) {
	// TODO add check to see if command already exists
	m_cmds[command] = del;
}

void Console::executeCommand(const string& command, const std::vector<string>& args) {
	auto commandIter = m_cmds.find(command);

	if (commandIter != m_cmds.end()) {
		commandIter->second(args);
	}
	else {
		(*this) << "Error: \'" << command << "\' is not a valid command.";
		newLine();
	}
}

Console& Console::operator<<(const string& msg) {
	write(msg);
	return *this;
}

Console& Console::operator<<(char c) {
	write(c);
	return *this;
}

void Console::erase(uint32 numChars) {
	// TODO see if it is possible to calculate the final cursor position and erase characters without iteration
	// TODO error checking
	CONSOLE_SCREEN_BUFFER_INFO cinfo;
	GetConsoleScreenBufferInfo(m_screenBuffer, &cinfo);
	
	for (int32 i = 0; i < numChars; i++) {
		// Update cursor
		if (cinfo.dwCursorPosition.X == 0) {
			cinfo.dwCursorPosition.X = cinfo.dwSize.X - 1;
			cinfo.dwCursorPosition.Y -= 1;
		}
		else {
			cinfo.dwCursorPosition.X -= 1;
		}
		SetConsoleCursorPosition(m_screenBuffer, cinfo.dwCursorPosition);

		// Erase character
		DWORD numCharsWritten = 0;
		const static char BLANK_SPACE = ' ';
		WriteConsoleOutputCharacter(m_screenBuffer, &BLANK_SPACE, 1, cinfo.dwCursorPosition, &numCharsWritten);
	}
}

void Console::parseCommandBuffer() {
	char* str = new char[m_charsInCmdBuffer + 1];
	for (int32 i = 0; i < m_charsInCmdBuffer; i++) {
		str[i] = m_cmdBuffer[i];
	}
	str[m_charsInCmdBuffer] = '\0';

	std::vector<string> tokens;
	char* nextToken = nullptr;

	char* token = strtok_s(str, " ", &nextToken);

	while (token != nullptr) {
		tokens.push_back(string(token));
		token = strtok_s(nullptr, " ", &nextToken);
	}

	delete[] str;

	// Remove the command itself from the list of tokens
	string command(tokens[0]);
	tokens.erase(tokens.begin());

	executeCommand(command, tokens);
}

#endif