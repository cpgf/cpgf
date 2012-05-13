#ifndef __GV8RUNNER_H
#define __GV8RUNNER_H

namespace cpgf {

class GScriptRunner;
struct IMetaService;

GScriptRunner * createV8ScriptRunner(IMetaService * service);



} // namespace cpgf



#endif
