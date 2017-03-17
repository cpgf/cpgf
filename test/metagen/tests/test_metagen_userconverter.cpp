#include "../include/metagen_userconverter.h"
#include "cpgf/scriptbind/gscriptuserconverter.h"
#include "cpgf/gmetaapiutil.h"

#include "testmetagen.h"

#include <string>

class MyScriptUserConverter : public cpgf::GScriptUserConverterBase
{
protected:
	virtual uint32_t G_API_CC canConvert(const cpgf::GScriptUserConverterParamData * paramData);
	virtual void G_API_CC convert(cpgf::GVariantData * outputValue, const cpgf::GScriptUserConverterParamData * paramData, uint32_t tag);
};

uint32_t G_API_CC MyScriptUserConverter::canConvert(const cpgf::GScriptUserConverterParamData * paramData)
{
	using namespace cpgf;

	const std::string checkParamName = "TestUserConverterData";

	GVariant value = createVariantFromData(paramData->sourceValue->value);
	GVariantType vt = value.getType();
	if((vtIsInteger(vt) || vtIsReal(vt)) && paramData->callable->getParamCount() == 1) {
		GMetaType paramType(metaGetParamType(paramData->callable, 0));
		if(! paramType.isPointer() && paramType.getBaseName() != NULL) {
			if(checkParamName == paramType.getBaseName()) {
				return 1;
			}
		}
	}
	return 0;
}

void G_API_CC MyScriptUserConverter::convert(cpgf::GVariantData * outputValue,
	const cpgf::GScriptUserConverterParamData * paramData, uint32_t tag)
{
	switch(tag) {
	case 1: // converter number to TestUserConverterData
		GVariant value = createVariantFromData(paramData->sourceValue->value);
		TestUserConverterData data(fromVariant<int>(value));
		GVariant output = createVariant<TestUserConverterData>(data, true);
		*outputValue = output.takeData();
		break;
	}
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

	// Verify that the function doesn't work before adding converter
	QERR(mtest.testUserConverterGetDoubleValue(5));

	// We add/remove the converter to script context in the test.
	// For normal usage, we should only add the converter only once.
	scriptContext->addScriptUserConverter(converter.get());

	// Add the converter twice, but only one copy should be kept.
	scriptContext->addScriptUserConverter(converter.get());

	QDO(a = mtest.testUserConverterGetDoubleValue(5));
	QASSERT(a == 10);

	scriptContext->removeScriptUserConverter(converter.get());
	
	// Verify that the converter was removed successfully
	QERR(mtest.testUserConverterGetDoubleValue(5));

	// Remove a non-exist converter, no exception should occur
	scriptContext->removeScriptUserConverter(converter.get());
}

#define CASE testScriptUserConverter
#include "do_testcase.h"



}
