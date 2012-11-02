#include "cpgf/scriptbind/gscriptbind.h"
#include "cpgf/scriptbind/gscriptbindapi.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/goutmain.h"

#include <iostream>

using namespace std;
using namespace cpgf;


template <typename T>
void bindClass(T * script, cpgf::IMetaService * service, const char * metaName, const char * bindName)
{
	using namespace cpgf;
	
	GScopedInterface<IMetaClass> metaClass(service->findClassByName(metaName));
	
	script->bindClass(bindName, metaClass.get());
}


template <typename T>
void bindMethod(T * script, cpgf::IMetaService * service, const char * metaName, const char * bindName)
{
	using namespace cpgf;
	
	GScopedInterface<IMetaModule> module(service->getModuleAt(0));
	GScopedInterface<IMetaClass> metaClass(module->getGlobalMetaClass());
	GScopedInterface<IMetaMethod> method(metaClass->getMethod(metaName));
	
	script->bindMethod(bindName, NULL, method.get());
}


template <typename T>
void bindBasicInfo(T * script, cpgf::IMetaService * service)
{
	bindClass(script, service, "metatest", "mtest");
	bindMethod(script, service, "scriptAssert", "scriptAssert");
}

void metagenBindBasicData(cpgf::GScriptObject * script, cpgf::IMetaService * service)
{
	bindBasicInfo(script, service);
}

void metagenBindBasicData(cpgf::IScriptObject * script, cpgf::IMetaService * service)
{
	bindBasicInfo(script, service);
}

void scriptAssert(bool b)
{
	if(! b) {
		cpgf::raiseException(1, "Script assertion failure!");
	}
}

G_AUTO_RUN_BEFORE_MAIN()
{
	GDefineMetaGlobal()
		._method("scriptAssert", &scriptAssert)
	;
}


