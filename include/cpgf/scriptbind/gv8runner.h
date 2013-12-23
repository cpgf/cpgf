#ifndef CPGF_GV8RUNNER_H
#define CPGF_GV8RUNNER_H

#include "cpgf/scriptbind/gscriptrunner.h"
#include <v8.h>

namespace cpgf {

v8::Isolate *getV8Isolate();
void setV8Isolate(v8::Isolate *isolate);

class GScriptRunner;
struct IMetaService;

GScriptRunner * createV8ScriptRunner(IMetaService * service);

GScriptRunner * createV8ScriptRunner(IMetaService * service, v8::Handle<v8::Context> ctx);


} // namespace cpgf



#endif
