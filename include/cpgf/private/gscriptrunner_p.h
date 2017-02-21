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

private:
	GScopedInterface<IMetaService> service;
	GScopedInterface<IScriptObject> scriptObject;
};


} // namespace cpgf



#endif
