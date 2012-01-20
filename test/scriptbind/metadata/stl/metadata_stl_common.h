#ifndef __METADATA_STL_COMMON_H
#define __METADATA_STL_COMMON_H


#include "../metadata_common.h"

#include <string>


using namespace std;

namespace test_metadata_stl {

class StlTestData
{
public:
	StlTestData() : value(0) {
	}
	
	explicit StlTestData(int value) : value(value) {
	}

	bool operator == (const StlTestData & other) const {
		return this->value == other.value;
	}
	
	bool operator < (const StlTestData & other) const {
		return this->value < other.value;
	}
	
	bool operator <= (const StlTestData & other) const {
		return this->value <= other.value;
	}
	
	bool equals(const StlTestData & other) const {
		return this->value == other.value;
	}
	
public:
	int value;	
};

class GTestValueProvider
{
public:
	explicit GTestValueProvider(TestScriptContext * context);
	virtual ~GTestValueProvider();

	virtual string value(int id) const = 0;
	virtual string equals(const string & a, const string & b) const = 0;
	
protected:
	TestScriptContext * context;	
};


class GTestValueProvider_Int : public GTestValueProvider
{
public:
	explicit GTestValueProvider_Int(TestScriptContext * context) : GTestValueProvider(context) {}
	
	virtual string value(int id) const;
	virtual string equals(const string & a, const string & b) const;
};


class GTestValueProvider_TestData : public GTestValueProvider
{
public:
	explicit GTestValueProvider_TestData(TestScriptContext * context) : GTestValueProvider(context) {}
	
	virtual string value(int id) const;
	virtual string equals(const string & a, const string & b) const;
};


class GTestValueProvider_String : public GTestValueProvider
{
public:
	explicit GTestValueProvider_String(TestScriptContext * context) : GTestValueProvider(context) {}
	
	virtual string value(int id) const;
	virtual string equals(const string & a, const string & b) const;
};


void bindCommonData(TestScriptContext * context);


inline void stlContainer_AddValue(TestScriptContext * context, int count, const string & method, int start, const GTestValueProvider & valueProvider)
{
	(void)context; (void)valueProvider;

	for(int i = 0; i < count; ++i) {
		DO(method + "(" + valueProvider.value(start + i) + ")")
	}
}

inline void stlContainer_VerifyContainer(TestScriptContext * context, const string & name, int count, int start, const GTestValueProvider & valueProvider)
{
	DO(string("i =") + name + ".begin()")
	for(int i = 0; i < count; ++i) {
		DOASSERTNOT("i.equals(" + name + "._end())")
		DOASSERT(valueProvider.equals("i.value()", valueProvider.value(start + i)))
		QDO(i.inc())
	}
}


template <typename Container, typename Binding>
void testStlContainer_Empty(Binding * binding, TestScriptContext * context, const string & className, const GTestValueProvider & valueProvider)
{
	(void)binding; (void)context; (void)className; (void)valueProvider;

	NEWOBJ("a", className + "()");

	QDO(it = a.begin())
	QASSERT(it.equals(a._end()))

	QASSERT(a.size() == 0)
	QASSERT(a.empty())
}


template <typename Container, typename Binding>
void testStlContainer_Set(Binding * binding, TestScriptContext * context, const string & className, const GTestValueProvider & valueProvider)
{
	(void)binding; (void)context; (void)className; (void)valueProvider;

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


template <typename Container, typename Binding>
void testStlContainer_Pushback(Binding * binding, TestScriptContext * context, const string & className, const GTestValueProvider & valueProvider)
{
	(void)binding; (void)context; (void)className; (void)valueProvider;

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


template <typename Container, typename Binding>
void testStlContainer_Popback(Binding * binding, TestScriptContext * context, const string & className, const GTestValueProvider & valueProvider)
{
	(void)binding; (void)context; (void)className; (void)valueProvider;

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


template <typename Container, typename Binding>
void testStlContainer_At(Binding * binding, TestScriptContext * context, const string & className, const GTestValueProvider & valueProvider)
{
	(void)binding; (void)context; (void)className; (void)valueProvider;

	NEWOBJ("a", className + "()");

	stlContainer_AddValue(context, 3, "a.push_back", 0, valueProvider);

	DOASSERT(valueProvider.equals("a.at(0)", valueProvider.value(0)))
	DOASSERT(valueProvider.equals("a.at(1)", valueProvider.value(1)))
	DOASSERT(valueProvider.equals("a.at(2)", valueProvider.value(2)))
}


template <typename Container, typename Binding>
void testStlContainer_FrontBack(Binding * binding, TestScriptContext * context, const string & className, const GTestValueProvider & valueProvider)
{
	(void)binding; (void)context; (void)className; (void)valueProvider;

	NEWOBJ("a", className + "()");

	stlContainer_AddValue(context, 3, "a.push_back", 0, valueProvider);

	DOASSERT(valueProvider.equals("a.front()", valueProvider.value(0)))
	DOASSERT(valueProvider.equals("a.back()", valueProvider.value(2)))
}


template <typename Container, typename Binding>
void testStlContainer_Assign(Binding * binding, TestScriptContext * context, const string & className, const GTestValueProvider & valueProvider)
{
	(void)binding; (void)context; (void)className; (void)valueProvider;

	NEWOBJ("a", className + "()");

	stlContainer_AddValue(context, 3, "a.push_back", 0, valueProvider);

	DO(string("a.assign(2, ") + valueProvider.value(38) + ")")
	QASSERT(a.size() == 2)

	DOASSERT(valueProvider.equals("a.front()", valueProvider.value(38)))
	DOASSERT(valueProvider.equals("a.back()", valueProvider.value(38)))
}


template <typename Container, typename Binding>
void testStlContainer_ForwardIterator(Binding * binding, TestScriptContext * context, const string & className, const GTestValueProvider & valueProvider)
{
	(void)binding; (void)context; (void)className; (void)valueProvider;

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


template <typename Container, typename Binding>
void testStlContainer_BackwardIterator(Binding * binding, TestScriptContext * context, const string & className, const GTestValueProvider & valueProvider)
{
	(void)binding; (void)context; (void)className; (void)valueProvider;

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


template <typename Container, typename Binding>
void testStlContainer_ReverseForwardIterator(Binding * binding, TestScriptContext * context, const string & className, const GTestValueProvider & valueProvider)
{
	(void)binding; (void)context; (void)className; (void)valueProvider;

	NEWOBJ("a", className + "()");

	stlContainer_AddValue(context, 3, "a.push_back", 0, valueProvider);

	QDO(i = a.rbegin())
	DOASSERT(valueProvider.equals("i.value()", valueProvider.value(2)))
	QDO(i = i.inc())
	DOASSERT(valueProvider.equals("i.value()", valueProvider.value(1)))
	QDO(i.inc())
	DOASSERT(valueProvider.equals("i.value()", valueProvider.value(0)))
}


template <typename Container, typename Binding>
void testStlContainer_ReverseBackwardIterator(Binding * binding, TestScriptContext * context, const string & className, const GTestValueProvider & valueProvider)
{
	(void)binding; (void)context; (void)className; (void)valueProvider;

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


template <typename Container, typename Binding>
void testStlContainer_RandomAccessIterator(Binding * binding, TestScriptContext * context, const string & className, const GTestValueProvider & valueProvider)
{
	(void)binding; (void)context; (void)className; (void)valueProvider;

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


template <typename Container, typename Binding>
void testStlContainer_ReverseRandomAccessIterator(Binding * binding, TestScriptContext * context, const string & className, const GTestValueProvider & valueProvider)
{
	(void)binding; (void)context; (void)className; (void)valueProvider;

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

template <typename Container, typename Binding>
void testStlContainer_Swap(Binding * binding, TestScriptContext * context, const string & className, const GTestValueProvider & valueProvider)
{
	(void)binding; (void)context; (void)className; (void)valueProvider;

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


template <typename Container, typename Binding>
void testStlContainer_Erase(Binding * binding, TestScriptContext * context, const string & className, const GTestValueProvider & valueProvider)
{
	(void)binding; (void)context; (void)className; (void)valueProvider;

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


template <typename Container, typename Binding>
void testStlContainer_Insert(Binding * binding, TestScriptContext * context, const string & className, const GTestValueProvider & valueProvider)
{
	(void)binding; (void)context; (void)className; (void)valueProvider;

	NEWOBJ("a", className + "()");

	stlContainer_AddValue(context, 5, "a.push_back", 1, valueProvider);

	QASSERT(a.size() == 5)
	QDO(i = a.begin())
	DO(string("a.insert(i, ") + valueProvider.value(0) + ")")
	QASSERT(a.size() == 6)
	stlContainer_VerifyContainer(context, "a", 6, 0, valueProvider);
}


template <typename Container, typename Binding>
void testStlContainer_Clear(Binding * binding, TestScriptContext * context, const string & className, const GTestValueProvider & valueProvider)
{
	(void)binding; (void)context; (void)className; (void)valueProvider;

	NEWOBJ("a", className + "()");

	stlContainer_AddValue(context, 5, "a.push_back", 1, valueProvider);

	QASSERT(a.size() == 5)
	QASSERTNOT(a.empty())

	QDO(a.clear())
	QASSERT(a.size() == 0)
	QASSERT(a.empty())
}




} // namespace test_metadata_stl


using namespace test_metadata_stl;



#endif
