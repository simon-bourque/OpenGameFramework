
#include "Core/Core.h"

#include <iostream>

#include "Core/Time.h"

void printToConsole(const string& message) {
	std::cout << getSystemTimeString() << ' ' << message.c_str() << std::endl;
}