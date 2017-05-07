
#include "EngineAssert.h"

#ifdef OS_WINDOWS

#include "Windows.h"
#include <cstdlib>

#define ID_BREAK 150
#define ID_TEXT 200
#define ID_IGNORE 250
#define ID_TERMINATE 300

LPWORD lpwAlign(LPWORD lpIn)
{
	//ULONG ul;

	//ul = (ULONG)lpIn;
	//ul++;
	//ul >>= 1;
	//ul <<= 1;

	ULONG ul;
	ul = (ULONG)lpIn;
	if (ul % 4) {
		ul += (4 - (ul % 4));
	}
	return (LPWORD)ul;
}

INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg)
	{
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_TERMINATE:
		case ID_IGNORE:
		case ID_TEXT:
		case ID_BREAK:
			EndDialog(hwndDlg, wParam);
			return TRUE;
		}
	}
	return FALSE;
}

bool showAssertDialogWindows(const string& msg, const string& fileName, uint32 lineNumber) {
	HGLOBAL hgbl;
	LPDLGTEMPLATE templatePtr;
	LPDLGITEMTEMPLATE itemPtr;
	LPWORD longPtr;
	LPWSTR stringPtr;
	LRESULT ret;
	int32 nchar;

	hgbl = GlobalAlloc(GMEM_ZEROINIT, 1024);
	if (!hgbl)
		return false;

	//setupDialogTemplate(hgbl);
	templatePtr = (LPDLGTEMPLATE)GlobalLock(hgbl);

	// Define a dialog box
	templatePtr->style = WS_POPUP | WS_BORDER | DS_MODALFRAME | WS_CAPTION | DS_CENTER;
	templatePtr->cdit = 4;         // Number of controls
	templatePtr->x = 0;
	templatePtr->y = 0;
	templatePtr->cx = 350;
	templatePtr->cy = 200;

	longPtr = (LPWORD)(templatePtr + 1);
	*longPtr++ = 0;             // No menu
	*longPtr++ = 0;             // Predefined dialog box class (by default)

	stringPtr = (LPWSTR)longPtr;
	nchar = MultiByteToWideChar(CP_ACP, 0, "Assert", -1, stringPtr, 50);
	longPtr += nchar;


	// Break button ---------------------------------------------------------------------------
	longPtr = lpwAlign(longPtr);    // Align DLGITEMTEMPLATE on DWORD boundary
	itemPtr = (LPDLGITEMTEMPLATE)longPtr;
	itemPtr->x = 55;
	itemPtr->y = 10;
	itemPtr->cx = 40;
	itemPtr->cy = 20;
	itemPtr->id = ID_BREAK;    // Help button identifier
	itemPtr->style = WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON;

	longPtr = (LPWORD)(itemPtr + 1);
	*longPtr++ = 0xFFFF;
	*longPtr++ = 0x0080;        // Button class atom

	stringPtr = (LPWSTR)longPtr;
	nchar = MultiByteToWideChar(CP_ACP, 0, "Break", -1, stringPtr, 50);
	longPtr += nchar;
	*longPtr++ = 0;             // No creation data

	
	// Ignore button ---------------------------------------------------------------------------
	longPtr = lpwAlign(longPtr);    // Align DLGITEMTEMPLATE on DWORD boundary
	itemPtr = (LPDLGITEMTEMPLATE)longPtr;
	itemPtr->x = 95;
	itemPtr->y = 10;
	itemPtr->cx = 40;
	itemPtr->cy = 20;
	itemPtr->id = ID_IGNORE;    // Help button identifier
	itemPtr->style = WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON;

	longPtr = (LPWORD)(itemPtr + 1);
	*longPtr++ = 0xFFFF;
	*longPtr++ = 0x0080;        // Button class atom

	stringPtr = (LPWSTR)longPtr;
	nchar = MultiByteToWideChar(CP_ACP, 0, "Ignore", -1, stringPtr, 50);
	longPtr += nchar;
	*longPtr++ = 0;             // No creation data

	
	// Terminate button ---------------------------------------------------------------------------
	longPtr = lpwAlign(longPtr);    // Align DLGITEMTEMPLATE on DWORD boundary
	itemPtr = (LPDLGITEMTEMPLATE)longPtr;
	itemPtr->x = 135;
	itemPtr->y = 10;
	itemPtr->cx = 40;
	itemPtr->cy = 20;
	itemPtr->id = ID_TERMINATE;    // Help button identifier
	itemPtr->style = WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON;

	longPtr = (LPWORD)(itemPtr + 1);
	*longPtr++ = 0xFFFF;
	*longPtr++ = 0x0080;        // Button class atom

	stringPtr = (LPWSTR)longPtr;
	nchar = MultiByteToWideChar(CP_ACP, 0, "Terminate", -1, stringPtr, 50);
	longPtr += nchar;
	*longPtr++ = 0;             // No creation data

	
	// Text ---------------------------------------------------------------------------
	longPtr = lpwAlign(longPtr);    // Align DLGITEMTEMPLATE on DWORD boundary
	itemPtr = (LPDLGITEMTEMPLATE)longPtr;
	itemPtr->x = 10;
	itemPtr->y = 55;
	itemPtr->cx = 100;
	itemPtr->cy = 20;
	itemPtr->id = ID_TEXT;    // Text identifier
	itemPtr->style = WS_CHILD | WS_VISIBLE | SS_LEFT;

	longPtr = (LPWORD)(itemPtr + 1);
	*longPtr++ = 0xFFFF;
	*longPtr++ = 0x0082;        // Static class

	// Convert string to unicode
	string finalMessage(fileName.substr(fileName.find_last_of("/\\") + 1));
	finalMessage.append(" : ");
	finalMessage.append(std::to_string(lineNumber));
	finalMessage.append(" - ");
	finalMessage.append(msg);
	uint32 size = finalMessage.size() + 1;

	const char* cString = finalMessage.c_str();
	wchar_t* newString = new wchar_t[size];
	nchar = MultiByteToWideChar(CP_ACP, 0, cString, -1, newString, size);

	stringPtr = (LPWSTR)longPtr;
	for (uint32 i = 0; i < size; i++) {
		*stringPtr++ = newString[i];
	}
	//for (stringPtr = (LPWSTR)longPtr; *stringPtr++ = (WCHAR)*newString++;);
	longPtr = (LPWORD)stringPtr;
	*longPtr++ = 0;             // No creation data
	delete[] newString;

	GlobalUnlock(hgbl);
	ret = DialogBoxIndirect(NULL, (LPDLGTEMPLATE)hgbl, NULL, (DLGPROC)DialogProc);
	GlobalFree(hgbl);

	if (ret == ID_TERMINATE) {
		std::exit(1);
	}

	return (ret == ID_BREAK);
}

void debugBreakWindows() {
	DebugBreak();
}

#endif