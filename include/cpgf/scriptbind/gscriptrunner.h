#ifndef __GSCRIPTRUNNER_H
#define __GSCRIPTRUNNER_H


#include "cpgf/gscopedptr.h"

#include <string>

namespace cpgf {


struct IMetaService;
struct IScriptObject;

class GScriptRunnerImplement;

class GScriptRunner
{
protected:
	GScriptRunner(GScriptRunnerImplement * implement);

public:
	~GScriptRunner();

public:
	void executeString(const char * code);
	bool executeFile(const char * fileName);

	IScriptObject * getScripeObject();

	IMetaService * getService();

private:
	GScopedPointer<GScriptRunnerImplement> implement;

private:
	friend class GScriptRunnerImplement;
};



} // namespace cpgf



#endif
