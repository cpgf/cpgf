#ifndef __GLUARUNNER_H
#define __GLUARUNNER_H

namespace cpgf {

class GScriptRunner;
struct IMetaService;

GScriptRunner * createLuaScriptRunner(IMetaService * service);



} // namespace cpgf



#endif
