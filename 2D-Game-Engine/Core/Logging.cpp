#include "Core/Core.h"
#include "Core/Time.h"

#include "Console/Console.h"
#include "Core/Singleton.h"

void printToConsole(const string& message) {
	(*getConsoleInstance()) << getSystemTimeString() << ' ' << message;
	getConsoleInstance()->newLine();
}