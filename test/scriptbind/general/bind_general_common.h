#ifndef __BIND_GENERAL_COMMON_H
#define __BIND_GENERAL_COMMON_H

#include "cpgf/gmetaapi.h"

#if ENABLE_LUA
#include "cpgf/scriptbind/gluabind.h"
#endif

#if ENABLE_V8
#include "cpgf/scriptbind/gv8bind.h"
#endif

#include "../testscriptbind.h"

#define QSTR(...) # __VA_ARGS__

#define DO(s) GCHECK(context->doString(s));
#define ERR(s) GCHECK(context->doError(s));
#define QDO(...) DO(# __VA_ARGS__)
#define QERR(...) ERR(# __VA_ARGS__)
#define QASSERT(...) QDO(scriptAssert(__VA_ARGS__))


namespace testscript {


class TestScriptCoder
{
public:
	virtual ~TestScriptCoder();
};

class TestScriptContext
{
public:
	TestScriptContext(TestScriptCoder * coder);
	virtual ~TestScriptContext();

private:
	cpgf::GScopedInterface<cpgf::IMetaService> service;
	cpgf::GScopedPointer<TestScriptCoder> coder;
};



} // namespace testscript




#endif


