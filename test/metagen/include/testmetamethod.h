#ifndef CPGF_TESTMETAMETHOD_H
#define CPGF_TESTMETAMETHOD_H

#include "cpgf/gcompiler.h"
#include "cpgf/gsharedptr.h"

#include <string>

#include <string.h>

class TestMethodMethodData
{
public:
	TestMethodMethodData() : magic(0x10e85f2a) {}

	bool check() const {
		return this->magic == 0x10e85f2a;
	}

private:
	int magic;
};

class TestMethodMethodClass
{
public:
	TestMethodMethodClass() : refString("referenceToString") {}
	
	bool func_null_38_abc(void * a, int b, const char * c) {
		return a == NULL && b == 38 && strcmp(c, "abc") == 0;
	}

	static bool G_API_CC sfunc_dEf_neg99(const std::string & a, int b) {
		return a == "dEf" && b == -99;
	}

	std::string func_return_What() const {
		return "What";
	}

	const std::string & func_return_ReferenceToString() const {
		return this->refString;
	}

	cpgf::GSharedPointer<TestMethodMethodData> func_return_data() const {
		return cpgf::GSharedPointer<TestMethodMethodData>(new TestMethodMethodData());
	}
	
private:
	std::string refString;
};



#endif
