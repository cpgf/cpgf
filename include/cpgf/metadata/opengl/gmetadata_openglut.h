#ifndef __GMETADATA_OPENGLUT_H
#define __GMETADATA_OPENGLUT_H


#include "cpgf/metadata/gmetadataconfig.h"

#include "cpgf/scriptbind/gscriptbindapi.h"


#include "cpgf/metadata/private/gmetadata_header.h"


namespace cpgf {

namespace metadata_internal {

void helper_glutDisplayFunc(IScriptFunction * scriptFunction);
void helper_glutReshapeFunc(IScriptFunction * scriptFunction);
void helper_glutKeyboardFunc(IScriptFunction * scriptFunction);
void helper_glutMouseFunc(IScriptFunction * scriptFunction);
void helper_glutMotionFunc(IScriptFunction * scriptFunction);
void helper_glutPassiveMotionFunc(IScriptFunction * scriptFunction);
void helper_glutEntryFunc(IScriptFunction * scriptFunction);
void helper_glutVisibilityFunc(IScriptFunction * scriptFunction);
void helper_glutIdleFunc(IScriptFunction * scriptFunction);
void helper_glutTimerFunc(unsigned int millis, IScriptFunction * scriptFunction, int value);
void helper_glutMenuStateFunc(IScriptFunction * scriptFunction);
#if (GLUT_API_VERSION >= 2)
void helper_glutSpecialFunc(IScriptFunction * scriptFunction);
void helper_glutSpaceballMotionFunc(IScriptFunction * scriptFunction);
void helper_glutSpaceballRotateFunc(IScriptFunction * scriptFunction);
void helper_glutSpaceballButtonFunc(IScriptFunction * scriptFunction);
void helper_glutButtonBoxFunc(IScriptFunction * scriptFunction);
void helper_glutDialsFunc(IScriptFunction * scriptFunction);
void helper_glutTabletMotionFunc(IScriptFunction * scriptFunction);
void helper_glutTabletButtonFunc(IScriptFunction * scriptFunction);
#if (GLUT_API_VERSION >= 3)
void helper_glutMenuStatusFunc(IScriptFunction * scriptFunction);
void helper_glutOverlayDisplayFunc(IScriptFunction * scriptFunction);
#if (GLUT_API_VERSION >= 4 || GLUT_XLIB_IMPLEMENTATION >= 9)
void helper_glutWindowStatusFunc(IScriptFunction * scriptFunction);
#endif
#if (GLUT_API_VERSION >= 4 || GLUT_XLIB_IMPLEMENTATION >= 13)
void helper_glutKeyboardUpFunc(IScriptFunction * scriptFunction);
void helper_glutSpecialUpFunc(IScriptFunction * scriptFunction);
void helper_glutJoystickFunc(IScriptFunction * scriptFunction, int pollInterval);
#endif
#endif
#endif


template <typename MetaDefine>
void doBuildMetaData_open_glut_constants(const GMetaDataConfigFlags & /*config*/, MetaDefine define)
{
	define.CPGF_MD_TEMPLATE _enum<int>("glut_enum")

		GENUM_VALUE(GLUT_API_VERSION)
		GENUM_VALUE(GLUT_XLIB_IMPLEMENTATION)

		/* Display mode bit masks. */
		GENUM_VALUE(GLUT_RGB)
		GENUM_VALUE(GLUT_RGBA)
		GENUM_VALUE(GLUT_INDEX)
		GENUM_VALUE(GLUT_SINGLE)
		GENUM_VALUE(GLUT_DOUBLE)
		GENUM_VALUE(GLUT_ACCUM)
		GENUM_VALUE(GLUT_ALPHA)
		GENUM_VALUE(GLUT_DEPTH)
		GENUM_VALUE(GLUT_STENCIL)
		#if (GLUT_API_VERSION >= 2)
		GENUM_VALUE(GLUT_MULTISAMPLE)
		GENUM_VALUE(GLUT_STEREO)
		#endif
		#if (GLUT_API_VERSION >= 3)
		GENUM_VALUE(GLUT_LUMINANCE)
		#endif

		/* Mouse buttons. */
		GENUM_VALUE(GLUT_LEFT_BUTTON)
		GENUM_VALUE(GLUT_MIDDLE_BUTTON)
		GENUM_VALUE(GLUT_RIGHT_BUTTON)

		/* Mouse button  state. */
		GENUM_VALUE(GLUT_DOWN)
		GENUM_VALUE(GLUT_UP)

		#if (GLUT_API_VERSION >= 2)
		/* function keys */
		GENUM_VALUE(GLUT_KEY_F1)
		GENUM_VALUE(GLUT_KEY_F2)
		GENUM_VALUE(GLUT_KEY_F3)
		GENUM_VALUE(GLUT_KEY_F4)
		GENUM_VALUE(GLUT_KEY_F5)
		GENUM_VALUE(GLUT_KEY_F6)
		GENUM_VALUE(GLUT_KEY_F7)
		GENUM_VALUE(GLUT_KEY_F8)
		GENUM_VALUE(GLUT_KEY_F9)
		GENUM_VALUE(GLUT_KEY_F10)
		GENUM_VALUE(GLUT_KEY_F11)
		GENUM_VALUE(GLUT_KEY_F12)
		/* directional keys */
		GENUM_VALUE(GLUT_KEY_LEFT)
		GENUM_VALUE(GLUT_KEY_UP)
		GENUM_VALUE(GLUT_KEY_RIGHT)
		GENUM_VALUE(GLUT_KEY_DOWN)
		GENUM_VALUE(GLUT_KEY_PAGE_UP)
		GENUM_VALUE(GLUT_KEY_PAGE_DOWN)
		GENUM_VALUE(GLUT_KEY_HOME)
		GENUM_VALUE(GLUT_KEY_END)
		GENUM_VALUE(GLUT_KEY_INSERT)
		#endif

		/* Entry/exit  state. */
		GENUM_VALUE(GLUT_LEFT)
		GENUM_VALUE(GLUT_ENTERED)

		/* Menu usage  state. */
		GENUM_VALUE(GLUT_MENU_NOT_IN_USE)
		GENUM_VALUE(GLUT_MENU_IN_USE)

		/* Visibility  state. */
		GENUM_VALUE(GLUT_NOT_VISIBLE)
		GENUM_VALUE(GLUT_VISIBLE)

		/* Window status  state. */
		GENUM_VALUE(GLUT_HIDDEN)
		GENUM_VALUE(GLUT_FULLY_RETAINED)
		GENUM_VALUE(GLUT_PARTIALLY_RETAINED)
		GENUM_VALUE(GLUT_FULLY_COVERED)

		/* Color index component selection values. */
		GENUM_VALUE(GLUT_RED)
		GENUM_VALUE(GLUT_GREEN)
		GENUM_VALUE(GLUT_BLUE)

		#if defined(_WIN32)
		/* Stroke font constants (use these in GLUT program). */
		GENUM_VALUE(GLUT_STROKE_ROMAN)
		GENUM_VALUE(GLUT_STROKE_MONO_ROMAN)

		/* Bitmap font constants (use these in GLUT program). */
		GENUM_VALUE(GLUT_BITMAP_9_BY_15)
		GENUM_VALUE(GLUT_BITMAP_8_BY_13)
		GENUM_VALUE(GLUT_BITMAP_TIMES_ROMAN_10)
		GENUM_VALUE(GLUT_BITMAP_TIMES_ROMAN_24)
		#if (GLUT_API_VERSION >= 3)
		GENUM_VALUE(GLUT_BITMAP_HELVETICA_10)
		GENUM_VALUE(GLUT_BITMAP_HELVETICA_12)
		GENUM_VALUE(GLUT_BITMAP_HELVETICA_18)
		#endif
		#else

		/* Stroke font constants (use these in GLUT program). */
		GENUM_VALUE(GLUT_STROKE_ROMAN)
		GENUM_VALUE(GLUT_STROKE_MONO_ROMAN)


		/* Bitmap font constants (use these in GLUT program). */
		GENUM_VALUE(GLUT_BITMAP_9_BY_15)
		GENUM_VALUE(GLUT_BITMAP_8_BY_13)
		GENUM_VALUE(GLUT_BITMAP_TIMES_ROMAN_10)
		GENUM_VALUE(GLUT_BITMAP_TIMES_ROMAN_24)
		#if (GLUT_API_VERSION >= 3)
		GENUM_VALUE(GLUT_BITMAP_HELVETICA_10)
		GENUM_VALUE(GLUT_BITMAP_HELVETICA_12)
		GENUM_VALUE(GLUT_BITMAP_HELVETICA_18)
		#endif
		#endif

		/* glutGet parameters. */
		GENUM_VALUE(GLUT_WINDOW_X)
		GENUM_VALUE(GLUT_WINDOW_Y)
		GENUM_VALUE(GLUT_WINDOW_WIDTH)
		GENUM_VALUE(GLUT_WINDOW_HEIGHT)
		GENUM_VALUE(GLUT_WINDOW_BUFFER_SIZE)
		GENUM_VALUE(GLUT_WINDOW_STENCIL_SIZE)
		GENUM_VALUE(GLUT_WINDOW_DEPTH_SIZE)
		GENUM_VALUE(GLUT_WINDOW_RED_SIZE)
		GENUM_VALUE(GLUT_WINDOW_GREEN_SIZE)
		GENUM_VALUE(GLUT_WINDOW_BLUE_SIZE)
		GENUM_VALUE(GLUT_WINDOW_ALPHA_SIZE)
		GENUM_VALUE(GLUT_WINDOW_ACCUM_RED_SIZE)
		GENUM_VALUE(GLUT_WINDOW_ACCUM_GREEN_SIZE)
		GENUM_VALUE(GLUT_WINDOW_ACCUM_BLUE_SIZE)
		GENUM_VALUE(GLUT_WINDOW_ACCUM_ALPHA_SIZE)
		GENUM_VALUE(GLUT_WINDOW_DOUBLEBUFFER)
		GENUM_VALUE(GLUT_WINDOW_RGBA)
		GENUM_VALUE(GLUT_WINDOW_PARENT)
		GENUM_VALUE(GLUT_WINDOW_NUM_CHILDREN)
		GENUM_VALUE(GLUT_WINDOW_COLORMAP_SIZE)
		#if (GLUT_API_VERSION >= 2)
		GENUM_VALUE(GLUT_WINDOW_NUM_SAMPLES)
		GENUM_VALUE(GLUT_WINDOW_STEREO)
		#endif
		#if (GLUT_API_VERSION >= 3)
		GENUM_VALUE(GLUT_WINDOW_CURSOR)
		#endif
		GENUM_VALUE(GLUT_SCREEN_WIDTH)
		GENUM_VALUE(GLUT_SCREEN_HEIGHT)
		GENUM_VALUE(GLUT_SCREEN_WIDTH_MM)
		GENUM_VALUE(GLUT_SCREEN_HEIGHT_MM)
		GENUM_VALUE(GLUT_MENU_NUM_ITEMS)
		GENUM_VALUE(GLUT_DISPLAY_MODE_POSSIBLE)
		GENUM_VALUE(GLUT_INIT_WINDOW_X)
		GENUM_VALUE(GLUT_INIT_WINDOW_Y)
		GENUM_VALUE(GLUT_INIT_WINDOW_WIDTH)
		GENUM_VALUE(GLUT_INIT_WINDOW_HEIGHT)
		GENUM_VALUE(GLUT_INIT_DISPLAY_MODE)
		#if (GLUT_API_VERSION >= 2)
		GENUM_VALUE(GLUT_ELAPSED_TIME)
		#endif
		#if (GLUT_API_VERSION >= 4 || GLUT_XLIB_IMPLEMENTATION >= 13)
		GENUM_VALUE(GLUT_WINDOW_FORMAT_ID)
		#endif

		#if (GLUT_API_VERSION >= 2)
		/* glutDeviceGet parameters. */
		GENUM_VALUE(GLUT_HAS_KEYBOARD)
		GENUM_VALUE(GLUT_HAS_MOUSE)
		GENUM_VALUE(GLUT_HAS_SPACEBALL)
		GENUM_VALUE(GLUT_HAS_DIAL_AND_BUTTON_BOX)
		GENUM_VALUE(GLUT_HAS_TABLET)
		GENUM_VALUE(GLUT_NUM_MOUSE_BUTTONS)
		GENUM_VALUE(GLUT_NUM_SPACEBALL_BUTTONS)
		GENUM_VALUE(GLUT_NUM_BUTTON_BOX_BUTTONS)
		GENUM_VALUE(GLUT_NUM_DIALS)
		GENUM_VALUE(GLUT_NUM_TABLET_BUTTONS)
		#endif
		#if (GLUT_API_VERSION >= 4 || GLUT_XLIB_IMPLEMENTATION >= 13)
		GENUM_VALUE(GLUT_DEVICE_IGNORE_KEY_REPEAT)
		GENUM_VALUE(GLUT_DEVICE_KEY_REPEAT)
		GENUM_VALUE(GLUT_HAS_JOYSTICK)
		GENUM_VALUE(GLUT_OWNS_JOYSTICK)
		GENUM_VALUE(GLUT_JOYSTICK_BUTTONS)
		GENUM_VALUE(GLUT_JOYSTICK_AXES)
		GENUM_VALUE(GLUT_JOYSTICK_POLL_RATE)
		#endif

		#if (GLUT_API_VERSION >= 3)
		/* glutLayerGet parameters. */
		GENUM_VALUE(GLUT_OVERLAY_POSSIBLE)
		GENUM_VALUE(GLUT_LAYER_IN_USE)
		GENUM_VALUE(GLUT_HAS_OVERLAY)
		GENUM_VALUE(GLUT_TRANSPARENT_INDEX)
		GENUM_VALUE(GLUT_NORMAL_DAMAGED)
		GENUM_VALUE(GLUT_OVERLAY_DAMAGED)

		#if (GLUT_API_VERSION >= 4 || GLUT_XLIB_IMPLEMENTATION >= 9)
		/* glutVideoResizeGet parameters. */
		GENUM_VALUE(GLUT_VIDEO_RESIZE_POSSIBLE)
		GENUM_VALUE(GLUT_VIDEO_RESIZE_IN_USE)
		GENUM_VALUE(GLUT_VIDEO_RESIZE_X_DELTA)
		GENUM_VALUE(GLUT_VIDEO_RESIZE_Y_DELTA)
		GENUM_VALUE(GLUT_VIDEO_RESIZE_WIDTH_DELTA)
		GENUM_VALUE(GLUT_VIDEO_RESIZE_HEIGHT_DELTA)
		GENUM_VALUE(GLUT_VIDEO_RESIZE_X)
		GENUM_VALUE(GLUT_VIDEO_RESIZE_Y)
		GENUM_VALUE(GLUT_VIDEO_RESIZE_WIDTH)
		GENUM_VALUE(GLUT_VIDEO_RESIZE_HEIGHT)
		#endif

		/* glutUseLayer parameters. */
		GENUM_VALUE(GLUT_NORMAL)
		GENUM_VALUE(GLUT_OVERLAY)

		/* glutGetModifiers return mask. */
		GENUM_VALUE(GLUT_ACTIVE_SHIFT)
		GENUM_VALUE(GLUT_ACTIVE_CTRL)
		GENUM_VALUE(GLUT_ACTIVE_ALT)

		/* glutSetCursor parameters. */
		/* Basic arrows. */
		GENUM_VALUE(GLUT_CURSOR_RIGHT_ARROW)
		GENUM_VALUE(GLUT_CURSOR_LEFT_ARROW)
		/* Symbolic cursor shapes. */
		GENUM_VALUE(GLUT_CURSOR_INFO)
		GENUM_VALUE(GLUT_CURSOR_DESTROY)
		GENUM_VALUE(GLUT_CURSOR_HELP)
		GENUM_VALUE(GLUT_CURSOR_CYCLE)
		GENUM_VALUE(GLUT_CURSOR_SPRAY)
		GENUM_VALUE(GLUT_CURSOR_WAIT)
		GENUM_VALUE(GLUT_CURSOR_TEXT)
		GENUM_VALUE(GLUT_CURSOR_CROSSHAIR)
		/* Directional cursors. */
		GENUM_VALUE(GLUT_CURSOR_UP_DOWN)
		GENUM_VALUE(GLUT_CURSOR_LEFT_RIGHT)
		/* Sizing cursors. */
		GENUM_VALUE(GLUT_CURSOR_TOP_SIDE)
		GENUM_VALUE(GLUT_CURSOR_BOTTOM_SIDE)
		GENUM_VALUE(GLUT_CURSOR_LEFT_SIDE)
		GENUM_VALUE(GLUT_CURSOR_RIGHT_SIDE)
		GENUM_VALUE(GLUT_CURSOR_TOP_LEFT_CORNER)
		GENUM_VALUE(GLUT_CURSOR_TOP_RIGHT_CORNER)
		GENUM_VALUE(GLUT_CURSOR_BOTTOM_RIGHT_CORNER)
		GENUM_VALUE(GLUT_CURSOR_BOTTOM_LEFT_CORNER)
		/* Inherit from parent window. */
		GENUM_VALUE(GLUT_CURSOR_INHERIT)
		/* Blank cursor. */
		GENUM_VALUE(GLUT_CURSOR_NONE)
		/* Fullscreen crosshair (if available). */
		GENUM_VALUE(GLUT_CURSOR_FULL_CROSSHAIR)
		#endif


		#if (GLUT_API_VERSION >= 4 || GLUT_XLIB_IMPLEMENTATION >= 13)
		/* GLUT device control sub-API. */
		/* glutSetKeyRepeat modes. */
		GENUM_VALUE(GLUT_KEY_REPEAT_OFF)
		GENUM_VALUE(GLUT_KEY_REPEAT_ON)
		GENUM_VALUE(GLUT_KEY_REPEAT_DEFAULT)

		/* Joystick button masks. */
		GENUM_VALUE(GLUT_JOYSTICK_BUTTON_A)
		GENUM_VALUE(GLUT_JOYSTICK_BUTTON_B)
		GENUM_VALUE(GLUT_JOYSTICK_BUTTON_C)
		GENUM_VALUE(GLUT_JOYSTICK_BUTTON_D)

		/* GLUT game mode sub-API. */
		/* glutGameModeGet. */
		GENUM_VALUE(GLUT_GAME_MODE_ACTIVE)
		GENUM_VALUE(GLUT_GAME_MODE_POSSIBLE)
		GENUM_VALUE(GLUT_GAME_MODE_WIDTH)
		GENUM_VALUE(GLUT_GAME_MODE_HEIGHT)
		GENUM_VALUE(GLUT_GAME_MODE_PIXEL_DEPTH)
		GENUM_VALUE(GLUT_GAME_MODE_REFRESH_RATE)
		GENUM_VALUE(GLUT_GAME_MODE_DISPLAY_CHANGED)
		#endif
	;
}

template <typename MetaDefine>
void doBuildMetaData_open_glut_functions(const GMetaDataConfigFlags & /*config*/, MetaDefine define)
{
	/* GLUT initialization sub-API. */
	GM(glutInit)
	#if defined(_WIN32) && !defined(GLUT_DISABLE_ATEXIT_HACK)
	GM(__glutInitWithExit)
	#ifndef GLUT_BUILDING_LIB
	GM(glutInit_ATEXIT_HACK)
	#define glutInit glutInit_ATEXIT_HACK
	#endif
	#endif
	GM(glutInitDisplayMode)
	#if (GLUT_API_VERSION >= 4 || GLUT_XLIB_IMPLEMENTATION >= 9)
	GM(glutInitDisplayString)
	#endif
	GM(glutInitWindowPosition)
	GM(glutInitWindowSize)
	GM(glutMainLoop)

	/* GLUT window sub-API. */
	GM(glutCreateWindow)
	#if defined(_WIN32) && !defined(GLUT_DISABLE_ATEXIT_HACK)
	GM(__glutCreateWindowWithExit)
	#ifndef GLUT_BUILDING_LIB
	GM(glutCreateWindow_ATEXIT_HACK)
	#define glutCreateWindow glutCreateWindow_ATEXIT_HACK
	#endif
	#endif
	GM(glutCreateSubWindow)
	GM(glutDestroyWindow)
	GM(glutPostRedisplay)
	#if (GLUT_API_VERSION >= 4 || GLUT_XLIB_IMPLEMENTATION >= 11)
	GM(glutPostWindowRedisplay)
	#endif
	GM(glutSwapBuffers)
	GM(glutGetWindow)
	GM(glutSetWindow)
	GM(glutSetWindowTitle)
	GM(glutSetIconTitle)
	GM(glutPositionWindow)
	GM(glutReshapeWindow)
	GM(glutPopWindow)
	GM(glutPushWindow)
	GM(glutIconifyWindow)
	GM(glutShowWindow)
	GM(glutHideWindow)
	#if (GLUT_API_VERSION >= 3)
	GM(glutFullScreen)
	GM(glutSetCursor)
	#if (GLUT_API_VERSION >= 4 || GLUT_XLIB_IMPLEMENTATION >= 9)
	GM(glutWarpPointer)
	#endif

	/* GLUT overlay sub-API. */
	GM(glutEstablishOverlay)
	GM(glutRemoveOverlay)
	GM(glutUseLayer)
	GM(glutPostOverlayRedisplay)
	#if (GLUT_API_VERSION >= 4 || GLUT_XLIB_IMPLEMENTATION >= 11)
	GM(glutPostWindowOverlayRedisplay)
	#endif
	GM(glutShowOverlay)
	GM(glutHideOverlay)
	#endif

	/* GLUT menu sub-API. */
	GM(glutCreateMenu)
	#if defined(_WIN32) && !defined(GLUT_DISABLE_ATEXIT_HACK)
	GM(__glutCreateMenuWithExit)
	#ifndef GLUT_BUILDING_LIB
	GM(glutCreateMenu_ATEXIT_HACK)
	#define glutCreateMenu glutCreateMenu_ATEXIT_HACK
	#endif
	#endif
	GM(glutDestroyMenu)
	GM(glutGetMenu)
	GM(glutSetMenu)
	GM(glutAddMenuEntry)
	GM(glutAddSubMenu)
	GM(glutChangeToMenuEntry)
	GM(glutChangeToSubMenu)
	GM(glutRemoveMenuItem)
	GM(glutAttachMenu)
	GM(glutDetachMenu)

	/* GLUT window callback sub-API. */
	GM(glutDisplayFunc)
	GM(glutReshapeFunc)
	GM(glutKeyboardFunc)
	GM(glutMouseFunc)
	GM(glutMotionFunc)
	GM(glutPassiveMotionFunc)
	GM(glutEntryFunc)
	GM(glutVisibilityFunc)
	GM(glutIdleFunc)
	GM(glutTimerFunc)
	GM(glutMenuStateFunc)
	#if (GLUT_API_VERSION >= 2)
	GM(glutSpecialFunc)
	GM(glutSpaceballMotionFunc)
	GM(glutSpaceballRotateFunc)
	GM(glutSpaceballButtonFunc)
	GM(glutButtonBoxFunc)
	GM(glutDialsFunc)
	GM(glutTabletMotionFunc)
	GM(glutTabletButtonFunc)
	#if (GLUT_API_VERSION >= 3)
	GM(glutMenuStatusFunc)
	GM(glutOverlayDisplayFunc)
	#if (GLUT_API_VERSION >= 4 || GLUT_XLIB_IMPLEMENTATION >= 9)
	GM(glutWindowStatusFunc)
	#endif
	#if (GLUT_API_VERSION >= 4 || GLUT_XLIB_IMPLEMENTATION >= 13)
	GM(glutKeyboardUpFunc)
	GM(glutSpecialUpFunc)
	GM(glutJoystickFunc)
	#endif
	#endif
	#endif

	/* GLUT color index sub-API. */
	GM(glutSetColor)
	GM(glutGetColor)
	GM(glutCopyColormap)

	/* GLUT state retrieval sub-API. */
	GM(glutGet)
	GM(glutDeviceGet)
	#if (GLUT_API_VERSION >= 2)
	/* GLUT extension support sub-API */
	GM(glutExtensionSupported)
	#endif
	#if (GLUT_API_VERSION >= 3)
	GM(glutGetModifiers)
	GM(glutLayerGet)
	#endif

	/* GLUT font sub-API */
	GM(glutBitmapCharacter)
	GM(glutBitmapWidth)
	GM(glutStrokeCharacter)
	GM(glutStrokeWidth)
	#if (GLUT_API_VERSION >= 4 || GLUT_XLIB_IMPLEMENTATION >= 9)
	GM(glutBitmapLength)
	GM(glutStrokeLength)
	#endif

	/* GLUT pre-built models sub-API */
	GM(glutWireSphere)
	GM(glutSolidSphere)
	GM(glutWireCone)
	GM(glutSolidCone)
	GM(glutWireCube)
	GM(glutSolidCube)
	GM(glutWireTorus)
	GM(glutSolidTorus)
	GM(glutWireDodecahedron)
	GM(glutSolidDodecahedron)
	GM(glutWireTeapot)
	GM(glutSolidTeapot)
	GM(glutWireOctahedron)
	GM(glutSolidOctahedron)
	GM(glutWireTetrahedron)
	GM(glutSolidTetrahedron)
	GM(glutWireIcosahedron)
	GM(glutSolidIcosahedron)

	#if (GLUT_API_VERSION >= 4 || GLUT_XLIB_IMPLEMENTATION >= 9)
	/* GLUT video resize sub-API. */
	GM(glutVideoResizeGet)
	GM(glutSetupVideoResizing)
	GM(glutStopVideoResizing)
	GM(glutVideoResize)
	GM(glutVideoPan)

	/* GLUT debugging sub-API. */
	GM(glutReportErrors)
	#endif

	#if (GLUT_API_VERSION >= 4 || GLUT_XLIB_IMPLEMENTATION >= 13)

	GM(glutIgnoreKeyRepeat)
	GM(glutSetKeyRepeat)
	GM(glutForceJoystickFunc)

	GM(glutGameModeString)
	GM(glutEnterGameMode)
	GM(glutLeaveGameMode)
	GM(glutGameModeGet)
	#endif
}

void helper_glutInit();

template <typename MetaDefine>
void doBuildMetaData_open_glut_helpers(const GMetaDataConfigFlags & config, MetaDefine define)
{
	if(! metaDataConfigIsScriptable(config)) {
		return;
	}

	define.CPGF_MD_TEMPLATE _method("glutInit", &helper_glutInit);

	define.CPGF_MD_TEMPLATE _method("glutDisplayFunc", &helper_glutDisplayFunc);
	define.CPGF_MD_TEMPLATE _method("glutReshapeFunc", &helper_glutReshapeFunc);
	define.CPGF_MD_TEMPLATE _method("glutKeyboardFunc", &helper_glutKeyboardFunc);
	define.CPGF_MD_TEMPLATE _method("glutMouseFunc", &helper_glutMouseFunc);
	define.CPGF_MD_TEMPLATE _method("glutMotionFunc", &helper_glutMotionFunc);
	define.CPGF_MD_TEMPLATE _method("glutPassiveMotionFunc", &helper_glutPassiveMotionFunc);
	define.CPGF_MD_TEMPLATE _method("glutEntryFunc", &helper_glutEntryFunc);
	define.CPGF_MD_TEMPLATE _method("glutVisibilityFunc", &helper_glutVisibilityFunc);
	define.CPGF_MD_TEMPLATE _method("glutIdleFunc", &helper_glutIdleFunc);
	define.CPGF_MD_TEMPLATE _method("glutTimerFunc", &helper_glutTimerFunc);
	define.CPGF_MD_TEMPLATE _method("glutMenuStateFunc", &helper_glutMenuStateFunc);
	#if (GLUT_API_VERSION >= 2)
	define.CPGF_MD_TEMPLATE _method("glutSpecialFunc", &helper_glutSpecialFunc);
	define.CPGF_MD_TEMPLATE _method("glutSpaceballMotionFunc", &helper_glutSpaceballMotionFunc);
	define.CPGF_MD_TEMPLATE _method("glutSpaceballRotateFunc", &helper_glutSpaceballRotateFunc);
	define.CPGF_MD_TEMPLATE _method("glutSpaceballButtonFunc", &helper_glutSpaceballButtonFunc);
	define.CPGF_MD_TEMPLATE _method("glutButtonBoxFunc", &helper_glutButtonBoxFunc);
	define.CPGF_MD_TEMPLATE _method("glutDialsFunc", &helper_glutDialsFunc);
	define.CPGF_MD_TEMPLATE _method("glutTabletMotionFunc", &helper_glutTabletMotionFunc);
	define.CPGF_MD_TEMPLATE _method("glutTabletButtonFunc", &helper_glutTabletButtonFunc);
	#if (GLUT_API_VERSION >= 3)
	define.CPGF_MD_TEMPLATE _method("glutMenuStatusFunc", &helper_glutMenuStatusFunc);
	define.CPGF_MD_TEMPLATE _method("glutOverlayDisplayFunc", &helper_glutOverlayDisplayFunc);
	#if (GLUT_API_VERSION >= 4 || GLUT_XLIB_IMPLEMENTATION >= 9)
	define.CPGF_MD_TEMPLATE _method("glutWindowStatusFunc", &helper_glutWindowStatusFunc);
	#endif
	#if (GLUT_API_VERSION >= 4 || GLUT_XLIB_IMPLEMENTATION >= 13)
	define.CPGF_MD_TEMPLATE _method("glutKeyboardUpFunc", &helper_glutKeyboardUpFunc);
	define.CPGF_MD_TEMPLATE _method("glutSpecialUpFunc", &helper_glutSpecialUpFunc);
	define.CPGF_MD_TEMPLATE _method("glutJoystickFunc", &helper_glutJoystickFunc);
	#endif
	#endif
	#endif
		
}

template <typename MetaDefine>
void doBuildMetaData_open_glut(const GMetaDataConfigFlags & config, MetaDefine define)
{
	doBuildMetaData_open_glut_constants(config, define);
	doBuildMetaData_open_glut_functions(config, define);
	doBuildMetaData_open_glut_helpers(config, define);
}


} // namespace metadata_internal


template <typename MetaDefine>
void buildMetaData_open_glut_constants(const GMetaDataConfigFlags & config, MetaDefine define)
{
	metadata_internal::doBuildMetaData_open_glut_constants(config, define);
}

template <typename MetaDefine>
void buildMetaData_open_glut_constants(MetaDefine define)
{
	buildMetaData_open_glut_constants(mdcScriptable | mdcAutoProperty, define);
}


template <typename MetaDefine>
void buildMetaData_open_glut_functions(const GMetaDataConfigFlags & config, MetaDefine define)
{
	metadata_internal::doBuildMetaData_open_glut_functions(config, define);
}

template <typename MetaDefine>
void buildMetaData_open_glut_functions(MetaDefine define)
{
	buildMetaData_open_glut_functions(mdcScriptable | mdcAutoProperty, define);
}


template <typename MetaDefine>
void buildMetaData_open_glut_helpers(const GMetaDataConfigFlags & config, MetaDefine define)
{
	metadata_internal::doBuildMetaData_open_glut_helpers(config, define);
}

template <typename MetaDefine>
void buildMetaData_open_glut_helpers(MetaDefine define)
{
	buildMetaData_open_glut_helpers(mdcScriptable | mdcAutoProperty, define);
}


// all in one register
template <typename MetaDefine>
void buildMetaData_open_glut(const GMetaDataConfigFlags & config, MetaDefine define)
{
	metadata_internal::doBuildMetaData_open_glut(config, define);
}

template <typename MetaDefine>
void buildMetaData_open_glut(MetaDefine define)
{
	buildMetaData_open_glut(mdcScriptable | mdcAutoProperty, define);
}


} // namespace cpgf


#include "cpgf/metadata/private/gmetadata_footer.h"


#endif

