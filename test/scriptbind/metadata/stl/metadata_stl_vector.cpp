#include "metadata_stl_common.h"
#include "cpgf/metadata/stl/gmetadata_vector.h"
#include "cpgf/metadata/gmetadatapolicydeduce.h"


namespace {


template <typename Binding>
void testStlContainer_AddValue(Binding * binding, TestScriptContext * context, int count, const string & method, int start, const GTestValueProvider & valueProvider)
{
	(void)binding; (void)context; (void)valueProvider;

	for(int i = 0; i < count; ++i) {
		DO(method + "(" + valueProvider.value(start + i) + ")")
	}
}


template <typename Container, typename Binding>
void testStlContainer_Empty(Binding * binding, TestScriptContext * context, const string & className, const GTestValueProvider & valueProvider)
{
	(void)binding; (void)context; (void)className; (void)valueProvider;

	NEWOBJ("a", className + "()");

	QDO(it = a.begin())
	QASSERT(it.equal(a._end()))

	QASSERT(a.size() == 0)
	QASSERT(a.empty())
}


template <typename Container, typename Binding>
void testStlContainer_Pushback(Binding * binding, TestScriptContext * context, const string & className, const GTestValueProvider & valueProvider)
{
	(void)binding; (void)context; (void)className; (void)valueProvider;

	NEWOBJ("a", className + "()");

	testStlContainer_AddValue(binding, context, 3, "a.push_back", 0, valueProvider);

	QASSERT(a.size() == 3)
	
	QDO(it = a.begin())
	QASSERTNOT(it.equal(a._end()))

	QASSERTNOT(a.empty())
}


template <typename Container, typename Binding>
void testStlContainer_At(Binding * binding, TestScriptContext * context, const string & className, const GTestValueProvider & valueProvider)
{
	(void)binding; (void)context; (void)className; (void)valueProvider;

	NEWOBJ("a", className + "()");

	testStlContainer_AddValue(binding, context, 3, "a.push_back", 0, valueProvider);

	DOASSERT(valueProvider.equal("a.at(0)", valueProvider.value(0)))
	DOASSERT(valueProvider.equal("a.at(1)", valueProvider.value(1)))
	DOASSERT(valueProvider.equal("a.at(2)", valueProvider.value(2)))
}


template <typename Container, typename Binding>
void testStlContainer_FrontBack(Binding * binding, TestScriptContext * context, const string & className, const GTestValueProvider & valueProvider)
{
	(void)binding; (void)context; (void)className; (void)valueProvider;

	NEWOBJ("a", className + "()");

	testStlContainer_AddValue(binding, context, 3, "a.push_back", 0, valueProvider);

	DOASSERT(valueProvider.equal("a.front()", valueProvider.value(0)))
	DOASSERT(valueProvider.equal("a.back()", valueProvider.value(2)))
}


template <typename Container, typename Binding>
void testStlContainer_Assign(Binding * binding, TestScriptContext * context, const string & className, const GTestValueProvider & valueProvider)
{
	(void)binding; (void)context; (void)className; (void)valueProvider;

	NEWOBJ("a", className + "()");

	testStlContainer_AddValue(binding, context, 3, "a.push_back", 0, valueProvider);

	DO(string("a.assign(2, ") + valueProvider.value(38) + ")")
	QASSERT(a.size() == 2)

	DOASSERT(valueProvider.equal("a.at(0)", valueProvider.value(38)))
	DOASSERT(valueProvider.equal("a.at(1)", valueProvider.value(38)))
}


template <typename Container, typename Binding>
void testStlContainer_ForwardIterator(Binding * binding, TestScriptContext * context, const string & className, const GTestValueProvider & valueProvider)
{
	(void)binding; (void)context; (void)className; (void)valueProvider;

	NEWOBJ("a", className + "()");

	testStlContainer_AddValue(binding, context, 3, "a.push_back", 0, valueProvider);

	QDO(i = a.begin())
	DOASSERT(valueProvider.equal("i.value()", valueProvider.value(0)))
	QDO(i = i.inc())
	DOASSERT(valueProvider.equal("i.value()", valueProvider.value(1)))
	QDO(i.inc())
	DOASSERT(valueProvider.equal("i.value()", valueProvider.value(2)))
}


template <typename Container, typename Binding>
void testStlContainer_BackwardIterator(Binding * binding, TestScriptContext * context, const string & className, const GTestValueProvider & valueProvider)
{
	(void)binding; (void)context; (void)className; (void)valueProvider;

	NEWOBJ("a", className + "()");

	testStlContainer_AddValue(binding, context, 3, "a.push_back", 0, valueProvider);

	QDO(i = a.begin())
	QDO(i.inc())
	QDO(i.inc())
	DOASSERT(valueProvider.equal("i.value()", valueProvider.value(2)))
	QDO(i = i.dec())
	DOASSERT(valueProvider.equal("i.value()", valueProvider.value(1)))
	QDO(i.dec())
	DOASSERT(valueProvider.equal("i.value()", valueProvider.value(0)))
}


template <typename Container, typename Binding>
void testStlContainer_ReverseForwardIterator(Binding * binding, TestScriptContext * context, const string & className, const GTestValueProvider & valueProvider)
{
	(void)binding; (void)context; (void)className; (void)valueProvider;

	NEWOBJ("a", className + "()");

	testStlContainer_AddValue(binding, context, 3, "a.push_back", 0, valueProvider);

	QDO(i = a.rbegin())
	DOASSERT(valueProvider.equal("i.value()", valueProvider.value(2)))
	QDO(i = i.inc())
	DOASSERT(valueProvider.equal("i.value()", valueProvider.value(1)))
	QDO(i.inc())
	DOASSERT(valueProvider.equal("i.value()", valueProvider.value(0)))
}


template <typename Container, typename Binding>
void testStlContainer_ReverseBackwardIterator(Binding * binding, TestScriptContext * context, const string & className, const GTestValueProvider & valueProvider)
{
	(void)binding; (void)context; (void)className; (void)valueProvider;

	NEWOBJ("a", className + "()");

	testStlContainer_AddValue(binding, context, 3, "a.push_back", 0, valueProvider);

	QDO(i = a.rbegin())
	QDO(i.inc())
	QDO(i.inc())
	DOASSERT(valueProvider.equal("i.value()", valueProvider.value(0)))
	QDO(i = i.dec())
	DOASSERT(valueProvider.equal("i.value()", valueProvider.value(1)))
	QDO(i.dec())
	DOASSERT(valueProvider.equal("i.value()", valueProvider.value(2)))
}




template <typename Container, typename Binding>
void testStlVector_Helper(Binding * binding, TestScriptContext * c, const char * className, const GTestValueProvider & valueProvider)
{
	GMetaDataNameReplacer replacer;
	initializeLuaReplacer(&replacer);
	GDefineMetaClass<Container> define = GDefineMetaClass<Container>::declare(className);
	buildMetaData_vector(define, true, typename GMetaDataPolicyDeduce<typename Container::value_type>::Result(), &replacer);

	GScopedInterface<IMetaClass> metaClass(static_cast<IMetaClass *>(metaItemToInterface(define.getMetaClass())));
	binding->bindClass(className, metaClass.get());

	GScopedPointer<TestScriptContext> context(c);

	testStlContainer_Empty<Container>(binding, c, className, valueProvider);
	testStlContainer_Pushback<Container>(binding, c, className, valueProvider);
	testStlContainer_FrontBack<Container>(binding, c, className, valueProvider);
	testStlContainer_Assign<Container>(binding, c, className, valueProvider);

	testStlContainer_ForwardIterator<Container>(binding, c, className, valueProvider);
	testStlContainer_BackwardIterator<Container>(binding, c, className, valueProvider);
	testStlContainer_ReverseForwardIterator<Container>(binding, c, className, valueProvider);
	testStlContainer_ReverseBackwardIterator<Container>(binding, c, className, valueProvider);

	context.reset();
}

template <typename Binding>
void test_stl_vector_int(Binding * binding, TestScriptContext * context)
{
	testStlVector_Helper<vector<int> >(binding, context, "vector_int", GTestValueProvider_Int(context));
}


#define CASE test_stl_vector_int
#include "metadata_stl_testcase.h"


template <typename Binding>
void test_stl_vector_object(Binding * binding, TestScriptContext * context)
{
	testStlVector_Helper<vector<StlTestData> >(binding, context, "vector_obj", GTestValueProvider_TestData(context));
}


#define CASE test_stl_vector_object
#include "metadata_stl_testcase.h"


template <typename Binding>
void test_stl_vector_string(Binding * binding, TestScriptContext * context)
{
	testStlVector_Helper<vector<string> >(binding, context, "vector_string", GTestValueProvider_String(context));
}


#define CASE test_stl_vector_string
#include "metadata_stl_testcase.h"



}
