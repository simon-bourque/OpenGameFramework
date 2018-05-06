
#include "Assert.h"

#ifdef PLATFORM_WINDOWS

#include <cstdlib>
#include <memory>
#include <sstream>
#include <vector>
#include <stdio.h>

#include "Core/Debug/Debug.h"

// IDs
constexpr uint16 ID_BREAK = 110;
constexpr uint16 ID_IGNORE = 111;
constexpr uint16 ID_TERMINATE = 112;
constexpr uint16 ID_EDITABLE_TEXT = 113;
constexpr uint16 ID_DONT_CARE = 500;

// Styles
constexpr uint32 STYLE_ITEM = WS_CHILD | WS_VISIBLE;
constexpr uint32 STYLE_BUTTON = STYLE_ITEM | BS_PUSHBUTTON | BS_TEXT;
constexpr uint32 STYLE_DEFAULT_BUTTON = STYLE_ITEM | BS_DEFPUSHBUTTON | BS_TEXT;
constexpr uint32 STYLE_EDITABLE_TEXT = STYLE_ITEM | ES_LEFT | ES_READONLY | ES_MULTILINE | WS_VSCROLL | WS_BORDER;
constexpr uint32 STYLE_STATIC_TEXT = STYLE_ITEM;
constexpr uint32 STYLE_WHITE_RECT = STYLE_ITEM | SS_WHITERECT;
constexpr uint32 STYLE_ICON = STYLE_ITEM | SS_ICON;

// Class ordinal values
constexpr uint16 CLASS_BUTTON = 0x0080;
constexpr uint16 CLASS_EDIT = 0x0081;
constexpr uint16 CLASS_STATIC = 0x0082;

static void addButtonToDialog(void** buffer, size_t& space, int16 x, int16 y, WORD id, DWORD style, const wchar_t* text, uint32 textLength);
static void addEditableTextToDialog(void** buffer, size_t& space, int16 x, int16 y, const wchar_t* text, uint32 textLength);
static void addStaticTextToDialog(void** buffer, size_t& space, int16 x, int16 y, const wchar_t* text, uint32 textLength);
static void addWhiteRectToDialog(void** buffer, size_t& space, int16 x, int16 y);
static void addIconToDialog(void** buffer, size_t& space, int16 x, int16 y);

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

	void* buffer = nullptr;

	buffer = GlobalLock(globalHandle);

	if (buffer == nullptr) {
		return false;
	}

	// Define the dialogue box
	LPDLGTEMPLATE templatePtr = reinterpret_cast<LPDLGTEMPLATE>(buffer);
	templatePtr->style = WS_POPUP | WS_BORDER | DS_MODALFRAME | WS_CAPTION | DS_CENTER | WS_SYSMENU | DS_MODALFRAME | DS_SHELLFONT;
	templatePtr->cdit = 7;         // Number of controls
	templatePtr->x = 0;
	templatePtr->y = 0;
	templatePtr->cx = 350;
	templatePtr->cy = 250;
	templatePtr++;
	
	LPWORD wordPtr = reinterpret_cast<LPWORD>(templatePtr);
	*wordPtr++ = 0; // No menu
	*wordPtr++ = 0; // Predefined dialog box class (by default)

	LPWSTR wideStringPtr = reinterpret_cast<LPWSTR>(wordPtr);
	memcpy(wideStringPtr, L"Open Game Framework", 20 * sizeof(wchar_t));
	wideStringPtr += 20;

	// Font
	wordPtr = reinterpret_cast<LPWORD>(wideStringPtr);
	*wordPtr++ = 8;

	wideStringPtr = reinterpret_cast<LPWSTR>(wordPtr);
	memcpy(wideStringPtr, L"MS Shell Dlg", 13 * sizeof(wchar_t));
	wideStringPtr += 13;

	buffer = reinterpret_cast<void*>(wideStringPtr);
	
	// Calculate space left
	size_t space = BUFF_SIZE - sizeof(LPDLGTEMPLATE) - (20 * sizeof(wchar_t)) - (3 * sizeof(WORD));

	// Add buttons
	addButtonToDialog(&buffer, space, 5, 230, ID_IGNORE, STYLE_DEFAULT_BUTTON, L"Ignore", 6);
	addButtonToDialog(&buffer, space, 60, 230, ID_BREAK, STYLE_BUTTON | (Debug::isDebuggerAttached() ? 0 : WS_DISABLED), L"Break", 5);
	addButtonToDialog(&buffer, space, 115, 230, ID_TERMINATE, STYLE_BUTTON, L"Terminate", 9);

	addWhiteRectToDialog(&buffer, space, 0, 0);
	addEditableTextToDialog(&buffer, space, 5, 45, msg.c_str(), msg.length());
	addIconToDialog(&buffer, space, 10, 10);
	addStaticTextToDialog(&buffer, space, 38, 18, L"Assertion Failed!", 17);

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

static void addDialogItem(void** buffer, size_t& space, int16 x, int16 y, int16 width, int16 height, WORD id, DWORD style, WORD classOrdinalValue, const wchar_t* title, uint32 titleLength) {
	const size_t itemSize = sizeof(DLGITEMTEMPLATE) + ((titleLength + 1) * sizeof(wchar_t)) + (sizeof(WORD) * 3);

	if (std::align(alignof(DWORD), itemSize, *buffer, space)) {
		LPDLGITEMTEMPLATE itemTemplatePtr = reinterpret_cast<LPDLGITEMTEMPLATE>(*buffer);
		itemTemplatePtr->x = x;
		itemTemplatePtr->y = y;
		itemTemplatePtr->cx = width;
		itemTemplatePtr->cy = height;
		itemTemplatePtr->id = id;
		itemTemplatePtr->style = style;
		itemTemplatePtr++;

		LPWORD wordPtr = reinterpret_cast<LPWORD>(itemTemplatePtr);
		*wordPtr++ = 0xFFFF;
		*wordPtr++ = classOrdinalValue;

		LPWSTR wideStringPtr = reinterpret_cast<LPWSTR>(wordPtr);
		memcpy(wideStringPtr, title, (titleLength + 1) * sizeof(wchar_t));
		wideStringPtr += titleLength + 1;

		wordPtr = reinterpret_cast<LPWORD>(wideStringPtr);
		*wordPtr++ = 0;             // No creation data

		*buffer = reinterpret_cast<void*>(wordPtr);
	}
}

static void addButtonToDialog(void** buffer, size_t& space, int16 x, int16 y, WORD id, DWORD style, const wchar_t* text, uint32 textLength) {
	addDialogItem(buffer, space, x, y, 50, 14, id, style, CLASS_BUTTON, text, textLength);
}

static void addEditableTextToDialog(void** buffer, size_t& space, int16 x, int16 y, const wchar_t* text, uint32 textLength) {
	addDialogItem(buffer, space, x, y, 340, 165, ID_EDITABLE_TEXT, STYLE_EDITABLE_TEXT, CLASS_EDIT, text, textLength);
}

static void addStaticTextToDialog(void** buffer, size_t& space, int16 x, int16 y, const wchar_t* text, uint32 textLength) {
	addDialogItem(buffer, space, x, y, 100, 20, ID_DONT_CARE, STYLE_STATIC_TEXT, CLASS_STATIC, text, textLength);
}

static void addWhiteRectToDialog(void** buffer, size_t& space, int16 x, int16 y) {
	addDialogItem(buffer, space, x, y, 500, 220, ID_DONT_CARE, STYLE_WHITE_RECT, CLASS_STATIC, L"\0", 1);
}

static void addIconToDialog(void** buffer, size_t& space, int16 x, int16 y) {
	wchar_t iconStr[2] = { 0xFFFF, (WORD)IDI_WARNING };
	addDialogItem(buffer, space, x, y, SM_CXICON, SM_CYICON, ID_DONT_CARE, STYLE_ICON, CLASS_STATIC, iconStr, 1);
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
		delete[] buffer;
		return std::wstring();
	}
	
	va_end(args);

	std::wstringstream sstream;
	sstream << L"File: " << fileName << "\r\n" << "Line: " << lineNumber << "\r\n\r\n" << buffer << "\r\n\r\n";
	for (const std::string& str : stackTrace) {
		sstream << str.c_str() << "\r\n";
	}

	delete[] buffer;
	return sstream.str();

}
#endif