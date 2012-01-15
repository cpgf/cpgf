#include "metadata_stl_common.h"
#include "cpgf/metadata/stl/gmetadata_list.h"
#include "cpgf/metadata/gmetadatapolicydeduce.h"

#include <list>


namespace {


template <typename Container, typename Binding>
void testStlContainer_Splice(Binding * binding, TestScriptContext * context, const string & className, const GTestValueProvider & valueProvider)
{
	(void)binding; (void)context; (void)className; (void)valueProvider;

	NEWOBJ("a", className + "()");

	stlContainer_AddValue(context, 5, "a.push_back", 0, valueProvider);

	NEWOBJ("b", className + "()");

	stlContainer_AddValue(context, 1, "b.push_back", 10, valueProvider);

	// a = 0 1 2 3 4
	// b = 10

	QASSERT(a.size() == 5)
	QASSERT(b.size() == 1)

	QDO(b.splice(b.begin(), a, a.begin().inc()))
	// a = 0 2 3 4
	// b = 1 10
	QASSERT(a.size() == 4)
	QASSERT(b.size() == 2)
	DOASSERT(valueProvider.equals("b.front()", valueProvider.value(1)))
	DOASSERT(valueProvider.equals("b.back()", valueProvider.value(10)))
	DOASSERT(valueProvider.equals("a.begin().value()", valueProvider.value(0)))
	DOASSERT(valueProvider.equals("a.begin().inc().value()", valueProvider.value(2)))

	QDO(b.splice(b.begin().inc(), a, a.begin().inc(), a.begin().inc().inc().inc()))
	// a = 0 4
	// b = 1 2 3 10
	QASSERT(a.size() == 2)
	QASSERT(b.size() == 4)
	DOASSERT(valueProvider.equals("a.begin().value()", valueProvider.value(0)))
	DOASSERT(valueProvider.equals("a.begin().inc().value()", valueProvider.value(4)))
	DOASSERT(valueProvider.equals("b.begin().value()", valueProvider.value(1)))
	DOASSERT(valueProvider.equals("b.begin().inc().value()", valueProvider.value(2)))
	DOASSERT(valueProvider.equals("b.begin().inc().inc().value()", valueProvider.value(3)))
	DOASSERT(valueProvider.equals("b.begin().inc().inc().inc().value()", valueProvider.value(10)))

	QDO(b.splice(b.begin(), a))
	// a = empty
	// b = 0 4 1 2 3 10
	QASSERT(a.size() == 0)
	QASSERT(b.size() == 6)
	DOASSERT(valueProvider.equals("b.front()", valueProvider.value(0)))
	DOASSERT(valueProvider.equals("b.back()", valueProvider.value(10)))
	DOASSERT(valueProvider.equals("b.begin().inc().value()", valueProvider.value(4)))
	DOASSERT(valueProvider.equals("b.begin().inc().inc().inc().value()", valueProvider.value(2)))
}


template <typename Container, typename Binding>
void testStlContainer_Remove(Binding * binding, TestScriptContext * context, const string & className, const GTestValueProvider & valueProvider)
{
	(void)binding; (void)context; (void)className; (void)valueProvider;

	NEWOBJ("a", className + "()");

	stlContainer_AddValue(context, 5, "a.push_back", 0, valueProvider);
	
	DO(string("a.remove(") + valueProvider.value(0) + ")")
	DO(string("a.remove(") + valueProvider.value(2) + ")")
	DO(string("a.remove(") + valueProvider.value(4) + ")")

	QASSERT(a.size() == 2)
	DOASSERT(valueProvider.equals("a.begin().value()", valueProvider.value(1)))
	DOASSERT(valueProvider.equals("a.begin().inc().value()", valueProvider.value(3)))
}

template <typename Container, typename Binding>
void testStlContainer_Unique(Binding * binding, TestScriptContext * context, const string & className, const GTestValueProvider & valueProvider)
{
	(void)binding; (void)context; (void)className; (void)valueProvider;

	NEWOBJ("a", className + "()");

	DO(string("a.push_back(") + valueProvider.value(1) + ")")
	DO(string("a.push_back(") + valueProvider.value(2) + ")")
	DO(string("a.push_back(") + valueProvider.value(2) + ")")
	DO(string("a.push_back(") + valueProvider.value(3) + ")")
	DO(string("a.push_back(") + valueProvider.value(3) + ")")

	QASSERT(a.size() == 5)
	QDO(a.unique())
	QASSERT(a.size() == 3)
	DOASSERT(valueProvider.equals("a.begin().value()", valueProvider.value(1)))
	DOASSERT(valueProvider.equals("a.begin().inc().value()", valueProvider.value(2)))
	DOASSERT(valueProvider.equals("a.begin().inc().inc().value()", valueProvider.value(3)))
}


template <typename Container, typename Binding>
void testStlContainer_Merge(Binding * binding, TestScriptContext * context, const string & className, const GTestValueProvider & valueProvider)
{
	(void)binding; (void)context; (void)className; (void)valueProvider;

	NEWOBJ("a", className + "()");

	stlContainer_AddValue(context, 3, "a.push_back", 0, valueProvider);

	NEWOBJ("b", className + "()");

	stlContainer_AddValue(context, 2, "b.push_back", 1, valueProvider);

	QDO(a.merge(b))
	QASSERT(a.size() == 5)
	QASSERT(b.size() == 0)
	DOASSERT(valueProvider.equals("a.begin().value()", valueProvider.value(0)))
	DOASSERT(valueProvider.equals("a.begin().inc().value()", valueProvider.value(1)))
	DOASSERT(valueProvider.equals("a.begin().inc().inc().value()", valueProvider.value(1)))
	DOASSERT(valueProvider.equals("a.begin().inc().inc().inc().value()", valueProvider.value(2)))
	DOASSERT(valueProvider.equals("a.begin().inc().inc().inc().inc().value()", valueProvider.value(2)))
}


template <typename Container, typename Binding>
void testStlContainer_Sort(Binding * binding, TestScriptContext * context, const string & className, const GTestValueProvider & valueProvider)
{
	(void)binding; (void)context; (void)className; (void)valueProvider;

	NEWOBJ("a", className + "()");

	DO(string("a.push_back(") + valueProvider.value(1) + ")")
	DO(string("a.push_back(") + valueProvider.value(4) + ")")
	DO(string("a.push_back(") + valueProvider.value(2) + ")")
	DO(string("a.push_back(") + valueProvider.value(0) + ")")
	DO(string("a.push_back(") + valueProvider.value(3) + ")")

	QDO(a.sort())
	QASSERT(a.size() == 5)
	stlContainer_VerifyContainer(context, "a", 5, 0, valueProvider);
}


template <typename Container, typename Binding>
void testStlContainer_Reverse(Binding * binding, TestScriptContext * context, const string & className, const GTestValueProvider & valueProvider)
{
	(void)binding; (void)context; (void)className; (void)valueProvider;

	NEWOBJ("a", className + "()");

	DO(string("a.push_back(") + valueProvider.value(4) + ")")
	DO(string("a.push_back(") + valueProvider.value(3) + ")")
	DO(string("a.push_back(") + valueProvider.value(2) + ")")
	DO(string("a.push_back(") + valueProvider.value(1) + ")")
	DO(string("a.push_back(") + valueProvider.value(0) + ")")

	QDO(a.reverse())
	QASSERT(a.size() == 5)
	stlContainer_VerifyContainer(context, "a", 5, 0, valueProvider);
}


template <typename Container, typename Binding>
void testStlList_Helper(Binding * binding, TestScriptContext * c, const char * className, const GTestValueProvider & valueProvider)
{
	GMetaDataNameReplacer replacer;
	initializeLuaReplacer(&replacer);
	GDefineMetaClass<Container> define = GDefineMetaClass<Container>::declare(className);
	buildMetaData_list(define, true, typename GMetaDataPolicyDeduce<typename Container::value_type>::Result(), &replacer);

	GScopedInterface<IMetaClass> metaClass(static_cast<IMetaClass *>(metaItemToInterface(define.getMetaClass())));
	binding->bindClass(className, metaClass.get());

	GScopedPointer<TestScriptContext> context(c);

	testStlContainer_Empty<Container>(binding, c, className, valueProvider);
	testStlContainer_Pushback<Container>(binding, c, className, valueProvider);
	testStlContainer_Popback<Container>(binding, c, className, valueProvider);
	testStlContainer_FrontBack<Container>(binding, c, className, valueProvider);
	testStlContainer_Assign<Container>(binding, c, className, valueProvider);

	testStlContainer_ForwardIterator<Container>(binding, c, className, valueProvider);
	testStlContainer_BackwardIterator<Container>(binding, c, className, valueProvider);
	testStlContainer_ReverseForwardIterator<Container>(binding, c, className, valueProvider);
	testStlContainer_ReverseBackwardIterator<Container>(binding, c, className, valueProvider);
	
	testStlContainer_Swap<Container>(binding, c, className, valueProvider);
	testStlContainer_Erase<Container>(binding, c, className, valueProvider);
	testStlContainer_Insert<Container>(binding, c, className, valueProvider);
	testStlContainer_Clear<Container>(binding, c, className, valueProvider);
	
	testStlContainer_Splice<Container>(binding, c, className, valueProvider);
	testStlContainer_Remove<Container>(binding, c, className, valueProvider);
	testStlContainer_Unique<Container>(binding, c, className, valueProvider);
	testStlContainer_Merge<Container>(binding, c, className, valueProvider);
	testStlContainer_Sort<Container>(binding, c, className, valueProvider);
	testStlContainer_Reverse<Container>(binding, c, className, valueProvider);

	context.reset();
}

template <typename Binding>
void test_stl_list_int(Binding * binding, TestScriptContext * context)
{
	testStlList_Helper<list<int> >(binding, context, "vector_int", GTestValueProvider_Int(context));
}


#define CASE test_stl_list_int
#include "metadata_stl_testcase.h"


template <typename Binding>
void test_stl_list_object(Binding * binding, TestScriptContext * context)
{
	testStlList_Helper<list<StlTestData> >(binding, context, "vector_obj", GTestValueProvider_TestData(context));
}


#define CASE test_stl_list_object
#include "metadata_stl_testcase.h"


template <typename Binding>
void test_stl_list_string(Binding * binding, TestScriptContext * context)
{
	testStlList_Helper<list<string> >(binding, context, "vector_string", GTestValueProvider_String(context));
}


#define CASE test_stl_list_string
#include "metadata_stl_testcase.h"



}
