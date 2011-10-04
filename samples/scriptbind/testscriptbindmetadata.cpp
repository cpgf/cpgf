#include "cpgf/gmetaclass.h"

#include "testscriptbindmetadata.h"

#include <string>

namespace testscript {

void bindBasicData(cpgf::GScriptObject * script)
{
	script->setFundamental("Magic1", Magic1);
	script->setFundamental("Magic2", Magic2);
	script->setFundamental("Magic3", Magic3);
}


GMETA_DEFINE_CLASS(TestData, TestData, "testscript::TestData") {
	using namespace cpgf;

	GMETA_FIELD(x);
	GMETA_FIELD(name);
}

GMETA_DEFINE_CLASS(TestObject, TestObject, "testscript::TestObject") {
	using namespace cpgf;

	GMETA_FIELD(value);
	reflectConstructor<void * (const TestObject &)>();
}



}

