#include "Window.h"

#ifdef USING_WIN32_WINDOW

#include <GL/glew.h>

#include "Input/Input.h"

#include <Windowsx.h>

LRESULT CALLBACK MainWndProc(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg)
	{
	case WM_CLOSE:
		getWindowInstance()->m_shouldClose = true;
		DestroyWindow(handle);
		return 0;
	case WM_SIZE:
		if (Singleton<Window>::isInitialized()) {
			getWindowInstance()->m_width = LOWORD(lParam);
			getWindowInstance()->m_height = HIWORD(lParam);
			glViewport(0, 0, LOWORD(lParam), HIWORD(lParam));
		}
		return 0;
	case WM_SYSKEYDOWN:
		// Alt-f4
		if (((lParam >> 29) & 0x1) && wParam == VK_F4) {
			return DefWindowProc(handle, msg, wParam, lParam);
		}
	case WM_KEYDOWN: {
		int32 mods = 0;
		if ((GetKeyState(VK_SHIFT) >> (sizeof(SHORT) - 1))) {
			mods |= Mods::MOD_SHIFT_BIT;
		}
		if ((GetKeyState(VK_CONTROL) >> (sizeof(SHORT) - 1))) {
			mods |= Mods::MOD_CONTROL_BIT;
		}
		if ((GetKeyState(VK_MENU) >> (sizeof(SHORT) - 1))) {
			mods |= Mods::MOD_ALT_BIT;
		}

		keyCallback(wParam, ((lParam >> 16) & 0xff), ((lParam >> 30) & 0x1) ? Actions::REPEAT : Actions::PRESS, mods);
		return 0;
	}
	case WM_SYSKEYUP:
	case WM_KEYUP: {
		keyCallback(wParam, ((lParam >> 16) & 0xff), Actions::RELEASE, 0);
		return 0;
	}
	case WM_SYSCOMMAND:
		if (wParam == SC_KEYMENU) {
			return 0;
		}
		return DefWindowProc(handle, msg, wParam, lParam);
	case WM_MOUSEMOVE: {
		float64 x = GET_X_LPARAM(lParam);
		float64 y = GET_Y_LPARAM(lParam);
		cursorPositionCallback(x, y);
		return 0;
	}
	case WM_LBUTTONDOWN:
		mouseButtonCallback(MouseButtons::MOUSE_BUTTON_LEFT, Actions::PRESS, 0);
		return 0;
	case WM_LBUTTONUP:
		mouseButtonCallback(MouseButtons::MOUSE_BUTTON_LEFT, Actions::RELEASE, 0);
		return 0;
	case WM_RBUTTONDOWN:
		mouseButtonCallback(MouseButtons::MOUSE_BUTTON_RIGHT, Actions::PRESS, 0);
		return 0;
	case WM_RBUTTONUP:
		mouseButtonCallback(MouseButtons::MOUSE_BUTTON_RIGHT, Actions::RELEASE, 0);
		return 0;
	case WM_MBUTTONDOWN:
		mouseButtonCallback(MouseButtons::MOUSE_BUTTON_MIDDLE, Actions::PRESS, 0);
		return 0;
	case WM_MBUTTONUP:
		mouseButtonCallback(MouseButtons::MOUSE_BUTTON_MIDDLE, Actions::RELEASE, 0);
		return 0;
	case WM_XBUTTONDOWN:
		mouseButtonCallback((GET_XBUTTON_WPARAM(wParam) & XBUTTON1) ? MouseButtons::MOUSE_BUTTON_4 : MouseButtons::MOUSE_BUTTON_5, Actions::PRESS, 0);
		return 0;
	case WM_XBUTTONUP:
		mouseButtonCallback((GET_XBUTTON_WPARAM(wParam) & XBUTTON1) ? MouseButtons::MOUSE_BUTTON_4 : MouseButtons::MOUSE_BUTTON_5, Actions::RELEASE, 0);
		return 0;
	case WM_MOUSEWHEEL: {
		float64 delta = GET_WHEEL_DELTA_WPARAM(wParam) / ((float64)WHEEL_DELTA);
		scrollCallback(0, delta);
		return 0;
	}
	default:
		return DefWindowProc(handle, msg, wParam, lParam);
	}
	return 0;
}

Window::Window(const string& title, int32 width, int32 height) : m_title(title), m_width(width), m_height(height), m_shouldClose(false) {
	
	// Display modes
	DEVMODE dm;
	ZeroMemory(&dm, sizeof(dm));
	dm.dmSize = sizeof(dm);
	dm.dmDriverExtra = 0;
	EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dm);
	string res = std::to_string(dm.dmPelsWidth) + "x" + std::to_string(dm.dmPelsHeight) + " " + std::to_string(dm.dmDisplayFrequency) + "hz";
	DEBUG_LOG(res);

	//DWORD mode = 0;
	//while (EnumDisplaySettings(NULL, mode++, &dm)) {
	//	string res = std::to_string(dm.dmPelsWidth) + "x" + std::to_string(dm.dmPelsHeight) + " " + std::to_string(dm.dmDisplayFrequency) + "hz";
	//	DEBUG_LOG(res);
	//}
	
	HMODULE instance = GetModuleHandle(NULL); // In the future perhaps use WinMain to get HINSTANCE
	if (!instance) {
		throw std::runtime_error("Failed to get module handle.");
	}

	WNDCLASSEX windowClass;
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_OWNDC; // Might need CS_HREDRAW | CS_VREDRAW not sure yet
	windowClass.lpfnWndProc = MainWndProc;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = instance;
	windowClass.hIcon = NULL; // In the future supply default icon?
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowClass.hbrBackground = NULL;
	windowClass.lpszMenuName = NULL;
	windowClass.lpszClassName = "MainWindow";
	windowClass.hIconSm = NULL;

	ATOM windowAtom = RegisterClassEx(&windowClass);
	if (!windowAtom) {
		throw std::runtime_error("Failed to register window class.");
	}

	// Center window
	int32 x = CW_USEDEFAULT;
	int32 y = CW_USEDEFAULT;

	if (dm.dmPelsWidth > width && dm.dmPelsHeight > height) {
		x = (dm.dmPelsWidth - width) / 2;
		y = (dm.dmPelsHeight - height) / 2;
	}

	m_handle = CreateWindowEx(0,
		"MainWindow",
		title.c_str(),
		WS_OVERLAPPEDWINDOW,          
		x,
		y,
		width,
		height,
		(HWND)NULL,            // no parent or owner window    
		(HMENU)NULL,           // class menu used              
		instance,              // instance handle              
		NULL);

	if (!m_handle) {
		throw std::runtime_error("Failed to create window.");
	}

	ShowWindow(m_handle, SW_SHOWDEFAULT);
	UpdateWindow(m_handle);

	// Initialize OpengGL
	m_deviceContext = GetDC(m_handle);

	// Setup pixel format
	PIXELFORMATDESCRIPTOR pfd;
	ZeroMemory(&pfd, sizeof(pfd));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 8;
	pfd.iLayerType = PFD_MAIN_PLANE;

	int format = ChoosePixelFormat(m_deviceContext, &pfd);
	if (!format) {
		throw std::runtime_error("Failed to choose pixel format.");
	}
	
	if (SetPixelFormat(m_deviceContext, format, &pfd) == FALSE) {
		throw std::runtime_error("Failed to set pixel format.");
	}

	m_openglRenderContext = wglCreateContext(m_deviceContext);
	
	if (wglMakeCurrent(m_deviceContext, m_openglRenderContext) == FALSE) {
		throw std::runtime_error("Failed to set opengl context.");
	}
}


Window::~Window() {
	// TODO error checking
	if (!m_shouldClose) {
		DestroyWindow(m_handle);
	}
	wglDeleteContext(m_openglRenderContext);
	ReleaseDC(m_handle, m_deviceContext);
}

void Window::setTitle(const string& title) {
	m_title = title;
	SetWindowText(m_handle, title.c_str());
}

void Window::swapBuffers() const {
	SwapBuffers(m_deviceContext);
}

bool Window::shouldClose() const {
	return m_shouldClose;
}

void Window::pollEvents() {
	static MSG msg;
	while (PeekMessage(&msg, m_handle, 0,0, PM_REMOVE) == TRUE) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

static void keyCallback(int32 key, int32 scancode, int32 action, int32 mods) {
	getInputInstance()->keyCallback(key, scancode, action, mods);
}

static void cursorPositionCallback(float64 xPos, float64 yPos) {
	getInputInstance()->cursorPositionCallback(xPos, yPos);
}

static void mouseButtonCallback(int32 button, int32 action, int32 mods) {
	getInputInstance()->mouseButtonCallback(button, action, mods);
}

static void scrollCallback(float64 xOffset, float64 yOffset) {
	getInputInstance()->scrollCallback(xOffset, yOffset);
}

#endif