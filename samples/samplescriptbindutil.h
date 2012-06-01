#ifndef __SAMPLESCRIPTBINDUTIL_H
#define __SAMPLESCRIPTBINDUTIL_H

#include "cpgf/scriptbind/gscriptrunner.h"

namespace cpgf {
	struct IMetaService;
} // namespace cpgf

enum ScriptLanguage {
	slJavascript, slLua, slPython
};

ScriptLanguage getScriptLanguageFromFileName(const char * fileName);
cpgf::GScriptRunner * createScriptRunnerFromScriptLanguage(ScriptLanguage lang, cpgf::IMetaService * service);
const char * getLanguageText(ScriptLanguage lang);


#endif
