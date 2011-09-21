#include <windows.h>
#include <windowsx.h>
#include <gl/gl.h>
#include <gl/glu.h>

#include "cpgf/game/gmockup.h"
#include "cpgf/game/gapplication.h"
#include "cpgf/game/grenderengine.h"
#include "cpgf/game/g2d/grenderengine2d.h"
#include "cpgf/game/ginput.h"


namespace cpgf {

const char *windowClassName = "g_opengl_win";
const int WM_MAINLOOP = WM_USER + 1;

class WindowHandles
{
public:
	WindowHandles() : handleDC(NULL), glContext(NULL), winHandle(NULL), handleInstance(NULL), fullScreen(false), messageWindowHandle(NULL) {
	}

public:
	HDC handleDC;
	HGLRC glContext;
	HWND winHandle;
	HINSTANCE handleInstance;
	BOOL fullScreen;

	HWND messageWindowHandle;
};

WindowHandles winHandles;

bool	active = true;

LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

class private_GEngineAccess
{
public:
	static void startEngine() {
		gEngine->startEngine();
	}

	static void stopEngine() {
		gEngine->stopEngine();
	}

	static void deactivateEngine() {
		gEngine->deactivateEngine();
	}

	static void activateEngine() {
		gEngine->activateEngine();
	}

	static void updateEngine() {
		gEngine->updateEngine();
	}

	static void drawEngine() {
		gEngine->drawEngine();
	}

	static void resizeViewport(int width, int height) {
		if(gEngine) {
			gEngine->resizeViewport(width, height);
		}
	}
};

GLvoid ReSizeGLScene(GLsizei width, GLsizei height)
{
	if(width == 0) {
		width = 1;
	}

	if(height == 0) {
		height = 1;
	}

	private_GEngineAccess::resizeViewport(width, height);
}

int InitGL()
{
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
	glClearDepth(1.0f);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);

	return true;
}

GLvoid KillGLWindow()
{
	if(winHandles.fullScreen) {
		ChangeDisplaySettings(NULL, 0);
		ShowCursor(true);
	}

	if(winHandles.glContext) {
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(winHandles.glContext);
		winHandles.glContext = NULL;
	}

	if(winHandles.handleDC) {
		ReleaseDC(winHandles.winHandle, winHandles.handleDC);
		winHandles.handleDC = NULL;
	}

	if(winHandles.winHandle) {
		DestroyWindow(winHandles.winHandle);
		winHandles.winHandle = NULL;
	}

	UnregisterClassA(windowClassName, winHandles.handleInstance);
}

void TraceWinError()
{
	int error = GetLastError();

	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		error,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR) &lpMsgBuf,
		0,
		NULL
	);

	MessageBoxA( NULL, (LPCSTR)lpMsgBuf, "Error", MB_OK | MB_ICONINFORMATION );

	LocalFree( lpMsgBuf );
}

bool registerWindowClass(const char *className, WNDPROC proc)
{
	WNDCLASSA wc;

	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc		= proc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= winHandles.handleInstance;
	wc.hIcon			= NULL;
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground	= NULL;
	wc.lpszMenuName		= NULL;
	wc.lpszClassName	= className;

	return RegisterClassA(&wc) != 0;
}

bool changeToFullscreen(const GAppInitConfig *config)
{
	DEVMODE dmScreenSettings;
	memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
	dmScreenSettings.dmSize = sizeof(dmScreenSettings);
	dmScreenSettings.dmPelsWidth = config->width;
	dmScreenSettings.dmPelsHeight = config->height;
	dmScreenSettings.dmBitsPerPel = config->bitsPerPixel;
	dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;


	return ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN) == DISP_CHANGE_SUCCESSFUL;
}

bool initDeviceContext(const GAppInitConfig *config)
{
	GLuint PixelFormat;

	static PIXELFORMATDESCRIPTOR pfd =	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		config->bitsPerPixel,
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		16,
		0,
		0,
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};

	winHandles.handleDC = GetDC(winHandles.winHandle);
	if(!winHandles.handleDC) {
		KillGLWindow();
		return false;
	}

	PixelFormat = ChoosePixelFormat(winHandles.handleDC, &pfd);
	if(!PixelFormat) {
		KillGLWindow();
		return false;
	}

	if(!SetPixelFormat(winHandles.handleDC,PixelFormat, &pfd)) {
		KillGLWindow();
		return false;
	}

	return true;
}

bool initWindow(const GAppInitConfig *config)
{
	DWORD dwExStyle;
	DWORD dwStyle;

	if(config->fullScreen) {
		dwExStyle = WS_EX_APPWINDOW;
		dwStyle = WS_POPUP;
//		ShowCursor(false);
	}
	else {
		dwExStyle = 0; //WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		dwStyle = WS_OVERLAPPEDWINDOW;
	}

	dwStyle |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	RECT winRect;
	winRect.left = 0;
	winRect.right = config->width;
	winRect.top = 0;
	winRect.bottom = config->height;

	AdjustWindowRectEx(&winRect, dwStyle, false, dwExStyle);


	winHandles.winHandle = CreateWindowExA(dwExStyle, windowClassName, "", dwStyle,
								0, 0, winRect.right - winRect.left, winRect.bottom - winRect.top,
								NULL, NULL,	winHandles.handleInstance, NULL);
	if(!winHandles.winHandle) {
		KillGLWindow();
		return false;
	}

	return true;
}

BOOL CreateGLWindow(const GAppInitConfig *config)
{
	winHandles.handleInstance = GetModuleHandle(NULL);

	if(!registerWindowClass(windowClassName, (WNDPROC)WndProc)) {
		return false;
	}

	if(config->fullScreen) {
		if(!changeToFullscreen(config)) {
			return false;
		}
	}

	if(!initWindow(config)) {
		return false;
	}

	if(!initDeviceContext(config)) {
		return false;
	}

	winHandles.glContext = wglCreateContext(winHandles.handleDC);
	if(!winHandles.glContext) {
		KillGLWindow();
		return false;
	}

	if(!wglMakeCurrent(winHandles.handleDC, winHandles.glContext)) {
		KillGLWindow();
		return false;
	}

	ReSizeGLScene(config->width, config->height);

	if(!InitGL()) {
		KillGLWindow();
		return false;
	}

	return true;
}

int translateKeyCode(int key)
{
	switch(key) {
		case VK_ESCAPE:
			return keyEscape;

		case VK_UP:
			return keyUp;

		case VK_LEFT:
			return keyLeft;

		case VK_RIGHT:
			return keyRight;

		case VK_DOWN:
			return keyDown;

		case VK_SPACE:
			return keySpace;

		default:
			return -1;
	}
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message) {
		case WM_ACTIVATE:
			if(!HIWORD(wParam)) {
				active = true;
			}
			else {
				active = false;
			}

			return 0;

		case WM_SYSCOMMAND:
			switch (wParam)
			{
				case SC_SCREENSAVE:
				case SC_MONITORPOWER:
				return 0;
			}
			break;

//		case WM_CLOSE:
		case WM_DESTROY:
			private_GEngineAccess::stopEngine();
			return 0;

		case WM_SIZE:
			ReSizeGLScene(LOWORD(lParam), HIWORD(lParam));
			return 0;

		case WM_KEYDOWN:
		{
			int key = translateKeyCode(wParam);
			if(key >= 0) {
				GRawInput::keyDown(key);
			}
		}
			return 0;

		case WM_KEYUP:
		{
			if(wParam == VK_F2) {
				private_GEngineAccess::stopEngine();
				return 0;
			}

			if(wParam == VK_F3) {
				private_GEngineAccess::startEngine();
				return 0;
			}

			int key = translateKeyCode(wParam);
			if(key >= 0) {
				GRawInput::keyRelease(key);
			}
		}
			return 0;

		case WM_MOUSEMOVE:
			GRawInput::touchMove(keyInvalid, static_cast<gcoord>(GET_X_LPARAM(lParam)), static_cast<gcoord>(GET_Y_LPARAM(lParam)));
			return 0;

		case WM_LBUTTONDOWN:
			GRawInput::touchDown(keyMouseLeft, static_cast<gcoord>(GET_X_LPARAM(lParam)), static_cast<gcoord>(GET_Y_LPARAM(lParam)));
			return 0;

		case WM_LBUTTONUP:
			GRawInput::touchUp(keyMouseLeft, static_cast<gcoord>(GET_X_LPARAM(lParam)), static_cast<gcoord>(GET_Y_LPARAM(lParam)));
			return 0;

		case WM_MBUTTONDOWN:
			GRawInput::touchDown(keyMouseMiddle, static_cast<gcoord>(GET_X_LPARAM(lParam)), static_cast<gcoord>(GET_Y_LPARAM(lParam)));
			return 0;

		case WM_MBUTTONUP:
			GRawInput::touchUp(keyMouseMiddle, static_cast<gcoord>(GET_X_LPARAM(lParam)), static_cast<gcoord>(GET_Y_LPARAM(lParam)));
			return 0;

		case WM_RBUTTONDOWN:
			GRawInput::touchDown(keyMouseRight, static_cast<gcoord>(GET_X_LPARAM(lParam)), static_cast<gcoord>(GET_Y_LPARAM(lParam)));
			return 0;

		case WM_RBUTTONUP:
			GRawInput::touchUp(keyMouseRight, static_cast<gcoord>(GET_X_LPARAM(lParam)), static_cast<gcoord>(GET_Y_LPARAM(lParam)));
			return 0;
	}


	return DefWindowProc(hWnd, message, wParam, lParam);
}

LRESULT CALLBACK messageWindowWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message) {
		case WM_MAINLOOP:
			private_GEngineAccess::updateEngine();
			private_GEngineAccess::drawEngine();

			SwapBuffers(winHandles.handleDC);
			break;
	};

	return DefWindowProc(hWnd, message, wParam, lParam);
}

bool createMessageWindow()
{
	if(winHandles.messageWindowHandle != NULL) {
		return true;
	}

	const char *className = "g_msg_win";

	if(!registerWindowClass(className, (WNDPROC)messageWindowWndProc)) {
		return false;
	}

	winHandles.messageWindowHandle = CreateWindowExA(0, className, "", WS_POPUP,
								0, 0, 0, 0,
								NULL, NULL,	winHandles.handleInstance, NULL);
	if(!winHandles.messageWindowHandle) {
		return false;
	}

	return true;
}

DWORD WINAPI mainLoopThread(LPVOID lpParameter)
{
	while(true) {
		if(winHandles.winHandle == 0 || winHandles.messageWindowHandle == 0) {
			break;
		}

		Sleep(gConfig.frameDuration);
		SendMessage(winHandles.messageWindowHandle, WM_MAINLOOP, NULL, NULL);
	}

	return 0;
}

bool gInitializeApplication(GAppInitConfig *config)
{
	if(!CreateGLWindow(config)) {
		return false;
	}

	GEngine::createEngine(config->engineHandler, new GOpenGLRenderEngineOpenGL(), new GDevice(), new GInput());

	SetParent(winHandles.winHandle, config->parentHandle);
	ShowWindow(winHandles.winHandle, SW_SHOW);
	SetFocus(winHandles.winHandle);

	private_GEngineAccess::startEngine();

	return true;
}

bool gStartupMainLoop()
{
	if(!createMessageWindow()) {
		return false;
	}

	DWORD threadID;
	CreateThread(NULL, 0, mainLoopThread, NULL, 0, &threadID);

	return true;
}

gmillisecond getMillisecond()
{
	return GetTickCount();
}


} // namespace cpgf
