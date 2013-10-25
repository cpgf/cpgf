#ifndef CPGF_GLUARUNNER_H
#define CPGF_GLUARUNNER_H

#include "cpgf/scriptbind/gscriptrunner.h"


namespace cpgf {

class GScriptRunner;
struct IMetaService;

GScriptRunner * createLuaScriptRunner(IMetaService * service);



} // namespace cpgf



#endif
