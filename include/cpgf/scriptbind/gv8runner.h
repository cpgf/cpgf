#ifndef __GV8RUNNER_H
#define __GV8RUNNER_H

#include "cpgf/scriptbind/gscriptrunner.h"


namespace cpgf {

class GScriptRunner;
struct IMetaService;

GScriptRunner * createV8ScriptRunner(IMetaService * service);



} // namespace cpgf



#endif
