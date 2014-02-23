#ifndef CPGF_TESTSCRIPTBINDMETADATA4_H
#define CPGF_TESTSCRIPTBINDMETADATA4_H

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

class BasicA
{
public:
	enum BasicEnum {
		a = 1, b = 2, c = 3
	};

public:
	class Inner {
	public:
		Inner() : x(5) {
		}
		
		int add() {
			++x;
			return x;
		}
		
	public:
		int x;

	};

public:
};

enum TestEnum {
	teCpp = 1, teLua = 2, teV8 = 3
};

int testAdd2(int a, int b);
int testAddN(const cpgf::GMetaVariadicParam * params);
int testAddCallback(cpgf::IScriptFunction * scriptFunction);
int testExecAddCallback();



}


#endif
