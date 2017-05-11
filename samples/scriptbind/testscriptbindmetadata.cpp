#include "cpgf/gmetadefine.h"
#include "cpgf/goutmain.h"

#include "testscriptbindmetadata.h"

#include <string>

namespace testscript {

void bindBasicData(cpgf::GScriptObject * script)
{
	using namespace cpgf;

	scriptSetValue(script, "Magic1", GScriptValue::fromPrimary(Magic1));
	scriptSetValue(script, "Magic2", GScriptValue::fromPrimary(Magic2));
	scriptSetValue(script, "Magic3", GScriptValue::fromPrimary(Magic3));
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

