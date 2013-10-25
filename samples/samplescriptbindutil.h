#ifndef CPGF_SAMPLESCRIPTBINDUTIL_H
#define CPGF_SAMPLESCRIPTBINDUTIL_H

#include "cpgf/scriptbind/gscriptrunner.h"

namespace cpgf {
	struct IMetaService;
	struct IScriptObject;
} // namespace cpgf

class ScriptHelperImplement;

class ScriptHelper
{
public:
	ScriptHelper(int argc, char * argv[]);
	~ScriptHelper();

	bool execute();
	
	cpgf::IMetaService * borrowService() const;
	cpgf::IScriptObject * borrowScriptObject() const;
	
private:
	cpgf::GScopedPointer<ScriptHelperImplement> implement;
};



#endif
