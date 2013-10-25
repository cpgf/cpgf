#ifndef CPGF_TESTSCRIPTBINDMETADATA_H
#define CPGF_TESTSCRIPTBINDMETADATA_H


#include "cpgf/metatraits/gmetaconverter_string.h"
#include "cpgf/scriptbind/gscriptbind.h"


namespace testscript {

const int Magic1 = 0x1999;
const int Magic2 = 0xbeef38;
const int Magic3 = 0xf00d;

struct TestData
{
	int x;
	std::string name;
};

class TestObject
{
public:
	TestObject() : value(Magic1) {
	}
	
	TestObject(const TestObject & other) : value(Magic2) {
		(void)other;
	}
	
	TestObject & operator = (const TestObject & other) {
		(void)other;

		this->value = Magic3;

		return *this;
	}
	
public:
	int	value;
};


void bindBasicData(cpgf::GScriptObject * script);



}


#endif
