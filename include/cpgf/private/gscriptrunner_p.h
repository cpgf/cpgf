#ifndef CPGF_GSCRIPTRUNNER_P_H
#define CPGF_GSCRIPTRUNNER_P_H


#include "cpgf/gapiutil.h"
#include "cpgf/gscopedinterface.h"

namespace cpgf {


struct IScriptObject;

class GScriptRunnerImplement
{
public:
	static GScriptRunner * createScriptRunner(GScriptRunnerImplement * implement);

public:
	GScriptRunnerImplement(IMetaService * service);
	virtual ~GScriptRunnerImplement();

	virtual void executeString(const char * code) = 0;

	IScriptObject * getScripeObject();
	IMetaService * getService();

protected:
	void setScripeObject(IScriptObject * scriptObject);
	// The derived class should clean up (such as shutting down the 
	// script engine) in its dtor after calling finalize() before exit the dtor,
	// because TestScriptContext needs to cleanup the script objects before
	// the underlying script engine is destroyed.
	void finalize();

private:
	GScopedInterface<IMetaService> service;
	GScopedInterface<IScriptObject> scriptObject;
};


} // namespace cpgf



#endif
