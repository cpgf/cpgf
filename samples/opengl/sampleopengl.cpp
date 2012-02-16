#include "cpgf/gmetadefine.h"

#include "../../test/scriptbind/bind_common.h"

#include <iostream>
#include <string.h>

using namespace cpgf;
using namespace std;

void registerOpenGL(GMetaClass * metaClass);
void registerOpenGLU(GMetaClass * metaClass);
void registerOpenGLUT(GMetaClass * metaClass);
void registerOpenGLUT2(GMetaClass * metaClass);
void registerOpenGLUT3(GMetaClass * metaClass);

//    v6----- v5
//   /|      /|
//  v1------v0|
//  | |     | |
//  | |v7---|-|v4
//  |/      |/
//  v2------v3
const char * luaCode = ""
	"rotate = 0 \n"
	"translateX = 0 \n"
	"translateY = 0 \n"
	"vertexList = 0 \n"
	"indexList = 0 \n"
	"colorList = 0 \n"
	"normalList = 0 \n"
	"function initData() \n"
		"vertexList = gl.createByteArray() \n"
		"indexList = gl.createByteArray() \n"
		"colorList = gl.createByteArray() \n"
		"normalList = gl.createByteArray() \n"

		"vertexList.writeFloat32(0.4) \n"  "vertexList.writeFloat32(0.4) \n"  "vertexList.writeFloat32(0.4) \n"
		"vertexList.writeFloat32(-0.4) \n" "vertexList.writeFloat32(0.4) \n"  "vertexList.writeFloat32(0.4) \n"
		"vertexList.writeFloat32(-0.4) \n" "vertexList.writeFloat32(-0.4) \n" "vertexList.writeFloat32(0.4) \n"
		"vertexList.writeFloat32(0.4) \n"  "vertexList.writeFloat32(-0.4) \n" "vertexList.writeFloat32(0.4) \n"
		"vertexList.writeFloat32(0.4) \n"  "vertexList.writeFloat32(-0.4) \n" "vertexList.writeFloat32(-0.4) \n"
		"vertexList.writeFloat32(0.4) \n"  "vertexList.writeFloat32(0.4) \n"  "vertexList.writeFloat32(-0.4) \n"
		"vertexList.writeFloat32(-0.4) \n" "vertexList.writeFloat32(0.4) \n"  "vertexList.writeFloat32(-0.4) \n"
		"vertexList.writeFloat32(-0.4) \n" "vertexList.writeFloat32(-0.4) \n" "vertexList.writeFloat32(-0.4) \n"

		"indexList.writeInt32(0) \n" "indexList.writeInt32(1) \n" "indexList.writeInt32(2) \n" "indexList.writeInt32(3) \n"
		"indexList.writeInt32(0) \n" "indexList.writeInt32(3) \n" "indexList.writeInt32(4) \n" "indexList.writeInt32(5) \n"
		"indexList.writeInt32(4) \n" "indexList.writeInt32(5) \n" "indexList.writeInt32(6) \n" "indexList.writeInt32(7) \n"
		"indexList.writeInt32(1) \n" "indexList.writeInt32(2) \n" "indexList.writeInt32(7) \n" "indexList.writeInt32(6) \n"
		"indexList.writeInt32(0) \n" "indexList.writeInt32(1) \n" "indexList.writeInt32(6) \n" "indexList.writeInt32(5) \n"
		"indexList.writeInt32(2) \n" "indexList.writeInt32(3) \n" "indexList.writeInt32(4) \n" "indexList.writeInt32(7) \n"

		"colorList.writeFloat32(1) \n"  "colorList.writeFloat32(0) \n"  "colorList.writeFloat32(0) \n"
		"colorList.writeFloat32(0) \n" "colorList.writeFloat32(1) \n"  "colorList.writeFloat32(0) \n"
		"colorList.writeFloat32(0) \n" "colorList.writeFloat32(0) \n" "colorList.writeFloat32(1) \n"
		"colorList.writeFloat32(1) \n"  "colorList.writeFloat32(1) \n" "colorList.writeFloat32(0) \n"
		"colorList.writeFloat32(1) \n"  "colorList.writeFloat32(0) \n" "colorList.writeFloat32(1) \n"
		"colorList.writeFloat32(0) \n"  "colorList.writeFloat32(1) \n"  "colorList.writeFloat32(1) \n"
		"colorList.writeFloat32(1) \n" "colorList.writeFloat32(1) \n"  "colorList.writeFloat32(1) \n"
		"colorList.writeFloat32(0) \n" "colorList.writeFloat32(0) \n" "colorList.writeFloat32(0) \n"

		"normalList.writeFloat32(1) \n"  "normalList.writeFloat32(0) \n"  "normalList.writeFloat32(0) \n"
		"normalList.writeFloat32(0) \n" "normalList.writeFloat32(1) \n"  "normalList.writeFloat32(0) \n"
		"normalList.writeFloat32(0) \n" "normalList.writeFloat32(0) \n" "normalList.writeFloat32(1) \n"
		"normalList.writeFloat32(0) \n"  "normalList.writeFloat32(0) \n" "normalList.writeFloat32(-1) \n"
		"normalList.writeFloat32(0) \n"  "normalList.writeFloat32(-1) \n" "normalList.writeFloat32(0) \n"
		"normalList.writeFloat32(-1) \n"  "normalList.writeFloat32(0) \n"  "normalList.writeFloat32(0) \n"
		"normalList.writeFloat32(-1) \n" "normalList.writeFloat32(1) \n"  "normalList.writeFloat32(0) \n"
		"normalList.writeFloat32(0) \n" "normalList.writeFloat32(-1) \n" "normalList.writeFloat32(-1) \n"
	"end \n"
	"function drawString(str) \n"
		"gl.glPushAttrib(gl.GL_LIGHTING_BIT + gl.GL_CURRENT_BIT) \n"
		"gl.glDisable(gl.GL_LIGHTING) \n"
		"local positionList = gl.createByteArray() \n"
		"positionList.writeFloat32(-0.6) \n"  "positionList.writeFloat32(-0.5) \n"  "positionList.writeFloat32(0.5) \n"
		"gl.glRasterPos3fv(positionList) \n"
		"for i=1, #str do \n"
			"gl.glutBitmapCharacter(gl.GLUT_BITMAP_TIMES_ROMAN_24, str:byte(i)) \n"
		"end \n"
		"gl.glEnable(gl.GL_LIGHTING) \n"
		"gl.glPopAttrib() \n"
	"end \n"
	"function render() \n"
		"gl.glClear(gl.GL_COLOR_BUFFER_BIT + gl.GL_DEPTH_BUFFER_BIT + gl.GL_STENCIL_BUFFER_BIT) \n"

		"gl.glEnableClientState(gl.GL_VERTEX_ARRAY) \n"
		"gl.glEnableClientState(gl.GL_COLOR_ARRAY) \n"
		"gl.glEnableClientState(gl.GL_NORMAL_ARRAY) \n"
	    "gl.glPushMatrix() \n"
		"gl.glTranslatef(translateX, translateY, 0) \n"
		"gl.glRotatef(rotate, 1, 0, 1) \n"
		"gl.glVertexPointer(3, gl.GL_FLOAT, 0, vertexList) \n"
		"gl.glColorPointer(3, gl.GL_FLOAT, 0, colorList) \n"
		"gl.glNormalPointer(gl.GL_FLOAT, 0, normalList) \n"
		"gl.glDrawElements(gl.GL_QUADS, 24, gl.GL_UNSIGNED_INT, indexList) \n"
		"drawString(\"Lua+OpenGL=Whoa!\") \n"
	    "gl.glPopMatrix() \n"
		"gl.glDisableClientState(gl.GL_VERTEX_ARRAY) \n"
		"gl.glDisableClientState(gl.GL_COLOR_ARRAY) \n"
		"gl.glDisableClientState(gl.GL_NORMAL_ARRAY) \n"
		"gl.glutSwapBuffers() \n"
	"end \n"
	"function reshape(w, h) \n"
		"gl.glViewport(0, 0, w, h) \n"
		"gl.glMatrixMode(gl.GL_MODELVIEW) \n"
	"end \n"
	"function specialKey(key, x, y) \n"
		"local delta = 0.02 \n"
		"if(key == gl.GLUT_KEY_LEFT) then \n"
		"translateX = translateX - delta \n"
		"elseif(key == gl.GLUT_KEY_RIGHT) then \n"
		"translateX = translateX + delta \n"
		"elseif(key == gl.GLUT_KEY_UP) then \n"
		"translateY = translateY + delta \n"
		"elseif(key == gl.GLUT_KEY_DOWN) then \n"
		"translateY = translateY - delta \n"
		"end \n"
	"end \n"
	"function keyboard(key, x, y) \n"
		"if(key == 27) then exitDemo() end \n"
	"end \n"
	"function timer(value) \n"
		"rotate = rotate + 1 \n"
		"rotate = rotate % 360 \n"
		"gl.glutPostRedisplay() \n"
		"gl.glutTimerFunc(value, timer, value) \n"
	"end \n"
	"function initLights() \n"
		"lightAmbient = gl.createByteArray() \n"
		"lightAmbient.writeFloat32(0.6) \n" "lightAmbient.writeFloat32(0.6) \n" "lightAmbient.writeFloat32(0.6) \n" "lightAmbient.writeFloat32(1.0) \n"
		"lightDiffuse = gl.createByteArray() \n"
		"lightDiffuse.writeFloat32(0.7) \n" "lightDiffuse.writeFloat32(0.7) \n" "lightDiffuse.writeFloat32(0.7) \n" "lightDiffuse.writeFloat32(1.0) \n"
		"lightSpecular = gl.createByteArray() \n"
		"lightSpecular.writeFloat32(1) \n" "lightSpecular.writeFloat32(1) \n" "lightSpecular.writeFloat32(1) \n" "lightSpecular.writeFloat32(1) \n"
		"lightPosition = gl.createByteArray() \n"
		"lightPosition.writeFloat32(0) \n" "lightPosition.writeFloat32(1) \n" "lightPosition.writeFloat32(-1) \n" "lightPosition.writeFloat32(1) \n"
		"gl.glLightfv(gl.GL_LIGHT0, gl.GL_AMBIENT, lightAmbient) \n"
		"gl.glLightfv(gl.GL_LIGHT0, gl.GL_DIFFUSE, lightDiffuse) \n"
		"gl.glLightfv(gl.GL_LIGHT0, gl.GL_SPECULAR, lightSpecular) \n"
		"gl.glLightfv(gl.GL_LIGHT0, gl.GL_POSITION, lightPosition) \n"
		"gl.glEnable(gl.GL_LIGHT0) \n"
	"end \n"
	"function start() \n"
		"initData() \n"
		"gl.glutInit() \n"
		"gl.glutInitDisplayMode(gl.GLUT_DEPTH + gl.GLUT_DOUBLE + gl.GLUT_RGB + gl.GLUT_STENCIL) \n"
		"gl.glutInitWindowPosition(100, 100) \n"
		"gl.glutInitWindowSize(320, 320) \n"
		"gl.glutCreateWindow(\"cpgf OpenGL binding demo\") \n"
		"gl.glutDisplayFunc(render) \n"
		"gl.glutReshapeFunc(reshape) \n"
		"gl.glutSpecialFunc(specialKey) \n"
	    "gl.glutKeyboardFunc(keyboard) \n"
		"period = 33 \n"
		"gl.glutTimerFunc(period, timer, period) \n"

		"gl.glShadeModel(gl.GL_SMOOTH) \n"
	    "gl.glHint(gl.GL_PERSPECTIVE_CORRECTION_HINT, gl.GL_NICEST) \n"
		"gl.glHint(gl.GL_LINE_SMOOTH_HINT, gl.GL_NICEST) \n"
		"gl.glHint(gl.GL_POLYGON_SMOOTH_HINT, gl.GL_NICEST) \n"

	    "gl.glEnable(gl.GL_DEPTH_TEST) \n"
		"gl.glEnable(gl.GL_LIGHTING) \n"
		"gl.glEnable(gl.GL_TEXTURE_2D) \n"
		"gl.glEnable(gl.GL_CULL_FACE) \n"

		"gl.glColorMaterial(gl.GL_FRONT_AND_BACK, gl.GL_AMBIENT_AND_DIFFUSE) \n"
		"gl.glEnable(gl.GL_COLOR_MATERIAL) \n"
	    "gl.glClearColor(0, 0, 0, 0) \n"
		"gl.glClearStencil(0) \n"
		"gl.glClearDepth(1.0) \n"
		"gl.glDepthFunc(gl.GL_LEQUAL) \n"

		"initLights() \n"
		"gl.glutMainLoop() \n"
	"end \n"
;

const char * jsCode = ""
	"rotate = 0 \n"
	"translateX = 0 \n"
	"translateY = 0 \n"
	"vertexList = 0 \n"
	"indexList = 0 \n"
	"colorList = 0 \n"
	"normalList = 0 \n"
	"function initData() { \n"
		"vertexList = gl.createByteArray() \n"
		"indexList = gl.createByteArray() \n"
		"colorList = gl.createByteArray() \n"
		"normalList = gl.createByteArray() \n"

		"vertexList.writeFloat32(0.4) \n"  "vertexList.writeFloat32(0.4) \n"  "vertexList.writeFloat32(0.4) \n"
		"vertexList.writeFloat32(-0.4) \n" "vertexList.writeFloat32(0.4) \n"  "vertexList.writeFloat32(0.4) \n"
		"vertexList.writeFloat32(-0.4) \n" "vertexList.writeFloat32(-0.4) \n" "vertexList.writeFloat32(0.4) \n"
		"vertexList.writeFloat32(0.4) \n"  "vertexList.writeFloat32(-0.4) \n" "vertexList.writeFloat32(0.4) \n"
		"vertexList.writeFloat32(0.4) \n"  "vertexList.writeFloat32(-0.4) \n" "vertexList.writeFloat32(-0.4) \n"
		"vertexList.writeFloat32(0.4) \n"  "vertexList.writeFloat32(0.4) \n"  "vertexList.writeFloat32(-0.4) \n"
		"vertexList.writeFloat32(-0.4) \n" "vertexList.writeFloat32(0.4) \n"  "vertexList.writeFloat32(-0.4) \n"
		"vertexList.writeFloat32(-0.4) \n" "vertexList.writeFloat32(-0.4) \n" "vertexList.writeFloat32(-0.4) \n"

		"indexList.writeInt32(0) \n" "indexList.writeInt32(1) \n" "indexList.writeInt32(2) \n" "indexList.writeInt32(3) \n"
		"indexList.writeInt32(0) \n" "indexList.writeInt32(3) \n" "indexList.writeInt32(4) \n" "indexList.writeInt32(5) \n"
		"indexList.writeInt32(4) \n" "indexList.writeInt32(5) \n" "indexList.writeInt32(6) \n" "indexList.writeInt32(7) \n"
		"indexList.writeInt32(1) \n" "indexList.writeInt32(2) \n" "indexList.writeInt32(7) \n" "indexList.writeInt32(6) \n"
		"indexList.writeInt32(0) \n" "indexList.writeInt32(1) \n" "indexList.writeInt32(6) \n" "indexList.writeInt32(5) \n"
		"indexList.writeInt32(2) \n" "indexList.writeInt32(3) \n" "indexList.writeInt32(4) \n" "indexList.writeInt32(7) \n"

		"colorList.writeFloat32(1) \n"  "colorList.writeFloat32(0) \n"  "colorList.writeFloat32(0) \n"
		"colorList.writeFloat32(0) \n" "colorList.writeFloat32(1) \n"  "colorList.writeFloat32(0) \n"
		"colorList.writeFloat32(0) \n" "colorList.writeFloat32(0) \n" "colorList.writeFloat32(1) \n"
		"colorList.writeFloat32(1) \n"  "colorList.writeFloat32(1) \n" "colorList.writeFloat32(0) \n"
		"colorList.writeFloat32(1) \n"  "colorList.writeFloat32(0) \n" "colorList.writeFloat32(1) \n"
		"colorList.writeFloat32(0) \n"  "colorList.writeFloat32(1) \n"  "colorList.writeFloat32(1) \n"
		"colorList.writeFloat32(1) \n" "colorList.writeFloat32(1) \n"  "colorList.writeFloat32(1) \n"
		"colorList.writeFloat32(0) \n" "colorList.writeFloat32(0) \n" "colorList.writeFloat32(0) \n"

		"normalList.writeFloat32(1) \n"  "normalList.writeFloat32(0) \n"  "normalList.writeFloat32(0) \n"
		"normalList.writeFloat32(0) \n" "normalList.writeFloat32(1) \n"  "normalList.writeFloat32(0) \n"
		"normalList.writeFloat32(0) \n" "normalList.writeFloat32(0) \n" "normalList.writeFloat32(1) \n"
		"normalList.writeFloat32(0) \n"  "normalList.writeFloat32(0) \n" "normalList.writeFloat32(-1) \n"
		"normalList.writeFloat32(0) \n"  "normalList.writeFloat32(-1) \n" "normalList.writeFloat32(0) \n"
		"normalList.writeFloat32(-1) \n"  "normalList.writeFloat32(0) \n"  "normalList.writeFloat32(0) \n"
		"normalList.writeFloat32(-1) \n" "normalList.writeFloat32(1) \n"  "normalList.writeFloat32(0) \n"
		"normalList.writeFloat32(0) \n" "normalList.writeFloat32(-1) \n" "normalList.writeFloat32(-1) \n"
	"} \n"
	"function drawString(str) { \n"
		"gl.glPushAttrib(gl.GL_LIGHTING_BIT + gl.GL_CURRENT_BIT) \n"
		"gl.glDisable(gl.GL_LIGHTING) \n"
		"var positionList = gl.createByteArray() \n"
		"positionList.writeFloat32(-0.6) \n"  "positionList.writeFloat32(-0.5) \n"  "positionList.writeFloat32(0.5) \n"
		"gl.glRasterPos3fv(positionList) \n"
		"for(i=0; i < str.length; ++i) {\n"
			"gl.glutBitmapCharacter(gl.GLUT_BITMAP_TIMES_ROMAN_24, str.charCodeAt(i)) \n"
		"} \n"
		"gl.glEnable(gl.GL_LIGHTING) \n"
		"gl.glPopAttrib() \n"
	"} \n"
	"function render() { \n"
		"gl.glClear(gl.GL_COLOR_BUFFER_BIT + gl.GL_DEPTH_BUFFER_BIT + gl.GL_STENCIL_BUFFER_BIT) \n"

		"gl.glEnableClientState(gl.GL_VERTEX_ARRAY) \n"
		"gl.glEnableClientState(gl.GL_COLOR_ARRAY) \n"
		"gl.glEnableClientState(gl.GL_NORMAL_ARRAY) \n"
	    "gl.glPushMatrix() \n"
		"gl.glTranslatef(translateX, translateY, 0) \n"
		"gl.glRotatef(rotate, 1, 0, 1) \n"
		"gl.glVertexPointer(3, gl.GL_FLOAT, 0, vertexList) \n"
		"gl.glColorPointer(3, gl.GL_FLOAT, 0, colorList) \n"
		"gl.glNormalPointer(gl.GL_FLOAT, 0, normalList) \n"
		"gl.glDrawElements(gl.GL_QUADS, 24, gl.GL_UNSIGNED_INT, indexList) \n"
		"drawString(\"Javascript+OpenGL=Whoa!\") \n"
	    "gl.glPopMatrix() \n"
		"gl.glDisableClientState(gl.GL_VERTEX_ARRAY) \n"
		"gl.glDisableClientState(gl.GL_COLOR_ARRAY) \n"
		"gl.glDisableClientState(gl.GL_NORMAL_ARRAY) \n"
		"gl.glutSwapBuffers() \n"
	"} \n"
	"function reshape(w, h) { \n"
		"gl.glViewport(0, 0, w, h) \n"
		"gl.glMatrixMode(gl.GL_MODELVIEW) \n"
	"} \n"
	"function specialKey(key, x, y) { \n"
		"delta = 0.02 \n"
		"if(key == gl.GLUT_KEY_LEFT) \n"
		"translateX = translateX - delta \n"
		"else if(key == gl.GLUT_KEY_RIGHT) \n"
		"translateX = translateX + delta \n"
		"else if(key == gl.GLUT_KEY_UP) \n"
		"translateY = translateY + delta \n"
		"else if(key == gl.GLUT_KEY_DOWN) \n"
		"translateY = translateY - delta \n"
		" \n"
	"} \n"
	"function keyboard(key, x, y) { \n"
		"if(key == 27) exitDemo() \n"
	"} \n"
	"function timer(value) { \n"
		"rotate = rotate + 1 \n"
		"rotate = rotate % 360 \n"
		"gl.glutPostRedisplay() \n"
		"gl.glutTimerFunc(value, timer, value) \n"
	"} \n"
	"function initLights() { \n"
		"lightAmbient = gl.createByteArray() \n"
		"lightAmbient.writeFloat32(0.6) \n" "lightAmbient.writeFloat32(0.6) \n" "lightAmbient.writeFloat32(0.6) \n" "lightAmbient.writeFloat32(1.0) \n"
		"lightDiffuse = gl.createByteArray() \n"
		"lightDiffuse.writeFloat32(0.7) \n" "lightDiffuse.writeFloat32(0.7) \n" "lightDiffuse.writeFloat32(0.7) \n" "lightDiffuse.writeFloat32(1.0) \n"
		"lightSpecular = gl.createByteArray() \n"
		"lightSpecular.writeFloat32(1) \n" "lightSpecular.writeFloat32(1) \n" "lightSpecular.writeFloat32(1) \n" "lightSpecular.writeFloat32(1) \n"
		"lightPosition = gl.createByteArray() \n"
		"lightPosition.writeFloat32(0) \n" "lightPosition.writeFloat32(1) \n" "lightPosition.writeFloat32(-1) \n" "lightPosition.writeFloat32(1) \n"
		"gl.glLightfv(gl.GL_LIGHT0, gl.GL_AMBIENT, lightAmbient) \n"
		"gl.glLightfv(gl.GL_LIGHT0, gl.GL_DIFFUSE, lightDiffuse) \n"
		"gl.glLightfv(gl.GL_LIGHT0, gl.GL_SPECULAR, lightSpecular) \n"
		"gl.glLightfv(gl.GL_LIGHT0, gl.GL_POSITION, lightPosition) \n"
		"gl.glEnable(gl.GL_LIGHT0) \n"
	"} \n"
	"function start() { \n"
		"initData() \n"
		"gl.glutInit() \n"
		"gl.glutInitDisplayMode(gl.GLUT_DEPTH + gl.GLUT_DOUBLE + gl.GLUT_RGB + gl.GLUT_STENCIL) \n"
		"gl.glutInitWindowPosition(200, 200) \n"
		"gl.glutInitWindowSize(320, 320) \n"
		"gl.glutCreateWindow(\"cpgf OpenGL binding demo\") \n"
		"gl.glutDisplayFunc(render) \n"
		"gl.glutReshapeFunc(reshape) \n"
		"gl.glutSpecialFunc(specialKey) \n"
	    "gl.glutKeyboardFunc(keyboard) \n"
		"period = 33 \n"
		"gl.glutTimerFunc(period, timer, period) \n"

		"gl.glShadeModel(gl.GL_SMOOTH) \n"
	    "gl.glHint(gl.GL_PERSPECTIVE_CORRECTION_HINT, gl.GL_NICEST) \n"
		"gl.glHint(gl.GL_LINE_SMOOTH_HINT, gl.GL_NICEST) \n"
		"gl.glHint(gl.GL_POLYGON_SMOOTH_HINT, gl.GL_NICEST) \n"

	    "gl.glEnable(gl.GL_DEPTH_TEST) \n"
		"gl.glEnable(gl.GL_LIGHTING) \n"
		"gl.glEnable(gl.GL_TEXTURE_2D) \n"
		"gl.glEnable(gl.GL_CULL_FACE) \n"

		"gl.glColorMaterial(gl.GL_FRONT_AND_BACK, gl.GL_AMBIENT_AND_DIFFUSE) \n"
		"gl.glEnable(gl.GL_COLOR_MATERIAL) \n"
	    "gl.glClearColor(0, 0, 0, 0) \n"
		"gl.glClearStencil(0) \n"
		"gl.glClearDepth(1.0) \n"
		"gl.glDepthFunc(gl.GL_LEQUAL) \n"

		"initLights() \n"
		"gl.glutMainLoop() \n"
	"} \n"
;


template <typename Binding>
void start(Binding * binding, TestScriptContext * c, GMetaClass * glMetaClass)
{
	GScopedPointer<TestScriptContext> context(c);

	GScopedInterface<IMetaClass> metaClass(static_cast<IMetaClass *>(metaItemToInterface(glMetaClass)));
	binding->bindClass("gl", metaClass.get());

	GScopedInterface<IMetaMethod> method(static_cast<IMetaMethod *>(metaItemToInterface(getGlobalMetaClass()->getMethod("exitDemo"))));
	binding->bindMethod("exitDemo", NULL, method.get());

	if(context->isLua()) {
		context->doString(luaCode);
	}

	if(context->isV8()) {
		context->doString(jsCode);
	}

	invokeScriptFunction(binding, "start");

	// should not come here;
	context.reset();
};

void exitDemo()
{
	exit(0);
}

int main(int argc, char * argv[])
{
	GDefineMetaClass<void> define = GDefineMetaClass<void>::declare("gl");

	registerOpenGL(define.getMetaClass());
	registerOpenGLU(define.getMetaClass());
	registerOpenGLUT(define.getMetaClass());
	registerOpenGLUT2(define.getMetaClass());
	registerOpenGLUT3(define.getMetaClass());

	GDefineMetaGlobal()
		._method("exitDemo", &exitDemo);

	bool runLua = true;

	if(argc > 1) {
		if(strcmp(argv[1], "v8") == 0) {
			runLua = false;
		}
	}

	if(runLua) {
		cout << "Running Lua script." << endl;
	}
	else {
		cout << "Running V8 Javascript." << endl;
	}
	cout << "If 'v8' is put as the parameter, V8 Javascript will be run." << endl
		<< endl
		<< "Press ESC in the window to exit." << endl
		<< "Don't click X button because GLUT doesn't exit main loop well." << endl
	;

	TestScriptContext * context;
	context = createTestScriptContext((runLua ? tslLua : tslV8), tsaLib);
	if(context->getBindingLib()) {
		start(context->getBindingLib(), context, define.getMetaClass());
	}
	else {
		start(context->getBindingApi(), context, define.getMetaClass());
	}

	return 0;
}

