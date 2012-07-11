#include "testscriptbindmetadata5.h"
#include "bind_common.h"
#include "cpgf/gmetadefine.h"

using namespace cpgf;
using namespace std;

namespace testscript {

int ScriptOverride::getValue()
{
	GScopedInterface<IScriptFunction> func(this->getScriptFunction("getValue"));
	if(func) {
		return fromVariant<int>(invokeScriptFunction(func.get(), this).getValue());
	}
	else {
		return super::getValue();
	}
}

void TestScriptBindMetaData5()
{
	GDefineMetaClass<ScriptOverride>
		::define("testscript::ScriptOverride")
		._constructor<void * (int)>()
		._method("getValue", &ScriptOverride::getValue)
		._field("n", &ScriptOverride::n)
	;
}

}

