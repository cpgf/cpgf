#ifndef CPGF_GPYTHONRUNNER_H
#define CPGF_GPYTHONRUNNER_H

namespace cpgf {

class GScriptRunner;
struct IMetaService;

GScriptRunner * createPythonScriptRunner(IMetaService * service);



} // namespace cpgf



#endif
