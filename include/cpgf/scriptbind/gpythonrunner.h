#ifndef __GPYTHONRUNNER_H
#define __GPYTHONRUNNER_H

namespace cpgf {

class GScriptRunner;
struct IMetaService;

GScriptRunner * createPythonScriptRunner(IMetaService * service);



} // namespace cpgf



#endif
