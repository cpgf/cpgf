#ifndef __GSPIDERMONKEYRUNNER_H
#define __GSPIDERMONKEYRUNNER_H

#include "cpgf/scriptbind/gscriptrunner.h"


namespace cpgf {

class GScriptRunner;
struct IMetaService;

GScriptRunner * createSpiderMonkeyScriptRunner(IMetaService * service);



} // namespace cpgf



#endif
