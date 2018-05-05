
#include "Assert.h"

#ifdef PLATFORM_WINDOWS

#include <cstdlib>
#include <memory>
#include <sstream>
#include <vector>
#include <stdio.h>

#include "Core/Debug/Debug.h"

#define ID_BREAK 110
#define ID_IGNORE 111
#define ID_TERMINATE 112
#define ID_EDITABLE_TEXT 113
#define ID_NO_COMMAND 500

union DialogBuffer {
	void* ptr;
	LPDLGTEMPLATE templatePtr;
	LPDLGITEMTEMPLATE itemTemplatePtr;
	LPWORD longPtr;
	LPWSTR wideStringPtr;
};

static void addButtonToDialog(DialogBuffer& buffer, size_t& space, int16 x, int16 y, WORD id, const wchar_t* label, uint32 labelLength, bool isDefButton = false);
static void addEditableTextToDialog(DialogBuffer& buffer, size_t& space, int16 x, int16 y, const wchar_t* text, uint32 textLength);
static void addStaticTextToDialog(DialogBuffer& buffer, size_t& space, int16 x, int16 y, const wchar_t* text, uint32 textLength);
static void addWhiteRectToDialog(DialogBuffer& buffer, size_t& space, int16 x, int16 y);
static void addIconToDialog(DialogBuffer& buffer, size_t& space, int16 x, int16 y);

INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {	
	switch (uMsg)
	{
	case WM_CTLCOLORSTATIC: {
		if (GetDlgCtrlID((HWND)lParam) != ID_EDITABLE_TEXT) {
			HGDIOBJ whiteBrush = GetStockObject(WHITE_BRUSH);
			return (INT_PTR)whiteBrush;
		}
		break;
	}
	case WM_INITDIALOG: {
		MessageBeep(MB_ICONWARNING);
		break;
	}
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_TERMINATE:
			[[fallthrough]];
		case ID_IGNORE:
			[[fallthrough]];
		case ID_BREAK:
			EndDialog(hwndDlg, wParam);
			return TRUE;
		}
		break;
	case WM_CLOSE:
		EndDialog(hwndDlg, ID_IGNORE);
		return TRUE;
	}
	return FALSE;
}

bool showAssertDialogWindows(const std::wstring& msg) {
	const static size_t BUFF_SIZE = 1024 * 5;

	// Init buffer
	HGLOBAL globalHandle = GlobalAlloc(GMEM_ZEROINIT, BUFF_SIZE);
	if (globalHandle == NULL) {
		return false;
	}

	DialogBuffer buffer;

	buffer.ptr = GlobalLock(globalHandle);

	if (buffer.ptr == nullptr) {
		return false;
	}

	// Define the dialogue box
	buffer.templatePtr->style = WS_POPUP | WS_BORDER | DS_MODALFRAME | WS_CAPTION | DS_CENTER | WS_SYSMENU | DS_MODALFRAME | DS_SHELLFONT;
	buffer.templatePtr->cdit = 7;         // Number of controls
	buffer.templatePtr->x = 0;
	buffer.templatePtr->y = 0;
	buffer.templatePtr->cx = 350;
	buffer.templatePtr->cy = 250;
	buffer.templatePtr++;
	
	*buffer.longPtr++ = 0; // No menu
	*buffer.longPtr++ = 0; // Predefined dialog box class (by default)

	memcpy(buffer.wideStringPtr, L"Open Game Framework", 20 * sizeof(wchar_t));
	buffer.wideStringPtr += 20;

	*buffer.longPtr++ = 8;
	memcpy(buffer.wideStringPtr, L"MS Shell Dlg", 13 * sizeof(wchar_t));
	buffer.wideStringPtr += 13;

	// Calculate space left
	size_t space = BUFF_SIZE - sizeof(LPDLGTEMPLATE) - (20 * sizeof(wchar_t)) - (3 * sizeof(WORD));

	// Add buttons
	addButtonToDialog(buffer, space, 5, 230, ID_IGNORE, L"Ignore", 6, true);
	addButtonToDialog(buffer, space, 60, 230, ID_BREAK, L"Break", 5);
	addButtonToDialog(buffer, space, 115, 230, ID_TERMINATE, L"Terminate", 9);

	// Add text
	//std::wstringstream sstream;
	//sstream << L"File: " << fileName.c_str() << "\r\n" << "Line: " << lineNumber << "\r\n\r\n" << msg.c_str() << "\r\n\r\n";
	//for (const std::string& str : stackTrace) {
	//	sstream << str.c_str() << "\r\n";
	//}

	addWhiteRectToDialog(buffer, space, 0, 0);
	addEditableTextToDialog(buffer, space, 5, 45, msg.c_str(), msg.length());
	addIconToDialog(buffer, space, 10, 10);
	addStaticTextToDialog(buffer, space, 38, 18, L"Assertion Failed!", 17);

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

static void addButtonToDialog(DialogBuffer& buffer, size_t& space, int16 x, int16 y, WORD id, const wchar_t* label, uint32 labelLength, bool isDefButton) {
	const size_t buttonSize = sizeof(DLGITEMTEMPLATE) + ((labelLength + 1) * sizeof(wchar_t)) + (sizeof(WORD) * 3);

	if (std::align(alignof(DWORD), buttonSize, buffer.ptr, space)) {
		buffer.itemTemplatePtr->x = x;
		buffer.itemTemplatePtr->y = y;
		buffer.itemTemplatePtr->cx = 50;
		buffer.itemTemplatePtr->cy = 14;
		buffer.itemTemplatePtr->id = id;
		buffer.itemTemplatePtr->style = WS_CHILD | WS_VISIBLE | ((isDefButton) ? BS_DEFPUSHBUTTON : BS_PUSHBUTTON);
		buffer.itemTemplatePtr++;

		*buffer.longPtr++ = 0xFFFF;
		*buffer.longPtr++ = 0x0080;        // Button class atom

		memcpy(buffer.wideStringPtr, label, (labelLength + 1) * sizeof(wchar_t));
		buffer.wideStringPtr += labelLength + 1;

		*buffer.longPtr++ = 0;             // No creation data
	}
}

static void addEditableTextToDialog(DialogBuffer& buffer, size_t& space, int16 x, int16 y, const wchar_t* text, uint32 textLength) {
	const size_t textSize = sizeof(DLGITEMTEMPLATE) + (sizeof(wchar_t) * textLength + 1) + (sizeof(WORD) * 3);

	if (std::align(alignof(DWORD), textSize, buffer.ptr, space)) {
		buffer.itemTemplatePtr->x = x;
		buffer.itemTemplatePtr->y = y;
		buffer.itemTemplatePtr->cx = 340;
		buffer.itemTemplatePtr->cy = 165;
		buffer.itemTemplatePtr->id = ID_EDITABLE_TEXT;    // Text identifier
		buffer.itemTemplatePtr->style = WS_CHILD | WS_VISIBLE | ES_LEFT | ES_READONLY | ES_MULTILINE | WS_VSCROLL | WS_BORDER;
		buffer.itemTemplatePtr++;

		*buffer.longPtr++ = 0xFFFF;
		*buffer.longPtr++ = 0x0081;        // Edit class

		memcpy(buffer.wideStringPtr, text, (textLength + 1) * sizeof(wchar_t));
		buffer.wideStringPtr += textLength + 1;

		*buffer.longPtr++ = 0;             // No creation data
	}
}

static void addStaticTextToDialog(DialogBuffer& buffer, size_t& space, int16 x, int16 y, const wchar_t* text, uint32 textLength) {
	const size_t textSize = sizeof(DLGITEMTEMPLATE) + (sizeof(wchar_t) * (textLength + 1)) + (sizeof(WORD) * 3);

	if (std::align(alignof(DWORD), textSize, buffer.ptr, space)) {
		buffer.itemTemplatePtr->x = x;
		buffer.itemTemplatePtr->y = y;
		buffer.itemTemplatePtr->cx = 100;
		buffer.itemTemplatePtr->cy = 20;
		buffer.itemTemplatePtr->id = ID_NO_COMMAND;    // Text identifier
		buffer.itemTemplatePtr->style = WS_CHILD | WS_VISIBLE;
		buffer.itemTemplatePtr++;

		*buffer.longPtr++ = 0xFFFF;
		*buffer.longPtr++ = 0x0082;        // Static class

		memcpy(buffer.wideStringPtr, text, (textLength + 1) * sizeof(wchar_t));
		buffer.wideStringPtr += textLength + 1;

		*buffer.longPtr++ = 0;             // No creation data
	}
}

static void addWhiteRectToDialog(DialogBuffer& buffer, size_t& space, int16 x, int16 y) {
	const size_t rectSize = sizeof(DLGITEMTEMPLATE) + sizeof(wchar_t) + (sizeof(WORD) * 3);

	if (std::align(alignof(DWORD), rectSize, buffer.ptr, space)) {
		buffer.itemTemplatePtr->x = x;
		buffer.itemTemplatePtr->y = y;
		buffer.itemTemplatePtr->cx = 500;
		buffer.itemTemplatePtr->cy = 220;
		buffer.itemTemplatePtr->id = ID_NO_COMMAND;    // Text identifier
		buffer.itemTemplatePtr->style = WS_CHILD | WS_VISIBLE | SS_WHITERECT;
		buffer.itemTemplatePtr++;

		*buffer.longPtr++ = 0xFFFF;
		*buffer.longPtr++ = 0x0082;        // Static class

		*buffer.wideStringPtr++ = L'\0';

		*buffer.longPtr++ = 0;             // No creation data
	}
}

static void addIconToDialog(DialogBuffer& buffer, size_t& space, int16 x, int16 y) {
	const size_t iconSize = sizeof(DLGITEMTEMPLATE) + (sizeof(WORD) * 5);
	
	if (std::align(alignof(DWORD), iconSize, buffer.ptr, space)) {
		buffer.itemTemplatePtr->x = x;
		buffer.itemTemplatePtr->y = y;
		buffer.itemTemplatePtr->cx = SM_CXICON;
		buffer.itemTemplatePtr->cy = SM_CYICON;
		buffer.itemTemplatePtr->id = ID_NO_COMMAND;    // Text identifier
		buffer.itemTemplatePtr->style = WS_CHILD | WS_VISIBLE | SS_ICON;
		buffer.itemTemplatePtr++;
	
		*buffer.longPtr++ = 0xFFFF;
		*buffer.longPtr++ = 0x0082;        // Static class
	
		*buffer.longPtr++ = 0xFFFF;
		*buffer.longPtr++ = (WORD)IDI_WARNING;
	
		*buffer.longPtr++ = 0;             // No creation data
	}

}

std::wstring formatAssertMessage(const char* fileName, uint32 lineNumber, const std::vector<string>& stackTrace, const char* msg, ...) {
	va_list args;
	va_start(args, msg);

	int32 bufferSize = _vscprintf(msg, args);
	if (bufferSize < 0)
	{
		// Error
		va_end(args);
		return std::wstring();
	}
	
	bufferSize++; // For terminating null character
	char* buffer = new char[bufferSize];
	int32 charsWritten = vsprintf_s(buffer, bufferSize, msg, args);
	if (charsWritten < 0)
	{
		// Error
		va_end(args);
		return std::wstring();
	}
	
	va_end(args);

	std::wstringstream sstream;
	sstream << L"File: " << fileName << "\r\n" << "Line: " << lineNumber << "\r\n\r\n" << buffer << "\r\n\r\n";
	for (const std::string& str : stackTrace) {
		sstream << str.c_str() << "\r\n";
	}

	return sstream.str();

}
#endif