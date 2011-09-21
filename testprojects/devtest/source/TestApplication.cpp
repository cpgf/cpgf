#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

#include "TestApplication.h"
#include "cpgf/game/grenderengine.h"
#include "cpgf/game/gentity.h"
#include "cpgf/game/g2d/gcomponents2d.h"
#include "cpgf/game/ginput.h"
#include "cpgf/game/gsubsystem.h"


void testMatrix()
{
	static bool done = false;

	if(done) return;
	done = true;

	const float x = 100, y = 200;
	const float angle = gPI_4;
	float m[16];
	GMatrix4 mat;

	mat.translate(x, y);
	mat.rotateZ(angle);
	mat.scale(1.2f, 1.5f);
	mat.translate(-x, -y);
//	mat.rotateZ(-angle);

	mat.getDataColumnMajor(m);
	debugTrace("\nMT================");
	debugPrintMatrix(m);
	m[0] = -1000;

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glTranslatef(x, y, 0);
	glRotatef(angle * 180 / gPI, 0, 0, 1);
	glScalef(1.2f, 1.5f, 1.0f);
	glTranslatef(-x, -y, 0);
//	glRotatef(-angle * 180 / gPI, 0, 0, 1);

	glGetFloatv(GL_MODELVIEW_MATRIX, m);
	glPopMatrix();

	debugTrace("\nGL================");
	debugPrintMatrix(m);
}

const gfloat linePoints[] = {
	5, 5,
	300, 300,
	500, 200
};

class MyEntity : public GEntity
{
private:
	typedef GEntity super;

public:
	MyEntity(GSubSystemManager * subSystemManager) : super(subSystemManager) {
		static int X = 1;

		this->N = X;
		++X;
	}

public:
	int N;
};

void onTouch(const GEvent & e)
{
	MyEntity * entity = static_cast<MyEntity * >(e.getSource());

	GRenderComponent * render = static_cast<GRenderComponent *>(entity->getComponent(getComponentRegister()->getComponentTypeFromName(componentName_Render)));
	render->color = 0xff000000 | ~(render->color & 0x00ffffff);
}

MyEntity * createTestEntity(GScene * scene)
{
	MyEntity * entity = new MyEntity(scene->getSubSystemManager());

	scene->getEntityPool()->addEntity(entity);

	return entity;
}

void initEntity(GScene * scene)
{
	GOrientationComponent * ori;
	GRenderComponent * render;
	GMotionComponent * motion;
	GGlobalTransformComponent * global;

	MyEntity * entity;
	MyEntity * fakeRoot;
	MyEntity * root2;

//	GSubSystemManager * subSystemManager = scene->getSubSystemManager();

//	GComponentRegister * reg = getComponentRegister();
	entity = createTestEntity(scene);
	fakeRoot = entity;

	entity->beginInstallComponent();
	render = new GRenderComponent();
	render->color = 0xff0000ff;
	entity->installComponent(render);
	entity->installComponent(new GLocalTransformComponent());
	ori = new GOrientationComponent();
	ori->setPosition(100, 60);
	ori->setRotate(gPI_4 / 2);
	ori->setZOrder(10);
	entity->installComponent(ori);
	global = new GGlobalTransformComponent();
	entity->installComponent(global);
	entity->endInstallComponent();

	entity = createTestEntity(scene);
	root2 = entity;

	entity->beginInstallComponent();
	render = new GRenderComponent();
	render->color = 0xffff0000;
	entity->installComponent(render);
	entity->installComponent(new GLocalTransformComponent());
	ori = new GOrientationComponent();
	ori->setPosition(100, 60);
	ori->setRotate(gPI_4 / 2);
	ori->setZOrder(100);
	entity->installComponent(ori);
	global = new GGlobalTransformComponent();
	global->setParentComponent(fakeRoot->getComponent(global->getType()));
	entity->installComponent(global);
	motion = new GMotionComponent();
	motion->getVelocityRotate()->setVelocity(0.01f);
	motion->getVelocityRotate()->setAccelerate(0.001f);
	motion->getVelocityRotate()->setLimitVelocity(0.1f);
	entity->installComponent(motion);
	entity->installComponent(new GTouchInputComponent());
	entity->endInstallComponent();
	entity->addListener(eventTouchDown, GCallback<void (const GEvent &)>(&onTouch));


	entity = createTestEntity(scene);

	entity->beginInstallComponent();
	render = new GRenderComponent();
	render->color = 0xff00ff00;
	entity->installComponent(render);
	entity->installComponent(new GLocalTransformComponent());
	ori = new GOrientationComponent();
	ori->setPosition(300, 60);
	ori->setZOrder(2000);
	entity->installComponent(ori);
	global = new GGlobalTransformComponent();
	entity->installComponent(global);
	entity->installComponent(new GTouchInputComponent());
	entity->endInstallComponent();
	entity->addListener(eventTouchDown, GCallback<void (const GEvent &)>(&onTouch));


	entity = createTestEntity(scene);

	entity->beginInstallComponent();
	render = new GRenderComponent();
	render->color = 0xffffff00;
	entity->installComponent(render);
	entity->installComponent(new GLocalTransformComponent());
	ori = new GOrientationComponent();
	ori->setPosition(100, 60);
	ori->setZOrder(200);
	entity->installComponent(ori);
	global = new GGlobalTransformComponent();
	global->setParentComponent(root2->getComponent(global->getType()));
	entity->installComponent(global);
	entity->endInstallComponent();


}

GSubSystemManager * testCreateDefaultSubSystemManager()
{
	GSubSystemManager * manager = new GSubSystemManager;

	getSubSystemRegister()->fillSubSystemManager(manager);

	return manager;
}

void TestApplication::onEngineStart()
{
	this->sceneManager.reset(new GSceneManager(this->getUpdateRunner()));

	GScene * scene = new GScene(testCreateDefaultSubSystemManager());
	initEntity(scene);

	this->sceneManager->gotoScene(scene);
}

void TestApplication::onEngineStop()
{
	this->sceneManager.reset(NULL);
}

void TestApplication::onEngineRender(GUpdateContext * context)
{
	super::onEngineRender(context);
}

void TestApplication::onEngineUpdate(GUpdateContext * context)
{
	super::onEngineUpdate(context);
}

