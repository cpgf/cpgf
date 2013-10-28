#include "../include/metagen_userconverter.h"
#include "cpgf/scriptbind/gscriptuserconverter.h"
#include "cpgf/gmetaapiutil.h"

#include "testmetagen.h"

#include <string>

class MyScriptUserConverter : public cpgf::GScriptUserConverterBase
{
protected:
	virtual cpgf::gapi_bool G_API_CC canConvert(const cpgf::GScriptUserConverterParamData * paramData);
	virtual void G_API_CC convert(cpgf::GVariantData * outputValue, const cpgf::GScriptUserConverterParamData * paramData);
};

cpgf::gapi_bool G_API_CC MyScriptUserConverter::canConvert(const cpgf::GScriptUserConverterParamData * paramData)
{
	using namespace cpgf;

	const std::string checkParamName = "TestUserConverterData";

	GVariant value = createVariantFromData(paramData->sourceValue->value);
	GVariantType vt = value.getType();
	if((vtIsInteger(vt) || vtIsReal(vt)) && paramData->callable->getParamCount() == 1) {
		GMetaType paramType(metaGetParamType(paramData->callable, 0));
		if(paramType.getBaseName() != NULL) {
			if(checkParamName == paramType.getBaseName()) {
				return true;
			}
		}
	}
	return false;
}

void G_API_CC MyScriptUserConverter::convert(cpgf::GVariantData * outputValue, const cpgf::GScriptUserConverterParamData * paramData)
{
	GVariant value = createVariantFromData(paramData->sourceValue->value);
	TestUserConverterData data(fromVariant<int>(value));
	GVariant output = createVariant<true>(data, true);
	*outputValue = output.takeData();
}


namespace {

using namespace cpgf;

void testScriptUserConverter(TestScriptContext * context)
{
	GScopedInterface<IScriptUserConverter> converter(new MyScriptUserConverter);
	GScopedInterface<IScriptContext> scriptContext;

	if(context->getBindingLib()) {
		scriptContext.reset(context->getBindingLib()->getContext());
	}
	
	if(context->getBindingApi()) {
		scriptContext.reset(context->getBindingApi()->getContext());
	}

	scriptContext->addScriptUserConverter(converter.get());

	QDO(a = mtest.testUserConverterGetDoubleValue(5));
	QASSERT(a == 10);

	scriptContext->removeScriptUserConverter(converter.get());
}

#define CASE testScriptUserConverter
#include "do_testcase.h"



}
