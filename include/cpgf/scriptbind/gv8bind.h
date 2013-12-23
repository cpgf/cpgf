#ifndef CPGF_GV8BIND_H
#define CPGF_GV8BIND_H


#include "cpgf/scriptbind/gscriptbind.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4100 4127)
#endif

#include "v8.h"

#if defined(_MSC_VER)
#pragma warning(pop)
#endif


namespace cpgf {

GScriptObject * createV8ScriptObject(IMetaService * service, v8::Local<v8::Object> object, const GScriptConfig & config);
IScriptObject * createV8ScriptInterface(IMetaService * service, v8::Local<v8::Object> object, const GScriptConfig & config);

void clearV8DataPool();

extern v8::Isolate *cpgf_isolate;

} // namespace cpgf




#endif


