#include "samplescriptbindutil.h"
#if ENABLE_LUA
#include "cpgf/scriptbind/gluarunner.h"
#endif
#if ENABLE_V8
#include "cpgf/scriptbind/gv8runner.h"
#endif
#if ENABLE_PYTHON
#endif

#include "string.h"


using namespace cpgf;

ScriptLanguage getScriptLanguageFromFileName(const char * fileName)
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

	return slJavascript;
}

GScriptRunner * createScriptRunnerFromScriptLanguage(ScriptLanguage lang, IMetaService * service)
{
	switch(lang) {
		case slJavascript:
#if ENABLE_V8
			return createV8ScriptRunner(service);
#endif
			break;
		
		case slLua:
#if ENABLE_LUA
			return createLuaScriptRunner(service);
#endif
			break;

		case slPython:
			return NULL;
	}

	return NULL;
}

const char * getLanguageText(ScriptLanguage lang)
{
	switch(lang) {
		case slJavascript:
			return "Javascript";
		
		case slLua:
			return "Lua";

		case slPython:
			return "Python";
	}

	return "Unknown";
}
