#include "cpgf/gmetadefine.h"
#include "../samplescriptbindutil.h"
#include "cpgf/scriptbind/gscriptbindutil.h"
#include "cpgf/gscopedinterface.h"

#include <iostream>

using namespace cpgf;
using namespace std;

int main(int argc, const char * argv[])
{
	const char * fileName = "irrlicht.js";

	if(argc > 1) {
		fileName = argv[1];
	}

	ScriptLanguage lang = getScriptLanguageFromFileName(fileName);
	
	cout << "Running " << getLanguageText(lang) << " script." << endl;

	intializeScriptEngine(lang);

	GScopedPointer<GScriptRunner> runner;
	GScopedInterface<IMetaService> service(createDefaultMetaService());
	
	runner.reset(createScriptRunnerFromScriptLanguage(lang, service.get()));

	GScopedInterface<IScriptObject> scriptObject(runner->getScripeObject());

	scriptObject->bindCoreService("cpgf", NULL);
	
	GScopedInterface<IMetaClass> metaClass(service->findClassByName("irrlicht"));
	
	scriptObject->bindClass("irr", metaClass.get());
	
	if(! runner->executeFile(fileName)) {
		cout << "Failed to execute " << fileName << ", maybe it doesn't exist?" << endl;
	}

	return 0;
}
