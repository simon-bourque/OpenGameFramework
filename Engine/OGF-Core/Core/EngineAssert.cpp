
#include "EngineAssert.h"

#ifdef PLATFORM_WINDOWS

#include <cstdlib>
#include <memory>
#include <sstream>
#include <vector>

#include <DbgHelp.h>

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

static void captureStackTrace(std::vector<std::string>& stackTrace);
static void addButtonToDialogue(DialogueBuffer& buffer, size_t& space, int16 x, int16 y, WORD id, const wchar_t* label, uint32 labelLength, bool isDefButton = false);
static void addTextToDialogue(DialogueBuffer& buffer, size_t& space, int16 x, int16 y, const wchar_t* text, uint32 textLength);

INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg)
	{
	case WM_INITDIALOG:
		MessageBeep(MB_ICONWARNING);
		break;
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
	buffer.templatePtr->style = WS_POPUP | WS_BORDER | DS_MODALFRAME | WS_CAPTION | DS_CENTER | WS_SYSMENU | DS_MODALFRAME | DS_SETFONT;
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

	*buffer.longPtr++ = 8;
	memcpy(buffer.wideStringPtr, L"MS Shell Dlg", 13 * sizeof(wchar_t));
	buffer.wideStringPtr += 13;

	// Calculate space left
	size_t space = BUFF_SIZE - sizeof(LPDLGTEMPLATE) - (20 * sizeof(wchar_t)) - (3 * sizeof(WORD));

	// Add buttons
	addButtonToDialogue(buffer, space, 5, 180, ID_IGNORE, L"Ignore", 6, true);
	addButtonToDialogue(buffer, space, 60, 180, ID_BREAK, L"Break", 5);
	addButtonToDialogue(buffer, space, 115, 180, ID_TERMINATE, L"Terminate", 9);

	// Capture stack trace
	std::vector<std::string> stackTrace;
	captureStackTrace(stackTrace);

	// Add text
	std::wstringstream sstream;
	//sstream << fileName.substr(fileName.find_last_of("/\\") + 1).c_str() << L" : " << lineNumber << L" - " << msg.c_str();
	sstream << L"File: " << fileName.c_str() << "\n" << "Line: " << lineNumber << "\n" << msg.c_str() << "\n\n";
	for (const std::string& str : stackTrace) {
		sstream << str.c_str() << "\n";
	}

	addTextToDialogue(buffer, space, 0, 0, sstream.str().c_str(), sstream.str().length());

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

static void captureStackTrace(std::vector<std::string>& stackTrace) {
	CONTEXT context = {0};
	context.ContextFlags = CONTEXT_CONTROL;
	
	//__asm {
	//	LABEL:
	//	mov[context.Ebp], ebp;
	//	mov[context.Esp], esp;
	//	mov eax, [LABEL];
	//	mov[context.Eip], eax;
	//}
	RtlCaptureContext(&context);

	STACKFRAME64 stackFrame;
#ifdef _WIN64
#define MACHINE_TYPE IMAGE_FILE_MACHINE_AMD64
	stackFrame.AddrPC.Offset = context.Rip;
	stackFrame.AddrPC.Mode = AddrModeFlat;
	stackFrame.AddrFrame.Offset = context.Rbp;
	stackFrame.AddrFrame.Mode = AddrModeFlat;
	stackFrame.AddrStack.Offset = context.Rsp;
	stackFrame.AddrStack.Mode = AddrModeFlat;
#else
#define MACHINE_TYPE IMAGE_FILE_MACHINE_I386
	stackFrame.AddrPC.Offset = context.Eip;
	stackFrame.AddrPC.Mode = AddrModeFlat;
	stackFrame.AddrFrame.Offset = context.Ebp;
	stackFrame.AddrFrame.Mode = AddrModeFlat;
	stackFrame.AddrStack.Offset = context.Esp;
	stackFrame.AddrStack.Mode = AddrModeFlat;
#endif



	while (StackWalk64(MACHINE_TYPE, GetCurrentProcess(), GetCurrentThread(), &stackFrame, &context, NULL, SymFunctionTableAccess64, SymGetModuleBase64, NULL)) {
		char buffer[sizeof(SYMBOL_INFO) + (MAX_SYM_NAME * sizeof(TCHAR))];
		PSYMBOL_INFO symInfo = reinterpret_cast<PSYMBOL_INFO>(buffer);
		symInfo->SizeOfStruct = sizeof(SYMBOL_INFO);
		symInfo->MaxNameLen = MAX_SYM_NAME;

		bool success2 = SymFromAddr(GetCurrentProcess(), stackFrame.AddrPC.Offset, 0, symInfo);
		std::string name(symInfo->Name, symInfo->NameLen);
		stackTrace.push_back(name);

		int x = 0;
	}

#undef MACHINE_TYPE
}

static void addButtonToDialogue(DialogueBuffer& buffer, size_t& space, int16 x, int16 y, WORD id, const wchar_t* label, uint32 labelLength, bool isDefButton) {
	const size_t buttonSize = sizeof(LPDLGITEMTEMPLATE) + ((labelLength + 1) * sizeof(wchar_t)) + (sizeof(WORD) * 3);

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

static void addTextToDialogue(DialogueBuffer& buffer, size_t& space, int16 x, int16 y, const wchar_t* text, uint32 textLength) {
	const size_t textSize = sizeof(LPDLGITEMTEMPLATE) + ((textLength + 1) * sizeof(wchar_t)) + (sizeof(WORD) * 3);

	if (std::align(alignof(DWORD), textSize, buffer.ptr, space)) {
		buffer.itemTemplatePtr->x = x;
		buffer.itemTemplatePtr->y = y;
		buffer.itemTemplatePtr->cx = 500;
		buffer.itemTemplatePtr->cy = 100;
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

#endif