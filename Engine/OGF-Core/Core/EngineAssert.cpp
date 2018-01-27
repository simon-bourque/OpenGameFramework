
#include "EngineAssert.h"

#ifdef PLATFORM_WINDOWS

#include "Windows.h"
#include <cstdlib>
#include <memory>
#include <sstream>

#define ID_BREAK 150
#define ID_TEXT 200
#define ID_IGNORE 250
#define ID_TERMINATE 300

union DialogueBuffer {
	void* ptr;
	LPDLGTEMPLATE templatePtr;
	LPDLGITEMTEMPLATE itemTemplatePtr;
	LPWORD longPtr;
	LPWSTR wideStringPtr;
};


static void addButtonToDialogue(DialogueBuffer& buffer, size_t& space, int16 x, int16 y, WORD id, const wchar_t* label, uint32 labelLength);
static void addTextToDialogue(DialogueBuffer& buffer, size_t& space, int16 x, int16 y, const wchar_t* text, uint32 textLength);

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
	const static size_t BUFF_SIZE = 1024;

	// Init buffer
	HGLOBAL globalHandle = GlobalAlloc(GMEM_ZEROINIT, BUFF_SIZE);
	if (globalHandle == NULL) {
		return false;
	}

	DialogueBuffer buffer;

	buffer.ptr = GlobalLock(globalHandle);

	if (buffer.ptr == nullptr) {
		return false;
	}

	// Define the dialogue box
	buffer.templatePtr->style = WS_POPUP | WS_BORDER | DS_MODALFRAME | WS_CAPTION | DS_CENTER;
	buffer.templatePtr->cdit = 4;         // Number of controls
	buffer.templatePtr->x = 0;
	buffer.templatePtr->y = 0;
	buffer.templatePtr->cx = 350;
	buffer.templatePtr->cy = 200;
	buffer.templatePtr++;

	*buffer.longPtr++ = 0; // No menu
	*buffer.longPtr++ = 0; // Predefined dialog box class (by default)

	memcpy(buffer.wideStringPtr, L"Assert", 7 * sizeof(wchar_t));
	buffer.wideStringPtr += 7;

	// Calculate space left
	size_t space = BUFF_SIZE - sizeof(LPDLGTEMPLATE) - (7 * sizeof(wchar_t)) - (2 * sizeof(WORD));

	// Add buttons
	addButtonToDialogue(buffer, space, 55, 10, ID_BREAK, L"Break", 5);
	addButtonToDialogue(buffer, space, 95, 10, ID_IGNORE, L"Ignore", 6);
	addButtonToDialogue(buffer, space, 135, 10, ID_TERMINATE, L"Terminate", 9);

	// Add text
	std::wstringstream sstream;
	sstream << fileName.substr(fileName.find_last_of("/\\") + 1).c_str() << L" : " << lineNumber << L" - " << msg.c_str();
	addTextToDialogue(buffer, space, 10, 55, sstream.str().c_str(), sstream.str().length());

	// Display dialogue
	GlobalUnlock(globalHandle);
	INT_PTR retVal = DialogBoxIndirect(NULL, reinterpret_cast<LPDLGTEMPLATE>(globalHandle), NULL, (DLGPROC)DialogProc);

	// Cleanup
	GlobalFree(globalHandle);

	if (retVal == ID_TERMINATE) {
		std::exit(1);
	}

	return (retVal == ID_BREAK);
}

static void addButtonToDialogue(DialogueBuffer& buffer, size_t& space, int16 x, int16 y, WORD id, const wchar_t* label, uint32 labelLength) {
	const size_t buttonSize = sizeof(LPDLGITEMTEMPLATE) + ((labelLength + 1) * sizeof(wchar_t)) + (sizeof(WORD) * 3);

	if (std::align(alignof(DWORD), buttonSize, buffer.ptr, space)) {
		buffer.itemTemplatePtr->x = x;
		buffer.itemTemplatePtr->y = y;
		buffer.itemTemplatePtr->cx = 40;
		buffer.itemTemplatePtr->cy = 20;
		buffer.itemTemplatePtr->id = id;
		buffer.itemTemplatePtr->style = WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON;
		buffer.itemTemplatePtr++;

		*buffer.longPtr++ = 0xFFFF;
		*buffer.longPtr++ = 0x0080;        // Button class atom

		memcpy(buffer.wideStringPtr, label, (labelLength + 1) * sizeof(wchar_t));
		buffer.wideStringPtr += labelLength + 1;

		*buffer.longPtr++ = 0;             // No creation data
	}
}

static void addTextToDialogue(DialogueBuffer& buffer, size_t& space, int16 x, int16 y, const wchar_t* text, uint32 textLength) {
	const size_t textSize = sizeof(LPDLGITEMTEMPLATE) + ((textLength + 1) * sizeof(wchar_t)) + (sizeof(WORD) * 3);

	if (std::align(alignof(DWORD), textSize, buffer.ptr, space)) {
		buffer.itemTemplatePtr->x = x;
		buffer.itemTemplatePtr->y = y;
		buffer.itemTemplatePtr->cx = 100;
		buffer.itemTemplatePtr->cy = 20;
		buffer.itemTemplatePtr->id = ID_TEXT;    // Text identifier
		buffer.itemTemplatePtr->style = WS_CHILD | WS_VISIBLE | SS_LEFT;
		buffer.itemTemplatePtr++;

		*buffer.longPtr++ = 0xFFFF;
		*buffer.longPtr++ = 0x0082;        // Static class

		memcpy(buffer.wideStringPtr, text, (textLength + 1) * sizeof(wchar_t));
		buffer.wideStringPtr += textLength + 1;

		*buffer.longPtr++ = 0;             // No creation data
	}
}

void debugBreakWindows() {
	DebugBreak();
}

#endif