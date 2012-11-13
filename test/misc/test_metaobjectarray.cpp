#include "test_misc_common.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/gmetaapiutil.h"
#include "cpgf/metautility/gmetaobjectarray.h"
#include "cpgf/gscopedinterface.h"


using namespace std;
using namespace cpgf;

namespace Test_MetaObjectArray { namespace {

const int MagicValue = 0x1cbadfed;

int defaultCreatedCount = 0;
int valueCreatedCount = 0;
int copiedCount = 0;
int freedCount = 0;

void resetCount()
{
	defaultCreatedCount = 0;
	valueCreatedCount = 0;
	copiedCount = 0;
	freedCount = 0;
}

class TestItem
{
public:
	TestItem()
		: n(0), s(), magic(MagicValue), isDefaultConstructed(true)
	{
		++defaultCreatedCount;
	}
	
	TestItem(int n, string s)
		: n(n), s(s), magic(MagicValue), isDefaultConstructed(false)
	{
		++valueCreatedCount;
	}

	TestItem(const TestItem & other)
		: n(other.n), s(other.s), magic(other.magic), isDefaultConstructed(other.isDefaultConstructed)
	{
		++copiedCount;
	}

	~TestItem() {
		++freedCount;
	}
	
	bool isItem() const {
		return this->magic == MagicValue;
	}

	bool isDefault() const {
		return this->isItem() && this->isDefaultConstructed && this->n == 0 && this->s.length() == 0;
	}
	
	bool check(int n, string s) const {
		return this->isItem() && this->n == n && this->s == s;
	}

public:
	int n;
	string s;
	
private:
	int magic;
	bool isDefaultConstructed;
};


IMetaClass * createMetaObjectArrayMetaClass()
{
	GDefineMetaClass<TestItem> d = GDefineMetaClass<TestItem>::declare("TestItem");

	d._field("n", &TestItem::n);
	d._field("s", &TestItem::s);
	
	return static_cast<IMetaClass *>(metaItemToInterface(d.takeMetaClass(), true));
}


GTEST(Test_MetaObjectArray_getCount)
{
	GScopedInterface<IMetaClass> metaClass(createMetaObjectArrayMetaClass());

	GMetaObjectArray oa(metaClass.get());
	
	GEQUAL(0, oa.getCount());

	oa.setItem(2, TestItem());
	GEQUAL(3, oa.getCount());

	oa.setItem(0, TestItem());
	GEQUAL(3, oa.getCount());
}


GTEST(Test_MetaObjectArray_defaultConstruct)
{
	GScopedInterface<IMetaClass> metaClass(createMetaObjectArrayMetaClass());

	GMetaObjectArray oa(metaClass.get());
	
	oa.setItem(3, TestItem());
	GCHECK(static_cast<TestItem *>(objectAddressFromVariant(oa.getItem(0)))->isDefault());
	GCHECK(static_cast<TestItem *>(objectAddressFromVariant(oa.getItem(1)))->isDefault());
	GCHECK(static_cast<TestItem *>(objectAddressFromVariant(oa.getItem(2)))->isDefault());
}


GTEST(Test_MetaObjectArray_getItem)
{
	GScopedInterface<IMetaClass> metaClass(createMetaObjectArrayMetaClass());

	TestItem * item;

	GMetaObjectArray oa(metaClass.get());
	oa.setItem(0, TestItem(38, "abc"));
	oa.setItem(1, TestItem(98, "def"));
	oa.setItem(2, TestItem(1999, "what"));
	
	item = static_cast<TestItem *>(objectAddressFromVariant(oa.getItem(0)));
	GCHECK(item->check(38, "abc"));
	item = static_cast<TestItem *>(objectAddressFromVariant(oa.getItem(1)));
	GCHECK(item->check(98, "def"));
	item = static_cast<TestItem *>(objectAddressFromVariant(oa.getItem(2)));
	GCHECK(item->check(1999, "what"));
}


GTEST(Test_MetaObjectArray_getLotsOfItem)
{
	GScopedInterface<IMetaClass> metaClass(createMetaObjectArrayMetaClass());

	TestItem * item;

	GMetaObjectArray oa(metaClass.get());
	const int COUNT = 1000;

	for(int i = 0; i < COUNT; ++i) {
		oa.setItem(i, TestItem(i, ""));
	}
	
	for(int i = 0; i < COUNT; ++i) {
		item = static_cast<TestItem *>(objectAddressFromVariant(oa.getItem(i)));
		GCHECK(item->check(i, ""));
	}
}


GTEST(Test_MetaObjectArray_countTrack)
{
	GScopedInterface<IMetaClass> metaClass(createMetaObjectArrayMetaClass());
	TestItem item;

	{
		resetCount();
		GMetaObjectArray oa(metaClass.get());
		oa.setItem(0, item);

		GEQUAL(1, defaultCreatedCount);
		GEQUAL(1, copiedCount);
	}
	GEQUAL(2, freedCount);

	{
		resetCount();
		GMetaObjectArray oa(metaClass.get());
		oa.setItem(3, item);

		GEQUAL(4, defaultCreatedCount);
		GEQUAL(1, copiedCount);
	}
	GEQUAL(5, freedCount);
}




} }

