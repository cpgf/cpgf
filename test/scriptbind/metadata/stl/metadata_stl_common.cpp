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

string GTestValueProvider_Int::equals(const string & a, const string & b) const
{
	return a + " == " + b;
}


string GTestValueProvider_TestData::value(int id) const
{
	stringstream stream;
	
	stream << context->getCoder()->getNew() << " StlTestData(" << id + 1 << ")";

	return stream.str();
}

string GTestValueProvider_TestData::equals(const string & a, const string & b) const
{
	return a + ".equals(" + b + ")";
}


string GTestValueProvider_String::value(int id) const
{
	stringstream stream;
	
	stream << "\"" << id + 1 << "\"";

	return stream.str();
}

string GTestValueProvider_String::equals(const string & a, const string & b) const
{
	return a + " == " + b;
}


G_AUTO_RUN_BEFORE_MAIN()
{
	GDefineMetaClass<StlTestData>
		::define("testStl::StlTestData")
		._constructor<void *()>()
		._constructor<void *(int)>()
		._method("equals", &StlTestData::equals)
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


void stlContainer_AddValue(TestScriptContext * context, int count, const string & method, int start, const GTestValueProvider & valueProvider)
{
	(void)context; (void)valueProvider;

	for(int i = 0; i < count; ++i) {
		DO(method + "(" + valueProvider.value(start + i) + ")")
	}
}

void stlContainer_VerifyContainer(TestScriptContext * context, const string & name, int count, int start, const GTestValueProvider & valueProvider)
{
	DO(string("i =") + name + ".begin()")
	for(int i = 0; i < count; ++i) {
		DOASSERTNOT("i.equals(" + name + "._end())")
		DOASSERT(valueProvider.equals("i.value()", valueProvider.value(start + i)))
		QDO(i.inc())
	}
}


void testStlContainer_Empty(TestScriptContext * context, const string & className, const GTestValueProvider & valueProvider)
{
	(void)context; (void)className; (void)valueProvider;

	NEWOBJ("a", className + "()");

	QDO(it = a.begin())
	QASSERT(it.equals(a._end()))

	QASSERT(a.size() == 0)
	QASSERT(a.empty())
}


void testStlContainer_Set(TestScriptContext * context, const string & className, const GTestValueProvider & valueProvider)
{
	(void)context; (void)className; (void)valueProvider;

	NEWOBJ("a", className + "()");

	DO(string("a.push_back(") + valueProvider.value(5) + ")")
	DO(string("a.push_back(") + valueProvider.value(5) + ")")
	DO(string("a.push_back(") + valueProvider.value(5) + ")")

	DOASSERT(valueProvider.equals("a.begin().value()", valueProvider.value(5)))
	DOASSERT(valueProvider.equals("a.begin().inc().value()", valueProvider.value(5)))
	DOASSERT(valueProvider.equals("a.begin().inc().inc().value()", valueProvider.value(5)))
	
	DO(string("a.begin().inc().set(") + valueProvider.value(1) + ")")

	DOASSERT(valueProvider.equals("a.begin().value()", valueProvider.value(5)))
	DOASSERT(valueProvider.equals("a.begin().inc().value()", valueProvider.value(1)))
	DOASSERT(valueProvider.equals("a.begin().inc().inc().value()", valueProvider.value(5)))
}


void testStlContainer_Pushback(TestScriptContext * context, const string & className, const GTestValueProvider & valueProvider)
{
	(void)context; (void)className; (void)valueProvider;

	NEWOBJ("a", className + "()");

	stlContainer_AddValue(context, 3, "a.push_back", 0, valueProvider);

	QASSERT(a.size() == 3)

	DOASSERT(valueProvider.equals("a.begin().value()", valueProvider.value(0)))
	DOASSERT(valueProvider.equals("a.begin().inc().value()", valueProvider.value(1)))
	DOASSERT(valueProvider.equals("a.begin().inc().inc().value()", valueProvider.value(2)))

	QDO(it = a.begin())
	QASSERTNOT(it.equals(a._end()))

	QASSERTNOT(a.empty())
}


void testStlContainer_Popback(TestScriptContext * context, const string & className, const GTestValueProvider & valueProvider)
{
	(void)context; (void)className; (void)valueProvider;

	NEWOBJ("a", className + "()");

	stlContainer_AddValue(context, 5, "a.push_back", 0, valueProvider);

	QASSERT(a.size() == 5)
	
	QDO(a.pop_back())
	QASSERT(a.size() == 4)
	stlContainer_VerifyContainer(context, "a", 4, 0, valueProvider);
	
	QDO(a.pop_back())
	QASSERT(a.size() == 3)
	stlContainer_VerifyContainer(context, "a", 3, 0, valueProvider);
}


void testStlContainer_At(TestScriptContext * context, const string & className, const GTestValueProvider & valueProvider)
{
	(void)context; (void)className; (void)valueProvider;

	NEWOBJ("a", className + "()");

	stlContainer_AddValue(context, 3, "a.push_back", 0, valueProvider);

	DOASSERT(valueProvider.equals("a.at(0)", valueProvider.value(0)))
	DOASSERT(valueProvider.equals("a.at(1)", valueProvider.value(1)))
	DOASSERT(valueProvider.equals("a.at(2)", valueProvider.value(2)))
}


void testStlContainer_FrontBack(TestScriptContext * context, const string & className, const GTestValueProvider & valueProvider)
{
	(void)context; (void)className; (void)valueProvider;

	NEWOBJ("a", className + "()");

	stlContainer_AddValue(context, 3, "a.push_back", 0, valueProvider);

	DOASSERT(valueProvider.equals("a.front()", valueProvider.value(0)))
	DOASSERT(valueProvider.equals("a.back()", valueProvider.value(2)))
}


void testStlContainer_Assign(TestScriptContext * context, const string & className, const GTestValueProvider & valueProvider)
{
	(void)context; (void)className; (void)valueProvider;

	NEWOBJ("a", className + "()");

	stlContainer_AddValue(context, 3, "a.push_back", 0, valueProvider);

	DO(string("a.assign(2, ") + valueProvider.value(38) + ")")
	QASSERT(a.size() == 2)

	DOASSERT(valueProvider.equals("a.front()", valueProvider.value(38)))
	DOASSERT(valueProvider.equals("a.back()", valueProvider.value(38)))
}


void testStlContainer_ForwardIterator(TestScriptContext * context, const string & className, const GTestValueProvider & valueProvider)
{
	(void)context; (void)className; (void)valueProvider;

	NEWOBJ("a", className + "()");

	stlContainer_AddValue(context, 3, "a.push_back", 0, valueProvider);

	QDO(i = a.begin())
	DOASSERT(valueProvider.equals("i.value()", valueProvider.value(0)))
	QDO(i = i.inc())
	DOASSERT(valueProvider.equals("i.value()", valueProvider.value(1)))
	QDO(i.inc())
	DOASSERT(valueProvider.equals("i.value()", valueProvider.value(2)))
	QDO(i.inc())
	QASSERT(i.equals(a._end()))
}


void testStlContainer_BackwardIterator(TestScriptContext * context, const string & className, const GTestValueProvider & valueProvider)
{
	(void)context; (void)className; (void)valueProvider;

	NEWOBJ("a", className + "()");

	stlContainer_AddValue(context, 3, "a.push_back", 0, valueProvider);

	QDO(i = a.begin())
	QDO(i.inc())
	QDO(i.inc())
	DOASSERT(valueProvider.equals("i.value()", valueProvider.value(2)))
	QDO(i = i.dec())
	DOASSERT(valueProvider.equals("i.value()", valueProvider.value(1)))
	QDO(i.dec())
	DOASSERT(valueProvider.equals("i.value()", valueProvider.value(0)))
}


void testStlContainer_ReverseForwardIterator(TestScriptContext * context, const string & className, const GTestValueProvider & valueProvider)
{
	(void)context; (void)className; (void)valueProvider;

	NEWOBJ("a", className + "()");

	stlContainer_AddValue(context, 3, "a.push_back", 0, valueProvider);

	QDO(i = a.rbegin())
	DOASSERT(valueProvider.equals("i.value()", valueProvider.value(2)))
	QDO(i = i.inc())
	DOASSERT(valueProvider.equals("i.value()", valueProvider.value(1)))
	QDO(i.inc())
	DOASSERT(valueProvider.equals("i.value()", valueProvider.value(0)))
}


void testStlContainer_ReverseBackwardIterator(TestScriptContext * context, const string & className, const GTestValueProvider & valueProvider)
{
	(void)context; (void)className; (void)valueProvider;

	NEWOBJ("a", className + "()");

	stlContainer_AddValue(context, 3, "a.push_back", 0, valueProvider);

	QDO(i = a.rbegin())
	DOASSERT(valueProvider.equals("i.value()", valueProvider.value(2)))
	QDO(i.inc())
	QDO(i.inc())
	DOASSERT(valueProvider.equals("i.value()", valueProvider.value(0)))
	QDO(i = i.dec())
	DOASSERT(valueProvider.equals("i.value()", valueProvider.value(1)))
	QDO(i.dec())
	DOASSERT(valueProvider.equals("i.value()", valueProvider.value(2)))
}


void testStlContainer_RandomAccessIterator(TestScriptContext * context, const string & className, const GTestValueProvider & valueProvider)
{
	(void)context; (void)className; (void)valueProvider;

	NEWOBJ("a", className + "()");

	stlContainer_AddValue(context, 5, "a.push_back", 0, valueProvider);

	QDO(i = a.begin())
	QDO(i.skip(2))
	DOASSERT(valueProvider.equals("i.value()", valueProvider.value(2)))
	QDO(i = i.skip(2))
	DOASSERT(valueProvider.equals("i.value()", valueProvider.value(4)))
	QDO(i.skip(-1))
	DOASSERT(valueProvider.equals("i.value()", valueProvider.value(3)))
}


void testStlContainer_ReverseRandomAccessIterator(TestScriptContext * context, const string & className, const GTestValueProvider & valueProvider)
{
	(void)context; (void)className; (void)valueProvider;

	NEWOBJ("a", className + "()");

	stlContainer_AddValue(context, 5, "a.push_back", 0, valueProvider);

	QDO(i = a.rbegin())
	DOASSERT(valueProvider.equals("i.value()", valueProvider.value(4)))
	QDO(i.skip(2))
	DOASSERT(valueProvider.equals("i.value()", valueProvider.value(2)))
	QDO(i = i.skip(2))
	DOASSERT(valueProvider.equals("i.value()", valueProvider.value(0)))
	QDO(i.skip(-1))
	DOASSERT(valueProvider.equals("i.value()", valueProvider.value(1)))
}


void testStlContainer_Swap(TestScriptContext * context, const string & className, const GTestValueProvider & valueProvider)
{
	(void)context; (void)className; (void)valueProvider;

	NEWOBJ("a", className + "()");

	stlContainer_AddValue(context, 3, "a.push_back", 0, valueProvider);

	NEWOBJ("b", className + "()");

	stlContainer_AddValue(context, 5, "b.push_back", 2, valueProvider);
	
	stlContainer_VerifyContainer(context, "a", 3, 0, valueProvider);
	stlContainer_VerifyContainer(context, "b", 5, 2, valueProvider);

	QDO(a.swap(b))
	
	stlContainer_VerifyContainer(context, "a", 5, 2, valueProvider);
	stlContainer_VerifyContainer(context, "b", 3, 0, valueProvider);
}


void testStlContainer_Erase(TestScriptContext * context, const string & className, const GTestValueProvider & valueProvider)
{
	(void)context; (void)className; (void)valueProvider;

	NEWOBJ("a", className + "()");

	stlContainer_AddValue(context, 5, "a.push_back", 0, valueProvider);

	QASSERT(a.size() == 5)
	QDO(i = a.begin())
	QDO(i = a.erase(i))
	QASSERT(a.size() == 4)
	DOASSERT(valueProvider.equals("a.front()", valueProvider.value(1)))
	DOASSERT(valueProvider.equals("i.value()", valueProvider.value(1)))
	QDO(i = a.erase(i))
	QASSERT(a.size() == 3)
	DOASSERT(valueProvider.equals("a.front()", valueProvider.value(2)))
	DOASSERT(valueProvider.equals("i.value()", valueProvider.value(2)))
}


void testStlContainer_Insert(TestScriptContext * context, const string & className, const GTestValueProvider & valueProvider)
{
	(void)context; (void)className; (void)valueProvider;

	NEWOBJ("a", className + "()");

	stlContainer_AddValue(context, 5, "a.push_back", 1, valueProvider);

	QASSERT(a.size() == 5)
	QDO(i = a.begin())
	DO(string("a.insert(i, ") + valueProvider.value(0) + ")")
	QASSERT(a.size() == 6)
	stlContainer_VerifyContainer(context, "a", 6, 0, valueProvider);
}


void testStlContainer_Clear(TestScriptContext * context, const string & className, const GTestValueProvider & valueProvider)
{
	(void)context; (void)className; (void)valueProvider;

	NEWOBJ("a", className + "()");

	stlContainer_AddValue(context, 5, "a.push_back", 1, valueProvider);

	QASSERT(a.size() == 5)
	QASSERTNOT(a.empty())

	QDO(a._clear())
	QASSERT(a.size() == 0)
	QASSERT(a.empty())
}


void testStlContainer_Pushfront(TestScriptContext * context, const string & className, const GTestValueProvider & valueProvider)
{
	(void)context; (void)className; (void)valueProvider;

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


void testStlContainer_Popfront(TestScriptContext * context, const string & className, const GTestValueProvider & valueProvider)
{
	(void)context; (void)className; (void)valueProvider;

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


void testStlContainer_Splice(TestScriptContext * context, const string & className, const GTestValueProvider & valueProvider)
{
	(void)context; (void)className; (void)valueProvider;

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


void testStlContainer_Remove(TestScriptContext * context, const string & className, const GTestValueProvider & valueProvider)
{
	(void)context; (void)className; (void)valueProvider;

	NEWOBJ("a", className + "()");

	stlContainer_AddValue(context, 5, "a.push_back", 0, valueProvider);
	
	DO(string("a.remove(") + valueProvider.value(0) + ")")
	DO(string("a.remove(") + valueProvider.value(2) + ")")
	DO(string("a.remove(") + valueProvider.value(4) + ")")

	QASSERT(a.size() == 2)
	DOASSERT(valueProvider.equals("a.begin().value()", valueProvider.value(1)))
	DOASSERT(valueProvider.equals("a.begin().inc().value()", valueProvider.value(3)))
}


void testStlContainer_Unique(TestScriptContext * context, const string & className, const GTestValueProvider & valueProvider)
{
	(void)context; (void)className; (void)valueProvider;

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


void testStlContainer_Merge(TestScriptContext * context, const string & className, const GTestValueProvider & valueProvider)
{
	(void)context; (void)className; (void)valueProvider;

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


void testStlContainer_Sort(TestScriptContext * context, const string & className, const GTestValueProvider & valueProvider)
{
	(void)context; (void)className; (void)valueProvider;

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


void testStlContainer_Reverse(TestScriptContext * context, const string & className, const GTestValueProvider & valueProvider)
{
	(void)context; (void)className; (void)valueProvider;

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





} // namespace test_metadata_stl

