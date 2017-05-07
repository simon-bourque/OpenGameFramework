
#include "Core/Core.h"

#include <iostream>
#include <ctime>

void printToConsole(const string& message) {

	static time_t currentTimeRaw = 0;
	static struct tm currentTime;
	
	time(&currentTimeRaw);
	localtime_s(&currentTime, &currentTimeRaw);

	const static int32 dateStrSize = 128;
	char dateStr[dateStrSize];

	strftime(dateStr, dateStrSize, "[%T] ", &currentTime);

	std::cout << dateStr << message.c_str() << std::endl;
}