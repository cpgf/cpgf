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
	
	bool equal(const StlTestData & other) const {
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
	virtual string equal(const string & a, const string & b) const = 0;
	
protected:
	TestScriptContext * context;	
};


class GTestValueProvider_Int : public GTestValueProvider
{
public:
	explicit GTestValueProvider_Int(TestScriptContext * context) : GTestValueProvider(context) {}
	
	virtual string value(int id) const;
	virtual string equal(const string & a, const string & b) const;
};


class GTestValueProvider_TestData : public GTestValueProvider
{
public:
	explicit GTestValueProvider_TestData(TestScriptContext * context) : GTestValueProvider(context) {}
	
	virtual string value(int id) const;
	virtual string equal(const string & a, const string & b) const;
};


class GTestValueProvider_String : public GTestValueProvider
{
public:
	explicit GTestValueProvider_String(TestScriptContext * context) : GTestValueProvider(context) {}
	
	virtual string value(int id) const;
	virtual string equal(const string & a, const string & b) const;
};


void bindCommonData(TestScriptContext * context);


} // namespace test_metadata_stl


using namespace test_metadata_stl;



#endif
