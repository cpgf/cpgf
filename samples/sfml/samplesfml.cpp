#include "../samplescriptbindutil.h"

#include "cpgf/scriptbind/gscriptbindapi.h"
#include "cpgf/gmetaapi.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/gscopedptr.h"
#include "cpgf/gapiutil.h"
#include "cpgf/gscopedinterface.h"

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

int main(int argc, char * argv[])
{
	const char * fileName = "sfml.js";
	
	if(argc > 1) {
		fileName = argv[1];
	}
	
	ScriptLanguage lang = getScriptLanguageFromFileName(fileName);
	
	cout << "Executing file " << fileName << " as " << getLanguageText(lang) << endl;

	GDefineMetaGlobal()
		._class(
			GDefineMetaNamespace::declare("test")
				._method("scriptTrace", &scriptTrace)
		)
	;

	GScopedInterface<IMetaService> service(createDefaultMetaService());

	intializeScriptEngine(lang);

	GScopedPointer<GScriptRunner> runner(createScriptRunnerFromScriptLanguage(lang, service.get()));
	
	run(runner.get(), fileName);
	
	finalizeScriptEngine(lang);
	
	return 0;
}
