#ifndef __GV8BIND_H
#define __GV8BIND_H


#include "cpgf/scriptbind/gscriptbind.h"

#include "v8.h"


namespace cpgf {

GScriptObject * createV8ScriptObject(IMetaService * service, v8::Local<v8::Object> object, const GScriptConfig & config);
IScriptObject * createV8ScriptInterface(IMetaService * service, v8::Local<v8::Object> object, const GScriptConfig & config);


} // namespace cpgf




#endif


