#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

#include <glut.h>

#include "cpgf/gmetadefine.h"

#include "bind_common.h"


using namespace cpgf;

void registerOpenGL(GMetaClass * metaClass);
void registerOpenGLU(GMetaClass * metaClass);
void registerOpenGLUT(GMetaClass * metaClass);

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
	"function initData() \n"
		"vertexList = gl.createByteArray() \n"
		"indexList = gl.createByteArray() \n"

		"vertexList.writeFloat32(0.5) \n" // 0
		"vertexList.writeFloat32(0.5) \n"
		"vertexList.writeFloat32(0.5) \n"

		"vertexList.writeFloat32(-0.5) \n" // 1
		"vertexList.writeFloat32(0.5) \n"
		"vertexList.writeFloat32(0.5) \n"

		"vertexList.writeFloat32(-0.5) \n" // 2
		"vertexList.writeFloat32(-0.5) \n"
		"vertexList.writeFloat32(0.5) \n"

		"vertexList.writeFloat32(0.5) \n" // 3
		"vertexList.writeFloat32(-0.5) \n"
		"vertexList.writeFloat32(0.5) \n"

		"vertexList.writeFloat32(0.5) \n" // 4
		"vertexList.writeFloat32(-0.5) \n"
		"vertexList.writeFloat32(-0.5) \n"

		"vertexList.writeFloat32(0.5) \n" // 5
		"vertexList.writeFloat32(0.5) \n"
		"vertexList.writeFloat32(-0.5) \n"

		"vertexList.writeFloat32(-0.5) \n" // 6
		"vertexList.writeFloat32(0.5) \n"
		"vertexList.writeFloat32(-0.5) \n"

		"vertexList.writeFloat32(-0.5) \n" // 7
		"vertexList.writeFloat32(-0.5) \n"
		"vertexList.writeFloat32(-0.5) \n"

		"indexList.writeInt32(0) \n"
		"indexList.writeInt32(1) \n"
		"indexList.writeInt32(2) \n"
		"indexList.writeInt32(3) \n"

		"indexList.writeInt32(0) \n"
		"indexList.writeInt32(3) \n"
		"indexList.writeInt32(4) \n"
		"indexList.writeInt32(5) \n"

		"indexList.writeInt32(4) \n"
		"indexList.writeInt32(5) \n"
		"indexList.writeInt32(6) \n"
		"indexList.writeInt32(7) \n"

		"indexList.writeInt32(1) \n"
		"indexList.writeInt32(2) \n"
		"indexList.writeInt32(7) \n"
		"indexList.writeInt32(6) \n"

		"indexList.writeInt32(0) \n"
		"indexList.writeInt32(1) \n"
		"indexList.writeInt32(6) \n"
		"indexList.writeInt32(5) \n"

		"indexList.writeInt32(2) \n"
		"indexList.writeInt32(3) \n"
		"indexList.writeInt32(4) \n"
		"indexList.writeInt32(7) \n"
	"end \n"
	"function render() \n"
		"gl.glClear(gl.GL_COLOR_BUFFER_BIT or gl.GL_DEPTH_BUFFER_BIT or gl.GL_STENCIL_BUFFER_BIT) \n"

		"gl.glEnableClientState(gl.GL_VERTEX_ARRAY) \n"
	    "gl.glPushMatrix() \n"
		"gl.glRotatef(rotate, 1, 0, 1) \n"
		"gl.glTranslatef(translateX, translateY, 0) \n"
		"gl.glFrontFace(gl.GL_CCW) \n"
		"gl.glCullFace(gl.GL_BACK) \n"
		"gl.glEnable(gl.GL_CULL_BACK) \n"
		"gl.glPolygonMode(gl.GL_FRONT_AND_BACK, gl.GL_LINE) \n"
		"gl.glVertexPointer(3, gl.GL_FLOAT, 0, vertexList) \n"
		"gl.glDrawElements(gl.GL_QUADS, 24, gl.GL_UNSIGNED_INT, indexList) \n"
	    "gl.glPopMatrix() \n"
		"gl.glFlush() \n"
	"end \n"
	"function reshape(w, h) \n"
		"gl.glViewport(0, 0, w, h) \n"
		"gl.glMatrixMode(gl.GL_PROJECTION) \n"
	"end \n"
	"function specialKey(key, x, y) \n"
		"local delta = 0.01 \n"
		"if(key == gl.GLUT_KEY_LEFT) then \n"
		"translateX = translateX - delta \n"
		"elseif(key == gl.GLUT_KEY_RIGHT) then \n"
		"translateX = translateX + delta \n"
		"elseif(key == gl.GLUT_KEY_UP) then \n"
		"translateY = translateY - delta \n"
		"elseif(key == gl.GLUT_KEY_DOWN) then \n"
		"translateY = translateY + delta \n"
		"end \n"
	"end \n"
	"function timer(value) \n"
		"rotate = rotate + 1 \n"
		"rotate = rotate % 360 \n"
		"gl.glutPostRedisplay() \n"
		"gl.glutTimerFunc(value, timer, value) \n"
	"end \n"
	"function start() \n"
		"initData() \n"
		"gl.glutInit() \n"
		"gl.glutInitDisplayMode(gl.GLUT_DEPTH or gl.GLUT_SINGLE or gl.GLUT_RGB or gl.GLUT_STENCIL) \n"
		"gl.glutInitWindowPosition(100,100) \n"
		"gl.glutInitWindowSize(500,500) \n"
		"gl.glutCreateWindow(\"cpgf OpenGL binding demo\") \n"
		"gl.glutDisplayFunc(render) \n"
		"gl.glutReshapeFunc(reshape) \n"
		"gl.glutSpecialFunc(specialKey) \n"
		"period = 33 \n"
		"gl.glutTimerFunc(period, timer, period) \n"
		"gl.glutMainLoop() \n"
	"end \n"
;


template <typename Binding>
void start(Binding * binding, TestScriptContext * c, GMetaClass * glMetaClass)
{
	GScopedPointer<TestScriptContext> context(c);
	
	GScopedInterface<IMetaClass> metaClass(static_cast<IMetaClass *>(metaItemToInterface(glMetaClass)));
	binding->bindClass("gl", metaClass.get());

	if(context->isLua()) {
		context->doString(luaCode);
		invokeScriptFunction(binding, "start", NULL, 0);
	}

	context.reset();
};

void testOpenGL()
{
	GDefineMetaClass<void> define = GDefineMetaClass<void>::declare("gl");

	registerOpenGL(define.getMetaClass());
	registerOpenGLU(define.getMetaClass());
	registerOpenGLUT(define.getMetaClass());

	TestScriptContext * context = createTestScriptContext(tslLua, tsaLib);
	if(context->getBindingLib()) {
		start(context->getBindingLib(), context, define.getMetaClass());
	}
	else {
		start(context->getBindingApi(), context, define.getMetaClass());
	}
}

