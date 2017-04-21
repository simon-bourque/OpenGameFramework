#include "Platform.h"

#ifdef OS_WINDOWS
	#include "Windows.h"
#endif


void displayErrorBox(const std::string& msg) {
#ifdef OS_WINDOWS
	MessageBox(NULL, msg.c_str(), "Exception", MB_OK | MB_ICONERROR);
#endif
}
