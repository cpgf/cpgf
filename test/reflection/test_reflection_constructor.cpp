#include "test_reflection_common.h"

#define CLASS TestClass_Constructor
#define NAME_CLASS GPP_STRINGIZE(CLASS)

#define CTOR(f) pointerAssign(ctor, metaClass->getConstructorAt(f))

using namespace std;
using namespace cpgf;

namespace Test_Constructor { namespace {

class CLASS {
public:
	int ci;
	string cs;

public:
	// 0
	CLASS() : ci(0), cs("") {
	}

	// 1
	CLASS(const CLASS & other) : ci(other.ci), cs(other.cs) {
	}

	// 2
	CLASS(int i) : ci(i), cs("") {
	}

	// 3
	CLASS(string s) : ci(0), cs(s) {
	}

	// 4
	CLASS(int i, string s) : ci(i), cs(s) {
	}

	// 5
	CLASS(const CLASS_DATA & data) : ci(data.i), cs(data.s) {
	}

	// 6
	CLASS(const CLASS_DATA * data) : ci(data->i), cs(data->s) {
	}

	// 7
	CLASS(const GMetaVariadicParam * params) {
		int total = 0;
		for(size_t i = 0; i < params->paramCount; ++i) {
			total += cpgf::fromVariant<int>(*(params->params[i]));
		}

		this->ci = total;
		this->cs = "Sum";
	}

};

GMETA_DEFINE_CLASS(CLASS, CLASS, NAME_CLASS) {
	reflectConstructor<void * ()>();
	reflectConstructor<void * (const CLASS &)>();
	reflectConstructor<void * (int)>();
	reflectConstructor<void * (string)>();
	reflectConstructor<void * (int, string)>();
	reflectConstructor<void * (const CLASS_DATA &)>();
	reflectConstructor<void * (const CLASS_DATA *)>();
	reflectConstructor<void * (const GMetaVariadicParam *)>();
}


GTEST(Lib_Exists)
{
	const GMetaClass * metaClass = findMetaClass(NAME_CLASS);
	GCHECK(metaClass);

	const GMetaConstructor * ctor;

	CTOR(0);
	GCHECK(ctor);

	CTOR(1);
	GCHECK(ctor);

	CTOR(2);
	GCHECK(ctor);

	CTOR(3);
	GCHECK(ctor);

	CTOR(4);
	GCHECK(ctor);

	CTOR(5);
	GCHECK(ctor);

	CTOR(6);
	GCHECK(ctor);

	CTOR(7);
	GCHECK(ctor);

	CTOR(8);
	GCHECK(! ctor);
}





} }

