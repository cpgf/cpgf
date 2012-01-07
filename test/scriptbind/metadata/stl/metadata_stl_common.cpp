#include "metadata_stl_common.h"

#include "cpgf/gmetadefine.h"
#include "cpgf/goutmain.h"

#include <sstream>


namespace test_metadata_stl {


GTestValueProvider::GTestValueProvider(TestScriptContext * context)
	: context(context)
{
}

GTestValueProvider::~GTestValueProvider()
{
}


string GTestValueProvider_Int::value(int id) const
{
	stringstream stream;
	
	stream << id + 1;

	return stream.str();
}

string GTestValueProvider_Int::equal(const string & a, const string & b) const
{
	return a + " == " + b;
}


string GTestValueProvider_TestData::value(int id) const
{
	stringstream stream;
	
	stream << context->getCoder()->getNew() << " StlTestData(" << id + 1 << ")";

	return stream.str();
}

string GTestValueProvider_TestData::equal(const string & a, const string & b) const
{
	return a + ".equal(" + b + ")";
}


string GTestValueProvider_String::value(int id) const
{
	stringstream stream;
	
	stream << "\"" << id + 1 << "\"";

	return stream.str();
}

string GTestValueProvider_String::equal(const string & a, const string & b) const
{
	return a + " == " + b;
}


G_AUTO_RUN_BEFORE_MAIN()
{
	GDefineMetaClass<StlTestData>
		::define("testStl::StlTestData")
		._constructor<void *()>()
		._constructor<void *(int)>()
		._method("equal", &StlTestData::equal)
		._field("value", &StlTestData::value)
	;		
}

template <typename T>
void doBindCommonData(T * binding, IMetaService * service)
{
	GScopedInterface<IMetaClass> metaClass(service->findClassByName("testStl::StlTestData"));
	binding->bindClass("StlTestData", metaClass.get());
}

void bindCommonData(TestScriptContext * context)
{
	IScriptObject * bindingApi = context->getBindingApi();
	GScriptObject * bindingLib = context->getBindingLib();

	if(bindingLib) {
		doBindCommonData(bindingLib, context->getService());
	}
	
	if(bindingApi) {
		doBindCommonData(bindingApi, context->getService());
	}
}



} // namespace test_metadata_stl

