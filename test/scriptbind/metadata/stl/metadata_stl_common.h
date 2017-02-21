#ifndef CPGF_METADATA_STL_COMMON_H
#define CPGF_METADATA_STL_COMMON_H


#include "../../../testmetatraits.h"

#include "../metadata_common.h"

#include <string>

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4503) // warning C503: decorated name length exceeded, name was truncated
#endif


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


void stlContainer_AddValue(TestScriptContext * context, int count, const string & method, int start, const GTestValueProvider & valueProvider);
void stlContainer_VerifyContainer(TestScriptContext * context, const string & name, int count, int start, const GTestValueProvider & valueProvider);

void testStlContainer_Empty(TestScriptContext * context, const string & className, const GTestValueProvider & valueProvider);
void testStlContainer_Set(TestScriptContext * context, const string & className, const GTestValueProvider & valueProvider);
void testStlContainer_Pushback(TestScriptContext * context, const string & className, const GTestValueProvider & valueProvider);
void testStlContainer_Popback(TestScriptContext * context, const string & className, const GTestValueProvider & valueProvider);
void testStlContainer_At(TestScriptContext * context, const string & className, const GTestValueProvider & valueProvider);
void testStlContainer_FrontBack(TestScriptContext * context, const string & className, const GTestValueProvider & valueProvider);
void testStlContainer_Assign(TestScriptContext * context, const string & className, const GTestValueProvider & valueProvider);
void testStlContainer_ForwardIterator(TestScriptContext * context, const string & className, const GTestValueProvider & valueProvider);
void testStlContainer_BackwardIterator(TestScriptContext * context, const string & className, const GTestValueProvider & valueProvider);
void testStlContainer_ReverseForwardIterator(TestScriptContext * context, const string & className, const GTestValueProvider & valueProvider);
void testStlContainer_ReverseBackwardIterator(TestScriptContext * context, const string & className, const GTestValueProvider & valueProvider);
void testStlContainer_RandomAccessIterator(TestScriptContext * context, const string & className, const GTestValueProvider & valueProvider);
void testStlContainer_ReverseRandomAccessIterator(TestScriptContext * context, const string & className, const GTestValueProvider & valueProvider);
void testStlContainer_Swap(TestScriptContext * context, const string & className, const GTestValueProvider & valueProvider);
void testStlContainer_Erase(TestScriptContext * context, const string & className, const GTestValueProvider & valueProvider);
void testStlContainer_Insert(TestScriptContext * context, const string & className, const GTestValueProvider & valueProvider);
void testStlContainer_Clear(TestScriptContext * context, const string & className, const GTestValueProvider & valueProvider);

void testStlContainer_Splice(TestScriptContext * context, const string & className, const GTestValueProvider & valueProvider);
void testStlContainer_Remove(TestScriptContext * context, const string & className, const GTestValueProvider & valueProvider);
void testStlContainer_Unique(TestScriptContext * context, const string & className, const GTestValueProvider & valueProvider);
void testStlContainer_Merge(TestScriptContext * context, const string & className, const GTestValueProvider & valueProvider);
void testStlContainer_Sort(TestScriptContext * context, const string & className, const GTestValueProvider & valueProvider);
void testStlContainer_Reverse(TestScriptContext * context, const string & className, const GTestValueProvider & valueProvider);

void testStlContainer_Pushfront(TestScriptContext * context, const string & className, const GTestValueProvider & valueProvider);
void testStlContainer_Popfront(TestScriptContext * context, const string & className, const GTestValueProvider & valueProvider);



} // namespace test_metadata_stl


using namespace test_metadata_stl;



#endif
