#include "cpgf/scriptbind/gv8runner.h"
#include "cpgf/scriptbind/gluarunner.h"
#include "cpgf/scriptbind/gpythonrunner.h"
#include "cpgf/scriptbind/gscriptbindapi.h"
#include "cpgf/gmetaapi.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/gscopedptr.h"
#include "cpgf/gapiutil.h"

#include "Python.h"

#include <cmath>
#include <cstring>
#include <iostream>

using namespace std;
using namespace cpgf;

void scriptTrace(const char * s)
{
	cout << "script message: " << s << endl;
}

void run(GScriptRunner * runner, const char * fileName)
{
	GScopedInterface<IMetaService> service(runner->getService());
	GScopedInterface<IScriptObject> scriptObject(runner->getScripeObject());
	GScopedInterface<IMetaClass> metaClass(service->findClassByName("sfml"));
	scriptObject->bindClass("sfml", metaClass.get());
	
	metaClass.reset(service->findClassByName("test"));
	scriptObject->bindClass("test", metaClass.get());

	if(! runner->executeFile(fileName)) {
		cout << "Failed to execute " << fileName << ", maybe it doesn't exist?" << endl;
	}
}

enum ScriptLanguage {
	slJavascript, slLua, slPython
};

int main(int argc, char * argv[])
{
	const char * fileName = "sfml.js";
	
	if(argc > 1) {
		fileName = argv[1];
	}
	
	const char * langText = "Javascript";
	ScriptLanguage lang = slJavascript;
	
	if(strstr(fileName, ".lua") != NULL) {
		lang = slLua;
		langText = "Lua";
	}
	if(strstr(fileName, ".py") != NULL) {
		lang = slPython;
		langText = "Python";
	}
	
	cout << "Executing file " << fileName << " as " << langText << endl;

	GDefineMetaGlobal()
		._class(
			GDefineMetaNamespace::declare("test")
				._method("scriptTrace", &scriptTrace)
		)
	;

	GScopedPointer<GScriptRunner> runner;
	GScopedInterface<IMetaService> service(createDefaultMetaService());

	if(lang == slPython) {
		Py_Initialize();
	}

	switch(lang) {
		case slJavascript:
			runner.reset(createV8ScriptRunner(service.get()));
			break;
			
		case slLua:
			runner.reset(createLuaScriptRunner(service.get()));
			break;
			
		case slPython:
			runner.reset(createPythonScriptRunner(service.get()));
			break;
	}
	
	run(runner.get(), fileName);
	
	if(lang == slPython) {
		Py_Finalize();
	}
	
	return 0;
}
