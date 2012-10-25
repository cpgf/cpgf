#include "test_misc_common.h"

#include "cpgf/gsharedptr.h"
#include "cpgf/metautility/gmetabytearray.h"
#include "cpgf/gvariant.h"

#include <vector>


using namespace cpgf;
using namespace std;


namespace Test_SharedPointer { namespace {

class TestData
{
public:
	explicit TestData(int * counter) : counter(counter) {
		++*counter;
	}

	virtual ~TestData() {
		--*counter;
	}

	virtual int getID() {
		return 0;
	}

private:
	int * counter;
};

class TestData2 : public TestData
{
public:
	explicit TestData2(int * counter) : TestData(counter) {
	}

	virtual int getID() {
		return 1;
	}

};

typedef GSharedPointer<TestData> SP;
typedef GSharedPointer<TestData2> SP2;

class TestObject
{
public:
	TestObject() {}

	explicit TestObject(int * counter) : data(new TestData(counter)) {
	}

	~TestObject() {
	}

private:
	SP data;
};


GTEST(TestSharedPointer_Simple)
{
	int count = 0;

	{
		SP sp(new TestData(&count));
		GEQUAL(1, count);
	}
	GEQUAL(0, count);

	{
		SP sp1(new TestData(&count));
		GEQUAL(1, count);

		SP sp2(new TestData(&count));
		GEQUAL(2, count);
		
		sp2 = sp1;
		GEQUAL(1, count);
	}
	GEQUAL(0, count);

	{
		SP sp1(new TestData(&count));
		GEQUAL(1, count);

		SP sp2 = sp1;
		GEQUAL(1, count);
		
		SP sp3;
		GEQUAL(1, count);
		
		sp3 = sp2;
		GEQUAL(1, count);
		
		sp1 = sp3;
		GEQUAL(1, count);
	}
	GEQUAL(0, count);
}


GTEST(TestSharedPointer_Object)
{
	int count = 0;

	{
		TestObject obj(&count);
		GEQUAL(1, count);
	}
	GEQUAL(0, count);

	{
		TestObject obj1(&count);
		GEQUAL(1, count);

		TestObject obj2(&count);
		GEQUAL(2, count);
		
		obj1 = obj2;
		GEQUAL(1, count);
	}
	GEQUAL(0, count);

	{
		TestObject obj1(&count);
		GEQUAL(1, count);

		TestObject obj2 = obj1;
		GEQUAL(1, count);
		
		TestObject obj3;
		GEQUAL(1, count);
		
		obj3 = obj2;
		GEQUAL(1, count);
		
		obj1 = obj3;
		GEQUAL(1, count);
	}
	GEQUAL(0, count);
}


GTEST(TestSharedPointer_DataInVector)
{
	const int iteral = 100;

	int count = 0;

	{
		vector<SP> v;

		for(int i = 0; i <iteral; ++i) {
			SP sp(new TestData(&count));
			v.push_back(sp);
		}
		GEQUAL(iteral, count);
    }
	GEQUAL(0, count);
}


GTEST(TestSharedPointer_DifferentSP)
{
	int count = 0;

	{
		SP2 sp2(new TestData2(&count));
		GEQUAL(1, count);

		SP sp(sp2);
		GEQUAL(1, count);
	}
	GEQUAL(0, count);

	{
		SP sp(new TestData2(&count));
		GEQUAL(1, count);

		SP2 sp2(sharedStaticCast<TestData2>(sp));
		GEQUAL(1, count);
	}
	GEQUAL(0, count);
}



} }

