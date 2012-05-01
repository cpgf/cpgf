#define GLUT_DISABLE_ATEXIT_HACK

#include "cpgf/gcompiler.h"
#include "cpgf/gmetadefine.h"

#if defined(_WIN32)
    #include <windows.h>
#endif

#ifdef G_OS_APPLE
    #include <GLUT/glut.h>
#else
    #include <GL/glut.h>
#endif

#include "cpgf/metadata/opengl/gmetadata_openglut.h"

#include "cpgf/gapi.h"
#include "cpgf/scriptbind/gscriptbindapi.h"
#include "cpgf/scriptbind/gscriptbindutil.h"

#include <map>

using namespace std;

namespace cpgf {


namespace metadata_internal {

namespace {

struct GlutScriptCallbackData
{
	GSharedInterface<IScriptFunction> display; // for glutDisplayFunc
	GSharedInterface<IScriptFunction> reshape; // for glutReshapeFunc
	GSharedInterface<IScriptFunction> keyboard; // for glutKeyboardFunc
	GSharedInterface<IScriptFunction> mouse; // for glutMouseFunc
	GSharedInterface<IScriptFunction> motion; // for glutMotionFunc
	GSharedInterface<IScriptFunction> passiveMotion; // for glutPassiveMotionFunc
	GSharedInterface<IScriptFunction> entry; // for glutEntryFunc
	GSharedInterface<IScriptFunction> visibility; // for glutVisibilityFunc
	GSharedInterface<IScriptFunction> idle; // for glutIdleFunc
	GSharedInterface<IScriptFunction> timer; // for glutTimerFunc
	GSharedInterface<IScriptFunction> menuState; // for glutMenuStateFunc

#if (GLUT_API_VERSION >= 2)
	GSharedInterface<IScriptFunction> special; // for glutSpecialFunc
	GSharedInterface<IScriptFunction> spaceballMotion; // for glutSpaceballMotionFunc
	GSharedInterface<IScriptFunction> spaceballRotate; // for glutSpaceballRotateFunc
	GSharedInterface<IScriptFunction> spaceballButton; // for glutSpaceballButtonFunc
	GSharedInterface<IScriptFunction> buttonBox; // for glutButtonBoxFunc
	GSharedInterface<IScriptFunction> dials; // for glutDialsFunc
	GSharedInterface<IScriptFunction> tabletMotion; // for glutTabletMotionFunc
	GSharedInterface<IScriptFunction> tabletButton; // for glutTabletButtonFunc
#if (GLUT_API_VERSION >= 3)
	GSharedInterface<IScriptFunction> overlayDisplay; // for glutOverlayDisplayFunc
	GSharedInterface<IScriptFunction> menuStatus; // for glutMenuStatusFunc
#if (GLUT_API_VERSION >= 4 || GLUT_XLIB_IMPLEMENTATION >= 9)
	GSharedInterface<IScriptFunction> windowStatus; // for glutWindowStatusFunc
#endif
#if (GLUT_API_VERSION >= 4 || GLUT_XLIB_IMPLEMENTATION >= 13)
	GSharedInterface<IScriptFunction> keyboardUp; // for glutKeyboardUpFunc
	GSharedInterface<IScriptFunction> specialUp; // for glutSpecialUpFunc
	GSharedInterface<IScriptFunction> joystick; // for glutJoystickFunc
#endif
#endif
#endif
};

typedef map<int, GlutScriptCallbackData> GlutCallbackMap;

GScopedPointer<GlutCallbackMap> callbackMap;

GlutScriptCallbackData * getCurrentCallbackData()
{
	if(! callbackMap) {
		callbackMap.reset(new GlutCallbackMap);
	}

	int currentWindow = glutGetWindow();
	return &(*callbackMap)[currentWindow];
}

} // unnamed namespace


void helper_glutInit()
{
	int n = 1;
	char c[] = "abc";
	char * pc = c;
	glutInit(&n, &pc);
}

void callback_glutDisplayFunc()
{
	invokeScriptFunction(getCurrentCallbackData()->display.get());
}

void helper_glutDisplayFunc(IScriptFunction * scriptFunction)
{
	getCurrentCallbackData()->display.reset(scriptFunction);
	glutDisplayFunc(&callback_glutDisplayFunc);
}

void callback_glutReshapeFunc(int width, int height)
{
	invokeScriptFunction(getCurrentCallbackData()->reshape.get(), width, height);
}

void helper_glutReshapeFunc(IScriptFunction * scriptFunction)
{
	getCurrentCallbackData()->reshape.reset(scriptFunction);
	glutReshapeFunc(&callback_glutReshapeFunc);
}

void callback_glutKeyboardFunc(unsigned char key, int x, int y)
{
	invokeScriptFunction(getCurrentCallbackData()->keyboard.get(), key, x, y);
}

void helper_glutKeyboardFunc(IScriptFunction * scriptFunction)
{
	getCurrentCallbackData()->keyboard.reset(scriptFunction);
	glutKeyboardFunc(&callback_glutKeyboardFunc);
}

void callback_glutMouseFunc(int button, int state, int x, int y)
{
	invokeScriptFunction(getCurrentCallbackData()->mouse.get(), button, state, x, y);
}

void helper_glutMouseFunc(IScriptFunction * scriptFunction)
{
	getCurrentCallbackData()->mouse.reset(scriptFunction);
	glutMouseFunc(&callback_glutMouseFunc);
}

void callback_glutMotionFunc(int x, int y)
{
	invokeScriptFunction(getCurrentCallbackData()->motion.get(), x, y);
}

void helper_glutMotionFunc(IScriptFunction * scriptFunction)
{
	getCurrentCallbackData()->motion.reset(scriptFunction);
	glutMotionFunc(&callback_glutMotionFunc);
}

void callback_glutPassiveMotionFunc(int x, int y)
{
	invokeScriptFunction(getCurrentCallbackData()->passiveMotion.get(), x, y);
}

void helper_glutPassiveMotionFunc(IScriptFunction * scriptFunction)
{
	getCurrentCallbackData()->passiveMotion.reset(scriptFunction);
	glutPassiveMotionFunc(&callback_glutPassiveMotionFunc);
}

void callback_glutEntryFunc(int state)
{
	invokeScriptFunction(getCurrentCallbackData()->entry.get(), state);
}

void helper_glutEntryFunc(IScriptFunction * scriptFunction)
{
	getCurrentCallbackData()->entry.reset(scriptFunction);
	glutEntryFunc(&callback_glutEntryFunc);
}

void callback_glutVisibilityFunc(int state)
{
	invokeScriptFunction(getCurrentCallbackData()->visibility.get(), state);
}

void helper_glutVisibilityFunc(IScriptFunction * scriptFunction)
{
	getCurrentCallbackData()->visibility.reset(scriptFunction);
	glutVisibilityFunc(&callback_glutVisibilityFunc);
}

void callback_glutIdleFunc()
{
	invokeScriptFunction(getCurrentCallbackData()->idle.get());
}

void helper_glutIdleFunc(IScriptFunction * scriptFunction)
{
	getCurrentCallbackData()->idle.reset(scriptFunction);
	glutIdleFunc(&callback_glutIdleFunc);
}

void callback_glutTimerFunc(int value)
{
	invokeScriptFunction(getCurrentCallbackData()->timer.get(), value);
}

void helper_glutTimerFunc(unsigned int millis, IScriptFunction * scriptFunction, int value)
{
	getCurrentCallbackData()->timer.reset(scriptFunction);
	glutTimerFunc(millis, &callback_glutTimerFunc, value);
}

void callback_glutMenuStateFunc(int state)
{
	invokeScriptFunction(getCurrentCallbackData()->menuState.get(), state);
}

void helper_glutMenuStateFunc(IScriptFunction * scriptFunction)
{
	getCurrentCallbackData()->menuState.reset(scriptFunction);
	glutMenuStateFunc(&callback_glutMenuStateFunc);
}

#if (GLUT_API_VERSION >= 2)
void callback_glutSpecialFunc(int key, int x, int y)
{
	invokeScriptFunction(getCurrentCallbackData()->special.get(), key, x, y);
}

void helper_glutSpecialFunc(IScriptFunction * scriptFunction)
{
	getCurrentCallbackData()->special.reset(scriptFunction);
	glutSpecialFunc(&callback_glutSpecialFunc);
}

void callback_glutSpaceballMotionFunc(int x, int y, int z)
{
	invokeScriptFunction(getCurrentCallbackData()->spaceballMotion.get(), x, y, z);
}

void helper_glutSpaceballMotionFunc(IScriptFunction * scriptFunction)
{
	getCurrentCallbackData()->spaceballMotion.reset(scriptFunction);
	glutSpaceballMotionFunc(&callback_glutSpaceballMotionFunc);
}

void callback_glutSpaceballRotateFunc(int x, int y, int z)
{
	invokeScriptFunction(getCurrentCallbackData()->spaceballRotate.get(), x, y, z);
}

void helper_glutSpaceballRotateFunc(IScriptFunction * scriptFunction)
{
	getCurrentCallbackData()->spaceballRotate.reset(scriptFunction);
	glutSpaceballRotateFunc(&callback_glutSpaceballRotateFunc);
}

void callback_glutSpaceballButtonFunc(int button, int state)
{
	invokeScriptFunction(getCurrentCallbackData()->spaceballButton.get(), button, state);
}

void helper_glutSpaceballButtonFunc(IScriptFunction * scriptFunction)
{
	getCurrentCallbackData()->spaceballButton.reset(scriptFunction);
	glutSpaceballButtonFunc(&callback_glutSpaceballButtonFunc);
}

void callback_glutButtonBoxFunc(int button, int state)
{
	invokeScriptFunction(getCurrentCallbackData()->buttonBox.get(), button, state);
}

void helper_glutButtonBoxFunc(IScriptFunction * scriptFunction)
{
	getCurrentCallbackData()->buttonBox.reset(scriptFunction);
	glutButtonBoxFunc(&callback_glutButtonBoxFunc);
}

void callback_glutDialsFunc(int dial, int value)
{
	invokeScriptFunction(getCurrentCallbackData()->dials.get(), dial, value);
}

void helper_glutDialsFunc(IScriptFunction * scriptFunction)
{
	getCurrentCallbackData()->dials.reset(scriptFunction);
	glutDialsFunc(&callback_glutDialsFunc);
}

void callback_glutTabletMotionFunc(int x, int y)
{
	invokeScriptFunction(getCurrentCallbackData()->tabletMotion.get(), x, y);
}

void helper_glutTabletMotionFunc(IScriptFunction * scriptFunction)
{
	getCurrentCallbackData()->tabletMotion.reset(scriptFunction);
	glutTabletMotionFunc(&callback_glutTabletMotionFunc);
}

void callback_glutTabletButtonFunc(int button, int state, int x, int y)
{
	invokeScriptFunction(getCurrentCallbackData()->tabletButton.get(), button, state, x, y);
}

void helper_glutTabletButtonFunc(IScriptFunction * scriptFunction)
{
	getCurrentCallbackData()->tabletButton.reset(scriptFunction);
	glutTabletButtonFunc(&callback_glutTabletButtonFunc);
}

#if (GLUT_API_VERSION >= 3)
void callback_glutMenuStatusFunc(int status, int x, int y)
{
	invokeScriptFunction(getCurrentCallbackData()->menuStatus.get(), status, x, y);
}

void helper_glutMenuStatusFunc(IScriptFunction * scriptFunction)
{
	getCurrentCallbackData()->menuStatus.reset(scriptFunction);
	glutMenuStatusFunc(&callback_glutMenuStatusFunc);
}

void callback_glutOverlayDisplayFunc()
{
	invokeScriptFunction(getCurrentCallbackData()->overlayDisplay.get());
}

void helper_glutOverlayDisplayFunc(IScriptFunction * scriptFunction)
{
	getCurrentCallbackData()->overlayDisplay.reset(scriptFunction);
	glutOverlayDisplayFunc(&callback_glutOverlayDisplayFunc);
}

#if (GLUT_API_VERSION >= 4 || GLUT_XLIB_IMPLEMENTATION >= 9)
void callback_glutWindowStatusFunc(int state)
{
	invokeScriptFunction(getCurrentCallbackData()->windowStatus.get(), state);
}

void helper_glutWindowStatusFunc(IScriptFunction * scriptFunction)
{
	getCurrentCallbackData()->windowStatus.reset(scriptFunction);
	glutWindowStatusFunc(&callback_glutWindowStatusFunc);
}

#endif
#if (GLUT_API_VERSION >= 4 || GLUT_XLIB_IMPLEMENTATION >= 13)
void callback_glutKeyboardUpFunc(unsigned char key, int x, int y)
{
	invokeScriptFunction(getCurrentCallbackData()->keyboardUp.get(), key, x, y);
}

void helper_glutKeyboardUpFunc(IScriptFunction * scriptFunction)
{
	getCurrentCallbackData()->keyboardUp.reset(scriptFunction);
	glutKeyboardUpFunc(&callback_glutKeyboardUpFunc);
}

void callback_glutSpecialUpFunc(int key, int x, int y)
{
	invokeScriptFunction(getCurrentCallbackData()->specialUp.get(), key, x, y);
}

void helper_glutSpecialUpFunc(IScriptFunction * scriptFunction)
{
	getCurrentCallbackData()->specialUp.reset(scriptFunction);
	glutSpecialUpFunc(&callback_glutSpecialUpFunc);
}

void callback_glutJoystickFunc(unsigned int buttonMask, int x, int y, int z)
{
	invokeScriptFunction(getCurrentCallbackData()->joystick.get(), buttonMask, x, y, z);
}

void helper_glutJoystickFunc(IScriptFunction * scriptFunction, int pollInterval)
{
	getCurrentCallbackData()->joystick.reset(scriptFunction);
	glutJoystickFunc(&callback_glutJoystickFunc, pollInterval);
}

#endif
#endif
#endif



} // namespace metadata_internal

} // namespace cpgf
