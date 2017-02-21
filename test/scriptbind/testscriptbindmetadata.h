#ifndef CPGF_TESTSCRIPTBINDMETADATA_H
#define CPGF_TESTSCRIPTBINDMETADATA_H


#include "../testmetatraits.h"

#include "cpgf/scriptbind/gscriptbind.h"
#include "cpgf/scriptbind/gscriptbindapi.h"
#include "cpgf/scriptbind/gscriptbindutil.h"
#include "cpgf/scriptbind/gscriptwrapper.h"


#include <string.h>

#include <string>


#define REG_NAME_TestObject "testscript::TestObject"
#define REG_NAME_TestData "testscript::TestData"
#define REG_NAME_TestEnum "testscript::TestEnum"
#define REG_NAME_BasicA "testscript::BasicA"


namespace testscript {

const int Magic1 = 0x1999;
const int Magic2 = 0xbeef38;
const int Magic3 = 0xf00d;

void bindBasicData(cpgf::GScriptObject * script, cpgf::IMetaService * service);
void bindBasicData(cpgf::IScriptObject * script, cpgf::IMetaService * service);

void scriptAssert(bool b);
void scriptNot(bool b);


const char * const testString = "TestingScript!";
const int testInt = 1978;
const int testObjValue = 2012;


}


#endif
