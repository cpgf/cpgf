#ifndef CPGF_TESTSCRIPTBINDMETADATA6_H
#define CPGF_TESTSCRIPTBINDMETADATA6_H

#include "../testmetatraits.h"

#include "cpgf/gmetaclass.h"

#include "cpgf/scriptbind/gscriptbind.h"
#include "cpgf/scriptbind/gscriptbindapi.h"
#include "cpgf/scriptbind/gscriptbindutil.h"
#include "cpgf/scriptbind/gscriptwrapper.h"

#include "testscriptbind.h"

#include <string.h>

#include <string>


namespace testscript {

class TestObjectLeak
{
public:
	static int instanceCount;

public:
	TestObjectLeak();
	~TestObjectLeak();
};


}


#endif
