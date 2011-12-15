#include "cpgf/gmetadefine.h"
#include "cpgf/goutmain.h"

#include "testscriptbindmetadata.h"

#include <string>

namespace testscript {

void bindBasicData(cpgf::GScriptObject * script)
{
	script->bindFundamental("Magic1", Magic1);
	script->bindFundamental("Magic2", Magic2);
	script->bindFundamental("Magic3", Magic3);
}


G_AUTO_RUN_BEFORE_MAIN()
{
	using namespace cpgf;

	GDefineMetaClass<TestData>
		::define("testscript::TestData")
		
		._field("x", &TestData::x)
		._field("name", &TestData::name)
	;

	GDefineMetaClass<TestObject>
		::define("testscript::TestObject")

		._constructor<void * (const TestObject &)>()
		._field("value", &TestObject::value)
	;
}

}

