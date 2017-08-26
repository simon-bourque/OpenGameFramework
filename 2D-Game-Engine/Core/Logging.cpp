#include "Core/Core.h"
#include "Core/Time.h"

#include "Console/Console.h"

void printToConsole(const string& message) {
	(*Console::get()) << getSystemTimeString() << ' ' << message;
	Console::get()->newLine();
}