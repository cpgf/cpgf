#include "cpgf/gmetadefine.h"

#include "../../test/scriptbind/bind_common.h"

#include "Box2D/Box2D.h"

#if defined(_WIN32)
	#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <iostream>
#include <string.h>

using namespace cpgf;
using namespace std;

void registerOpenGL(GMetaClass * metaClass);
void registerOpenGLU(GMetaClass * metaClass);
void registerOpenGLUT(GMetaClass * metaClass);
void registerOpenGLUT2(GMetaClass * metaClass);
void registerOpenGLUT3(GMetaClass * metaClass);


class DebugDraw : public b2DebugDraw
{
public:
	void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);

	void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);

	void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color);

	void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color);

	void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color);

	void DrawTransform(const b2Transform& xf);

    void DrawPoint(const b2Vec2& p, float32 size, const b2Color& color);

    void DrawString(int x, int y, const char* string, ...); 

    void DrawAABB(b2AABB* aabb, const b2Color& color);
};

void DebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	glColor3f(color.r, color.g, color.b);
	glBegin(GL_LINE_LOOP);
	for (int32 i = 0; i < vertexCount; ++i)
	{
		glVertex2f(vertices[i].x, vertices[i].y);
	}
	glEnd();
}

void DebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	glEnable(GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(0.5f * color.r, 0.5f * color.g, 0.5f * color.b, 0.5f);
	glBegin(GL_TRIANGLE_FAN);
	for (int32 i = 0; i < vertexCount; ++i)
	{
		glVertex2f(vertices[i].x, vertices[i].y);
	}
	glEnd();
	glDisable(GL_BLEND);

	glColor4f(color.r, color.g, color.b, 1.0f);
	glBegin(GL_LINE_LOOP);
	for (int32 i = 0; i < vertexCount; ++i)
	{
		glVertex2f(vertices[i].x, vertices[i].y);
	}
	glEnd();
}

void DebugDraw::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
{
	const float32 k_segments = 16.0f;
	const float32 k_increment = 2.0f * b2_pi / k_segments;
	float32 theta = 0.0f;
	glColor3f(color.r, color.g, color.b);
	glBegin(GL_LINE_LOOP);
	for (int32 i = 0; i < k_segments; ++i)
	{
		b2Vec2 v = center + radius * b2Vec2(cosf(theta), sinf(theta));
		glVertex2f(v.x, v.y);
		theta += k_increment;
	}
	glEnd();
}

void DebugDraw::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
{
	const float32 k_segments = 16.0f;
	const float32 k_increment = 2.0f * b2_pi / k_segments;
	float32 theta = 0.0f;
	glEnable(GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(0.5f * color.r, 0.5f * color.g, 0.5f * color.b, 0.5f);
	glBegin(GL_TRIANGLE_FAN);
	for (int32 i = 0; i < k_segments; ++i)
	{
		b2Vec2 v = center + radius * b2Vec2(cosf(theta), sinf(theta));
		glVertex2f(v.x, v.y);
		theta += k_increment;
	}
	glEnd();
	glDisable(GL_BLEND);

	theta = 0.0f;
	glColor4f(color.r, color.g, color.b, 1.0f);
	glBegin(GL_LINE_LOOP);
	for (int32 i = 0; i < k_segments; ++i)
	{
		b2Vec2 v = center + radius * b2Vec2(cosf(theta), sinf(theta));
		glVertex2f(v.x, v.y);
		theta += k_increment;
	}
	glEnd();

	b2Vec2 p = center + radius * axis;
	glBegin(GL_LINES);
	glVertex2f(center.x, center.y);
	glVertex2f(p.x, p.y);
	glEnd();
}

void DebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
	glColor3f(color.r, color.g, color.b);
	glBegin(GL_LINES);
	glVertex2f(p1.x, p1.y);
	glVertex2f(p2.x, p2.y);
	glEnd();
}

void DebugDraw::DrawTransform(const b2Transform& xf)
{
	b2Vec2 p1 = xf.position, p2;
	const float32 k_axisScale = 0.4f;
	glBegin(GL_LINES);
	
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex2f(p1.x, p1.y);
	p2 = p1 + k_axisScale * xf.R.col1;
	glVertex2f(p2.x, p2.y);

	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex2f(p1.x, p1.y);
	p2 = p1 + k_axisScale * xf.R.col2;
	glVertex2f(p2.x, p2.y);

	glEnd();
}

void DebugDraw::DrawPoint(const b2Vec2& p, float32 size, const b2Color& color)
{
	glPointSize(size);
	glBegin(GL_POINTS);
	glColor3f(color.r, color.g, color.b);
	glVertex2f(p.x, p.y);
	glEnd();
	glPointSize(1.0f);
}

void DebugDraw::DrawString(int x, int y, const char *string, ...)
{
	(void)x, (void)y, (void)string;

/*	char buffer[128];

	va_list arg;
	va_start(arg, string);
	vsprintf(buffer, string, arg);
	va_end(arg);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	int w = glutGet(GLUT_WINDOW_WIDTH);
	int h = glutGet(GLUT_WINDOW_HEIGHT);
	gluOrtho2D(0, w, h, 0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glColor3f(0.9f, 0.6f, 0.6f);
	glRasterPos2i(x, y);
	int32 length = (int32)strlen(buffer);
	for (int32 i = 0; i < length; ++i)
	{
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, buffer[i]);
	}

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
*/
}

void DebugDraw::DrawAABB(b2AABB* aabb, const b2Color& c)
{
	glColor3f(c.r, c.g, c.b);
	glBegin(GL_LINE_LOOP);
	glVertex2f(aabb->lowerBound.x, aabb->lowerBound.y);
	glVertex2f(aabb->upperBound.x, aabb->lowerBound.y);
	glVertex2f(aabb->upperBound.x, aabb->upperBound.y);
	glVertex2f(aabb->lowerBound.x, aabb->upperBound.y);
	glEnd();
}


const char * luaCode = ""
	"world = 0 \n"
	"ground = 0 \n"
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
	"function renderBox2d() \n"
		"world.Step(0.016, 8, 3) \n"
		"world.DrawDebugData() \n"
	"end \n"
	"function render() \n"
		"gl.glClear(gl.GL_COLOR_BUFFER_BIT + gl.GL_DEPTH_BUFFER_BIT + gl.GL_STENCIL_BUFFER_BIT) \n"

		"gl.glEnableClientState(gl.GL_VERTEX_ARRAY) \n"
		"gl.glEnableClientState(gl.GL_COLOR_ARRAY) \n"
		"gl.glEnableClientState(gl.GL_NORMAL_ARRAY) \n"
	    "gl.glPushMatrix() \n"
		"renderBox2d() \n"
	    "gl.glPopMatrix() \n"
		"gl.glDisableClientState(gl.GL_VERTEX_ARRAY) \n"
		"gl.glDisableClientState(gl.GL_COLOR_ARRAY) \n"
		"gl.glDisableClientState(gl.GL_NORMAL_ARRAY) \n"
		"gl.glutSwapBuffers() \n"
	"end \n"
	"function reshape(w, h) \n"
		"gl.glViewport(0, 0, w, h) \n"
		"gl.glMatrixMode(gl.GL_PROJECTION) \n"
		"gl.glLoadIdentity() \n"
		"gl.gluOrtho2D(-25, 25, -5, 45) \n"
	"end \n"
	"function specialKey(key, x, y) \n"
	"end \n"
	"function keyboard(key, x, y) \n"
		"if(key == 27) then exitDemo() end \n"
	"end \n"
	"function timer(value) \n"
		"gl.glutPostRedisplay() \n"
		"gl.glutTimerFunc(value, timer, value) \n"
	"end \n"
	"function setupBox2d() \n" // the test is from SliderCrank.h in Box2D testbed
		"gravity = box2d.b2Vec2(0.0, -10.0) \n"
		"world = box2d.b2World(gravity, 1); \n"
		"bd = box2d.b2BodyDef() \n"
		"ground = world.CreateBody(bd) \n"
		"shape = box2d.b2PolygonShape() \n"
		"shape.SetAsEdge(box2d.b2Vec2(-40.0, 0.0), box2d.b2Vec2(40.0, 0.0)) \n"
		"ground.CreateFixture(shape, 0.0)"
		"prevBody = ground \n"

		// Define crank.
		"shape = box2d.b2PolygonShape() \n"
		"shape.SetAsBox(0.5, 2.0) \n"
		"bd = box2d.b2BodyDef() \n"
		"bd.type = box2d.b2BodyType.b2_dynamicBody \n"
		"bd.position.Set(0.0, 7.0) \n"
		"body = world.CreateBody(bd) \n"
		"body.CreateFixture(shape, 2.0) \n"
		"rjd = box2d.b2RevoluteJointDef() \n"
		"rjd.Initialize(prevBody, body, box2d.b2Vec2(0.0, 5.0)) \n"
		"rjd.motorSpeed = 1.0 * box2d.b2_pi \n"
		"rjd.maxMotorTorque = 10000.0 \n"
		"rjd.enableMotor = true \n"
		"m_joint1 = world.CreateJoint(rjd) \n"
		"prevBody = body \n"

		// Define follower.
		"shape = box2d.b2PolygonShape() \n"
		"shape.SetAsBox(0.5, 4.0) \n"
		"bd = box2d.b2BodyDef() \n"
		"bd.type = box2d.b2BodyType.b2_dynamicBody \n"
		"bd.position.Set(0.0, 13.0) \n"
		"body = world.CreateBody(bd) \n"
		"body.CreateFixture(shape, 2.0) \n"
		"rjd = box2d.b2RevoluteJointDef() \n"
		"rjd.Initialize(prevBody, body, box2d.b2Vec2(0.0, 9.0)) \n"
		"rjd.enableMotor = false \n"
		"world.CreateJoint(rjd) \n"
		"prevBody = body \n"

		// Define piston
		"shape = box2d.b2PolygonShape() \n"
		"shape.SetAsBox(1.5, 1.5) \n"
		"bd = box2d.b2BodyDef() \n"
		"bd.type = box2d.b2BodyType.b2_dynamicBody \n"
		"bd.position.Set(0.0, 17.0) \n"
		"body = world.CreateBody(bd) \n"
		"body.CreateFixture(shape, 2.0) \n"
		"rjd = box2d.b2RevoluteJointDef() \n"
		"rjd.Initialize(prevBody, body, box2d.b2Vec2(0.0, 17.0)) \n"
		"world.CreateJoint(rjd) \n"
		"pjd = box2d.b2PrismaticJointDef()\n"
		"pjd.Initialize(ground, body, box2d.b2Vec2(0.0, 17.0), box2d.b2Vec2(0.0, 1.0)) \n"
		"pjd.maxMotorForce = 1000.0 \n"
		"pjd.enableMotor = true \n"
		"m_joint2 = world.CreateJoint(pjd) \n"

		// Define payload.
		"shape = box2d.b2PolygonShape() \n"
		"shape.SetAsBox(1.5, 1.5) \n"
		"bd = box2d.b2BodyDef() \n"
		"bd.type = box2d.b2BodyType.b2_dynamicBody \n"
		"bd.position.Set(0.0, 23.0) \n"
		"body = world.CreateBody(bd) \n"
		"body.CreateFixture(shape, 2.0) \n"
	"end \n"
	"function getWorld() \n"
		"return world \n"
	"end \n"
	"function start() \n"
		"gl.glutInit() \n"
		"gl.glutInitDisplayMode(gl.GLUT_DEPTH + gl.GLUT_DOUBLE + gl.GLUT_RGB + gl.GLUT_STENCIL) \n"
		"gl.glutInitWindowPosition(100, 100) \n"
		"gl.glutInitWindowSize(320, 320) \n"
		"gl.glutCreateWindow(\"cpgf OpenGL binding demo\") \n"
		"gl.glutDisplayFunc(render) \n"
		"gl.glutReshapeFunc(reshape) \n"
		"gl.glutSpecialFunc(specialKey) \n"
	    "gl.glutKeyboardFunc(keyboard) \n"
		"period = 1 \n"
		"gl.glutTimerFunc(period, timer, period) \n"

		"gl.glShadeModel(gl.GL_SMOOTH) \n"
	    "gl.glHint(gl.GL_PERSPECTIVE_CORRECTION_HINT, gl.GL_NICEST) \n"
		"gl.glHint(gl.GL_LINE_SMOOTH_HINT, gl.GL_NICEST) \n"
		"gl.glHint(gl.GL_POLYGON_SMOOTH_HINT, gl.GL_NICEST) \n"

		"gl.glColorMaterial(gl.GL_FRONT_AND_BACK, gl.GL_AMBIENT_AND_DIFFUSE) \n"
		"gl.glEnable(gl.GL_COLOR_MATERIAL) \n"
	    "gl.glClearColor(0, 0, 0, 0) \n"
		"gl.glClearStencil(0) \n"
		"gl.glClearDepth(1.0) \n"
		"gl.glDepthFunc(gl.GL_LEQUAL) \n"

		"gl.glutMainLoop() \n"
	"end \n"
;

const char * jsCode = ""
;


template <typename Binding>
void start(Binding * binding, TestScriptContext * c, const GMetaClass * glMetaClass, const GMetaClass * box2dMetaClass)
{
	GScopedPointer<TestScriptContext> context(c);

	GScopedInterface<IMetaClass> metaClass(static_cast<IMetaClass *>(metaItemToInterface(glMetaClass)));
	binding->bindClass("gl", metaClass.get());
	
	metaClass.reset(static_cast<IMetaClass *>(metaItemToInterface(box2dMetaClass)));
	binding->bindClass("box2d", metaClass.get());

	GScopedInterface<IMetaMethod> method(static_cast<IMetaMethod *>(metaItemToInterface(getGlobalMetaClass()->getMethod("exitDemo"))));
	binding->bindMethod("exitDemo", NULL, method.get());

	if(context->isLua()) {
		context->doString(luaCode);
	}

	if(context->isV8()) {
		context->doString(jsCode);
	}

	invokeScriptFunction(binding, "setupBox2d");
	GMetaVariant mv = invokeScriptFunction(binding, "getWorld");
	b2World * world = fromVariant<b2World *>(mv.getValue());
	static DebugDraw debugDraw;
	debugDraw.SetFlags(b2DebugDraw::e_shapeBit);
	world->SetDebugDraw(&debugDraw);
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
		start(context->getBindingLib(), context, define.getMetaClass(), getGlobalMetaClass()->getClass("box2d"));
	}
	else {
		start(context->getBindingApi(), context, define.getMetaClass(), getGlobalMetaClass()->getClass("box2d"));
	}

	return 0;
}

