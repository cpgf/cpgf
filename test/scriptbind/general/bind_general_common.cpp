#include "bind_general_common.h"


using namespace cpgf;


namespace testscript {


TestScriptCoder::~TestScriptCoder()
{
}


TestScriptContext::TestScriptContext(TestScriptCoder * coder)
	: service(createDefaultMetaService()), coder(coder)
{
}

TestScriptContext::~TestScriptContext()
{
}



} // namespace testscript



