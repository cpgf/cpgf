#include "test_reflection_common.h"

#define NAME(cls) GPP_STRINGIZE(cls)

using namespace std;
using namespace cpgf;

namespace Test_ClassHierarchy { namespace {


class ClassA {
public:
	int fieldA;

	virtual int getID() { return 1; }

	void methodA() {}
};

class ClassB : public ClassA {
public:
	int fieldB;
	
	virtual int getID() { return 2; }
};



} }

