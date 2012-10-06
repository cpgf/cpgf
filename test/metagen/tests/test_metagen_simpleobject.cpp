#include "testmetagen.h"


namespace {


void testSimpleData(TestScriptContext * context)
{
	QNEWOBJ(a, mtest.SimpleData());
	QDO(a.n = 38);
	QDO(a.s = "abc");
	// not support casting char * to std::wstring yet
//	QDO(a.ws = "wide");
	
	QNEWOBJ(b, mtest.SimpleData(a));
	QASSERT(b.n == 38);
	QASSERT(b.s == "abc");
	// not support casting char * to std::wstring yet
//	QASSERT(b.ws == "wide");
}


#define CASE testSimpleData
#include "do_testcase.h"


void testSimpleObject_Data(TestScriptContext * context)
{
	QNEWOBJ(a, mtest.SimpleObject());
	QDO(a.data.n = 38);
	QDO(a.data.s = "abc");
	QDO(a.data.atom.value = 58);
	QASSERT(a.checkData(38));
	QASSERT(a.checkData("abc"));
	QASSERT(a.checkAtom(58));
}


#define CASE testSimpleObject_Data
#include "do_testcase.h"



void testSimpleObject_GetData(TestScriptContext * context)
{
	QNEWOBJ(a, mtest.SimpleObject());
	QDO(a.getData().n = 78);
	QDO(a.getData().s = "def");
	QDO(a.getData().atom.value = 98);
	QASSERT(a.checkData(78));
	QASSERT(a.checkData("def"));
	QASSERT(a.checkAtom(98));
}

#define CASE testSimpleObject_GetData
#include "do_testcase.h"


void testSimpleObject_SetData(TestScriptContext * context)
{
	QNEWOBJ(a, mtest.SimpleObject());
	QNEWOBJ(d, mtest.SimpleData(1999, "what"));
	QDO(d.atom.value = 108);
	QDO(a.setData(d));
	QASSERT(a.checkData(1999));
	QASSERT(a.checkData("what"));
	QASSERT(a.checkAtom(108));
}


#define CASE testSimpleObject_SetData
#include "do_testcase.h"


void testSimpleObject_metaClass(TestScriptContext * context)
{
	QASSERT(mtest.metaClassIsSimpleObject(mtest.SimpleObject));
	QASSERT(mtest.metaClassIsSimpleObject(mtest.getSimpleObjectClass(mtest.SimpleObject)));
	QDO(cls = mtest.getSimpleObjectClass(mtest.SimpleObject));
	QNEWOBJ(a, cls());
}


#define CASE testSimpleObject_metaClass
#include "do_testcase.h"


void testSimpleObject_Data_GScopedPointer(TestScriptContext * context)
{
	QNEWOBJ(a, mtest.createSharedSimpleObject());
	QDO(a.data.n = 38);
	QDO(a.data.s = "abc");
	QDO(a.data.atom.value = 58);
	QASSERT(a.checkData(38));
	QASSERT(a.checkData("abc"));
	QASSERT(a.checkAtom(58));
}


#define CASE testSimpleObject_Data_GScopedPointer
#include "do_testcase.h"


void testSimpleObject_Data_GScopedPointerGetPointer(TestScriptContext * context)
{
	QNEWOBJ(b, mtest.createSharedSimpleObject());
	QDO(a = mtest.getSharedSimpleObject(b));
	QDO(a.data.n = 38);
	QDO(a.data.s = "abc");
	QDO(a.data.atom.value = 58);
	QASSERT(a.checkData(38));
	QASSERT(a.checkData("abc"));
	QASSERT(a.checkAtom(58));
}


#define CASE testSimpleObject_Data_GScopedPointerGetPointer
#include "do_testcase.h"


void testSimpleObject_Data_GScopedPointerGetN(TestScriptContext * context)
{
	QNEWOBJ(a, mtest.createSharedSimpleObject());
	QDO(a.data.n = 98);
	QASSERT(mtest.getSimpleObjectPointerN(a) == 98);
	QASSERT(mtest.getSimpleObjectN(a) == 98);
}


#define CASE testSimpleObject_Data_GScopedPointerGetN
#include "do_testcase.h"




}
