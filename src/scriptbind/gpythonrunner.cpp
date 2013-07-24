#include "cpgf/scriptbind/gscriptrunner.h"
#include "cpgf/private/gscriptrunner_p.h"
#include "cpgf/scriptbind/gscriptbind.h"
#include "cpgf/scriptbind/gpythonbind.h"
#include "cpgf/gmetaapi.h"

#include <stdexcept>

#include <iostream>

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4127) // warning C4127: conditional expression is constant
#endif

namespace cpgf {


namespace {

class GPythonScriptRunnerImplement : public GScriptRunnerImplement
{
private:
	typedef GScriptRunnerImplement super;

public:
	GPythonScriptRunnerImplement(IMetaService * service);
	~GPythonScriptRunnerImplement();

	virtual void executeString(const char * code);

private:
	PyObject * object;
};


GPythonScriptRunnerImplement::GPythonScriptRunnerImplement(IMetaService * service)
	: super(service)
{
	this->object = PyImport_ImportModule("__main__");
	
	GScopedInterface<IMetaService> metaService(this->getService());
	GScopedInterface<IScriptObject> scriptObject(createPythonScriptInterface(metaService.get(), this->object, GScriptConfig()));
	this->setScripeObject(scriptObject.get());
}

GPythonScriptRunnerImplement::~GPythonScriptRunnerImplement()
{
	Py_XDECREF(this->object);
}

void GPythonScriptRunnerImplement::executeString(const char * code)
{
	PyRun_SimpleString(code);
}


} // unnamed namespace


GScriptRunner * createPythonScriptRunner(IMetaService * service)
{
	return GScriptRunnerImplement::createScriptRunner(new GPythonScriptRunnerImplement(service));
}



} // namespace cpgf
