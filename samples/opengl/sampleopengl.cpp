#include "cpgf/gmetadefine.h"
#include "cpgf/scriptbind/gscriptbindutil.h"
#include "cpgf/metadata/util/gmetadata_bytearray.h"

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

	const char * fileName = "opengl.js";
	
	if(argc > 1) {
		fileName = argv[1];
	}

	ScriptLanguage lang = getScriptLanguageFromFileName(fileName);
	
	cout << "Running " << getLanguageText(lang) << " script." << endl;
	cout << "Press ESC in the window to exit." << endl
		<< "Don't click X button because GLUT doesn't exit main loop well." << endl
	;

	intializeScriptEngine(lang);
	
	GScopedPointer<GScriptRunner> runner;
	GScopedInterface<IMetaService> service(createDefaultMetaService());
	
	runner.reset(createScriptRunnerFromScriptLanguage(lang, service.get()));

	GScopedInterface<IScriptObject> scriptObject(runner->getScripeObject());
	
	GScopedInterface<IMetaClass> glMetaClass(static_cast<IMetaClass *>(metaItemToInterface(define.getMetaClass())));
	scriptObject->bindClass("gl", glMetaClass.get());
	GScopedInterface<IMetaMethod> method(static_cast<IMetaMethod *>(metaItemToInterface(getGlobalMetaClass()->getMethod("exitDemo"))));
	scriptObject->bindMethod("exitDemo", NULL, method.get());
	
	GDefineMetaClass<IByteArray> byteArrayDefine = GDefineMetaClass<IByteArray>::declare("IByteArray");
	buildMetaData_byteArray(byteArrayDefine);
	GScopedInterface<IMetaClass> byteArrayMetaClass(static_cast<IMetaClass *>(metaItemToInterface(byteArrayDefine.getMetaClass())));
	scriptObject->bindClass("IByteArray", byteArrayMetaClass.get());
	
	if(runner->executeFile(fileName)) {
		invokeScriptFunction(scriptObject.get(), "start");
	}
	else {
		cout << "Failed to execute " << fileName << ", maybe it doesn't exist?" << endl;
	}
	
	finalizeScriptEngine(lang);

	return 0;
}

