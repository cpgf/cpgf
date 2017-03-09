#include "samplescriptbindutil.h"
#include "cpgf/scriptbind/gscriptbindutil.h"
#include "cpgf/gscopedinterface.h"

#if ENABLE_LUA
	#include "cpgf/scriptbind/gluarunner.h"
#endif

#if ENABLE_V8
	#include "cpgf/scriptbind/gv8runner.h"
#endif

#if ENABLE_PYTHON
	#include "cpgf/scriptbind/gpythonrunner.h"
	#include "Python.h"
#endif

#if ENABLE_SPIDERMONKEY
	#include "cpgf/scriptbind/gspidermonkeyrunner.h"
#endif

#include <string.h>
#include <iostream>

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4996)
#endif


using namespace cpgf;
using namespace std;

enum ScriptLanguage {
	slV8Javascript, slSpiderMonkeyJavaScript, slLua, slPython
};

ScriptLanguage getScriptLanguageFromFileName(const char * fileName, bool useV8)
{
	const char * ext = strrchr(fileName, '.');

	if(ext != NULL) {
		if(strcmp(ext, ".lua") == 0) {
			return slLua;
		}
		if(strcmp(ext, ".py") == 0) {
			return slPython;
		}
	}

	return useV8 ? slV8Javascript : slSpiderMonkeyJavaScript;
}

GScriptRunner * createScriptRunnerFromScriptLanguage(ScriptLanguage lang, IMetaService * service)
{
	switch(lang) {
		case slV8Javascript:
#if ENABLE_V8
			return createV8ScriptRunner(service);
#endif
			break;
		
		case slSpiderMonkeyJavaScript:
#if ENABLE_SPIDERMONKEY
			return createSpiderMonkeyScriptRunner(service);
#endif
			break;
		
		case slLua:
#if ENABLE_LUA
			return createLuaScriptRunner(service);
#endif
			break;

		case slPython:
#if ENABLE_PYTHON
			return createPythonScriptRunner(service);
#endif
			break;
	}

	return NULL;
}

const char * getLanguageText(ScriptLanguage lang)
{
	switch(lang) {
		case slV8Javascript:
			return "V8 Javascript";
		
		case slSpiderMonkeyJavaScript:
			return "SpiderMonkey Javascript";
		
		case slLua:
			return "Lua";

		case slPython:
			return "Python";
	}

	return "Unknown";
}

void intializeScriptEngine(ScriptLanguage lang)
{
	switch(lang) {
		case slV8Javascript:
			break;
		
		case slSpiderMonkeyJavaScript:
			break;

		case slLua:
			break;

		case slPython:
#if ENABLE_PYTHON
			Py_Initialize();
#endif
			break;
	}
}

void finalizeScriptEngine(ScriptLanguage lang)
{
	switch(lang) {
		case slV8Javascript:
			break;
		
		case slSpiderMonkeyJavaScript:
			break;

		case slLua:
			break;

		case slPython:
#if ENABLE_PYTHON
			Py_Finalize();
#endif
			break;
	}
}

class ScriptHelperImplement
{
public:
	ScriptHelperImplement(int argc, char * argv[]);
	~ScriptHelperImplement();
	
	bool execute();
	
	IMetaService * borrowService() const {
		return this->service.get();
	}
	
	IScriptObject * borrowScriptObject() const {
		return this->scriptObject.get();
	}
	
private:
	void parseCommandLine(int argc, char * argv[]);
	
private:
	const char * fileName;
	ScriptLanguage scriptLanguage;
	GScopedPointer<GScriptRunner> runner;
	GScopedInterface<IMetaService> service;
	GScopedInterface<IScriptObject> scriptObject;
};

void die(const char * message)
{
	if(message != NULL)  {
		cerr << message << endl;
	}
	exit(1);
}

ScriptHelperImplement::ScriptHelperImplement(int argc, char * argv[])
	: fileName(NULL), scriptLanguage(slSpiderMonkeyJavaScript)
{
	this->parseCommandLine(argc, argv);

	cout << "Running " << getLanguageText(this->scriptLanguage) << " script." << endl;
	cout << "File: " << this->fileName << endl;
	cout << endl;

	intializeScriptEngine(this->scriptLanguage);

	this->service.reset(createDefaultMetaService());
	this->runner.reset(createScriptRunnerFromScriptLanguage(this->scriptLanguage, this->service.get()));
	
	if(! this->runner) {
		cout << "Can't create script engine, maybe not included?" << endl;
		exit(1);
	}
	
	this->scriptObject.reset(this->runner->getScripeObject());

	this->scriptObject->bindCoreService("cpgf", NULL);
}

ScriptHelperImplement::~ScriptHelperImplement()
{
	finalizeScriptEngine(this->scriptLanguage);
}

bool ScriptHelperImplement::execute()
{
	bool success = this->runner->executeFile(this->fileName);
	if(! success) {
		cerr << "Failed to execute file " << this->fileName << endl;
	}
	return success;
}

void ScriptHelperImplement::parseCommandLine(int argc, char * argv[])
{
	bool useV8 = false;
	this->fileName = NULL;
	
	for(int i = 1; i < argc; ++i) {
		const char * arg = argv[i];
		if(arg[0] == '-') {
			if(strcmp(arg, "-v8") == 0) {
				useV8 = true;
			}
		}
		else {
			if(this->fileName != NULL) {
				die("Only one file can be specified.");
			}
			this->fileName = arg;
		}
	}
	if(this->fileName == NULL) {
		die("Please specify a file name in the command line.");
	}
	
	FILE * file = fopen(this->fileName, "rb");
	if(file == NULL) {
		cerr << "File doesn't exist " << this->fileName << endl;
		die(NULL);
	}
	fclose(file);
	
	this->scriptLanguage = getScriptLanguageFromFileName(this->fileName, useV8);
}


ScriptHelper::ScriptHelper(int argc, char * argv[])
	: implement(new ScriptHelperImplement(argc, argv))
{
}

ScriptHelper::~ScriptHelper()
{
}

bool ScriptHelper::execute()
{
	return this->implement->execute();
}

IMetaService * ScriptHelper::borrowService() const
{
	return this->implement->borrowService();
}

IScriptObject * ScriptHelper::borrowScriptObject() const
{
	return this->implement->borrowScriptObject();
}

