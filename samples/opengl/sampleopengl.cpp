#include "cpgf/gmetadefine.h"
#include "cpgf/scriptbind/gscriptbindutil.h"
#include "cpgf/gscopedinterface.h"

#include "../samplescriptbindutil.h"

#include <iostream>
#include <string.h>

using namespace cpgf;
using namespace std;

void registerOpenGL(GMetaClass * metaClass);
void registerOpenGLU(GMetaClass * metaClass);
void registerOpenGLUT(GMetaClass * metaClass);
void registerOpenGLUT2(GMetaClass * metaClass);
void registerOpenGLUT3(GMetaClass * metaClass);

void exitDemo()
{
	exit(0);
}

int main(int argc, char * argv[])
{
	GDefineMetaNamespace define = GDefineMetaNamespace::declare("gl");

	registerOpenGL(define.getMetaClass());
	registerOpenGLU(define.getMetaClass());
	registerOpenGLUT(define.getMetaClass());
	registerOpenGLUT2(define.getMetaClass());
	registerOpenGLUT3(define.getMetaClass());

	GDefineMetaGlobal()
		._method("exitDemo", &exitDemo);

	ScriptHelper scriptHelper(argc, argv);

	cout << "Press ESC in the window to exit." << endl
		<< "Don't click X button because GLUT doesn't exit main loop well." << endl
	;

	GScopedInterface<IMetaClass> glMetaClass(static_cast<IMetaClass *>(metaItemToInterface(define.getMetaClass())));
	scriptHelper.borrowScriptObject()->bindClass("gl", glMetaClass.get());
	GScopedInterface<IMetaMethod> method(static_cast<IMetaMethod *>(metaItemToInterface(getGlobalMetaClass()->getMethod("exitDemo"))));
	scriptHelper.borrowScriptObject()->bindMethod("exitDemo", NULL, method.get());
	
	if(scriptHelper.execute()) {
		invokeScriptFunction(scriptHelper.borrowScriptObject(), "start");
	}

	return 0;
}

