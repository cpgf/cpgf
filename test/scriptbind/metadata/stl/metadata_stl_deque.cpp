#include "metadata_stl_common.h"
#include "cpgf/metadata/stl/gmetadata_deque.h"
#include "cpgf/metadata/gmetadatapolicydeduce.h"

#include <deque>


namespace {


template <typename Container, typename Binding>
void testStlContainer_Pushfront(Binding * binding, TestScriptContext * context, const string & className, const GTestValueProvider & valueProvider)
{
	(void)binding; (void)context; (void)className; (void)valueProvider;

	NEWOBJ("a", className + "()");

	stlContainer_AddValue(context, 3, "a.push_front", 0, valueProvider);

	QASSERT(a.size() == 3)

	DOASSERT(valueProvider.equals("a.begin().value()", valueProvider.value(2)))
	DOASSERT(valueProvider.equals("a.begin().inc().value()", valueProvider.value(1)))
	DOASSERT(valueProvider.equals("a.begin().inc().inc().value()", valueProvider.value(0)))
	
	QDO(it = a.begin())
	QASSERTNOT(it.equals(a._end()))

	QASSERTNOT(a.empty())
}


template <typename Container, typename Binding>
void testStlContainer_Popfront(Binding * binding, TestScriptContext * context, const string & className, const GTestValueProvider & valueProvider)
{
	(void)binding; (void)context; (void)className; (void)valueProvider;

	NEWOBJ("a", className + "()");

	stlContainer_AddValue(context, 5, "a.push_back", 0, valueProvider);

	QASSERT(a.size() == 5)
	
	QDO(a.pop_front())
	QASSERT(a.size() == 4)
	stlContainer_VerifyContainer(context, "a", 4, 1, valueProvider);
	
	QDO(a.pop_front())
	QASSERT(a.size() == 3)
	stlContainer_VerifyContainer(context, "a", 3, 2, valueProvider);
}


template <typename Container, typename Binding>
void testStlDeque_Helper(Binding * binding, TestScriptContext * c, const char * className, const GTestValueProvider & valueProvider)
{
	GMetaDataNameReplacer replacer;
	initializeLuaReplacer(&replacer);
	GDefineMetaClass<Container> define = GDefineMetaClass<Container>::declare(className);
	buildMetaData_deque(define, typename GMetaDataPolicyDeduce<typename Container::value_type>::Result(), &replacer);

	GScopedInterface<IMetaClass> metaClass(static_cast<IMetaClass *>(metaItemToInterface(define.getMetaClass())));
	binding->bindClass(className, metaClass.get());

	GScopedPointer<TestScriptContext> context(c);

	testStlContainer_Empty<Container>(binding, c, className, valueProvider);
	testStlContainer_Pushfront<Container>(binding, c, className, valueProvider);
	testStlContainer_Popfront<Container>(binding, c, className, valueProvider);
	testStlContainer_Pushback<Container>(binding, c, className, valueProvider);
	testStlContainer_Popback<Container>(binding, c, className, valueProvider);
	testStlContainer_FrontBack<Container>(binding, c, className, valueProvider);
	testStlContainer_Assign<Container>(binding, c, className, valueProvider);
	testStlContainer_At<Container>(binding, c, className, valueProvider);

	testStlContainer_ForwardIterator<Container>(binding, c, className, valueProvider);
	testStlContainer_BackwardIterator<Container>(binding, c, className, valueProvider);
	testStlContainer_ReverseForwardIterator<Container>(binding, c, className, valueProvider);
	testStlContainer_ReverseBackwardIterator<Container>(binding, c, className, valueProvider);
	testStlContainer_RandomAccessIterator<Container>(binding, c, className, valueProvider);
	testStlContainer_ReverseRandomAccessIterator<Container>(binding, c, className, valueProvider);
	
	testStlContainer_Swap<Container>(binding, c, className, valueProvider);
	testStlContainer_Erase<Container>(binding, c, className, valueProvider);
	testStlContainer_Insert<Container>(binding, c, className, valueProvider);
	testStlContainer_Clear<Container>(binding, c, className, valueProvider);

	context.reset();
}

template <typename Binding>
void test_stl_deque_int(Binding * binding, TestScriptContext * context)
{
	testStlDeque_Helper<deque<int> >(binding, context, "deque_int", GTestValueProvider_Int(context));
}


#define CASE test_stl_deque_int
#include "metadata_stl_testcase.h"


template <typename Binding>
void test_stl_deque_object(Binding * binding, TestScriptContext * context)
{
	testStlDeque_Helper<deque<StlTestData> >(binding, context, "deque_obj", GTestValueProvider_TestData(context));
}


#define CASE test_stl_deque_object
#include "metadata_stl_testcase.h"


template <typename Binding>
void test_stl_deque_string(Binding * binding, TestScriptContext * context)
{
	testStlDeque_Helper<deque<string> >(binding, context, "deque_string", GTestValueProvider_String(context));
}


#define CASE test_stl_deque_string
#include "metadata_stl_testcase.h"



}
