#ifndef __BIND_GENERAL_COMMON_H
#define __BIND_GENERAL_COMMON_H

#include "cpgf/gmetaapi.h"

#include "../testscriptbind.h"

#include <string>

#define QSTR(...) # __VA_ARGS__

#define DO(s) GCHECK(context->doString(s));
#define ERR(s) GCHECK(context->doError(s));
#define QDO(...) DO(# __VA_ARGS__)
#define QERR(...) ERR(# __VA_ARGS__)
#define QASSERT(...) QDO(scriptAssert(__VA_ARGS__))
#define NEWOBJ(lhs, ...) DO(context->getCoder()->newObject(lhs, __VA_ARGS__))
#define QNEWOBJ(lhs, ...) DO(context->getCoder()->newObject(# lhs, # __VA_ARGS__))


namespace testscript {

enum TestScriptLang {
	tslLua, tslV8
};

enum TestScriptApi {
	tsaLib, tsaApi
};


class TestScriptCoder
{
public:
	virtual ~TestScriptCoder();

	virtual std::string getNew() = 0;
	virtual std::string newObject(const std::string & lhs, const std::string & className);
};

class TestScriptContext
{
public:
	TestScriptContext(TestScriptCoder * coder);
	virtual ~TestScriptContext();

	void setBinding(cpgf::GScriptObject * binding);
	void setBinding(cpgf::IScriptObject * binding);

	cpgf::GScriptObject * getBindingLib() const {
		return this->bindingLib.get();
	}

	cpgf::IScriptObject * getBindingApi() const {
		return this->bindingApi.get();
	}

	cpgf::IMetaService * getService() const {
		return this->service.get();
	}

	TestScriptCoder * getCoder() const {
		return this->coder.get();
	}

	bool doString(const std::string & code) const;
	bool doError(const std::string & code) const;

protected:
	virtual bool doLib(const char * code) const = 0;
	virtual bool doApi(const char * code) const = 0;

protected:
	bool canPrintError() const {
		return this->printError;
	}

private:
	cpgf::GScopedInterface<cpgf::IMetaService> service;
	cpgf::GScopedPointer<TestScriptCoder> coder;
	cpgf::GScopedPointer<cpgf::GScriptObject> bindingLib;
	cpgf::GScopedInterface<cpgf::IScriptObject> bindingApi;
	mutable bool printError;
};



TestScriptContext * createTestScriptContext(TestScriptLang lang, TestScriptApi api);


} // namespace testscript

using namespace cpgf;
using namespace testscript;
using namespace std;


#endif


