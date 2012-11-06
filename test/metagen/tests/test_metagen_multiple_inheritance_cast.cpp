#include "testmetagen.h"


namespace {


void metagenTest_MultipleInheritanceCast_PassRawPointerAsParam(TestScriptContext * context)
{
	QDO(d = mtest.getMultipleInheritanceCastD());
	QASSERT(mtest.checkMultipleInheritanceCastDAsR(d));

	QDO(r = mtest.getMultipleInheritanceCastDAsR());
	QASSERT(mtest.checkMultipleInheritanceCastRAsD(r));
}

#define CASE metagenTest_MultipleInheritanceCast_PassRawPointerAsParam
#include "do_testcase.h"


void metagenTest_MultipleInheritanceCast_PassSharedPointerAsParam(TestScriptContext * context)
{
	QDO(d = mtest.getMultipleInheritanceCastDSharedPointer());
	QASSERT(mtest.checkMultipleInheritanceCastDAsR(d));

	QDO(r = mtest.getMultipleInheritanceCastDAsRSharedPointer());
	QASSERT(mtest.checkMultipleInheritanceCastRAsD(r));
}

#define CASE metagenTest_MultipleInheritanceCast_PassSharedPointerAsParam
#include "do_testcase.h"


void metagenTest_MultipleInheritanceCast_SetRawPointerAsField(TestScriptContext * context)
{
	QNEWOBJ(data, mtest.MultipleInheritanceCastFieldData());

	QDO(d = mtest.getMultipleInheritanceCastD());
	QDO(data.rawR = d);
	QASSERT(data.checkR());

	QDO(r = mtest.getMultipleInheritanceCastDAsR());
	QDO(data.rawD = r);
	QASSERT(data.checkD());
}

#define CASE metagenTest_MultipleInheritanceCast_SetRawPointerAsField
#include "do_testcase.h"


void metagenTest_MultipleInheritanceCast_SetSharedPointerAsField(TestScriptContext * context)
{
	QNEWOBJ(data, mtest.MultipleInheritanceCastFieldData());

	QDO(d = mtest.getMultipleInheritanceCastDSharedPointer());
	QDO(data.rawR = d);
	QASSERT(data.checkR());

	QDO(r = mtest.getMultipleInheritanceCastDAsRSharedPointer());
	QDO(data.rawD = r);
	QASSERT(data.checkD());
}

#define CASE metagenTest_MultipleInheritanceCast_SetSharedPointerAsField
#include "do_testcase.h"


void metagenTest_MultipleInheritanceCast_Cast(TestScriptContext * context)
{
	QDO(cpgf._import("cpgf", "builtin.core"));

	QDO(r = mtest.getMultipleInheritanceCastDAsR());
	QERR(r.iAmD());

	QDO(d = cpgf.cast(r, mtest.MultipleInheritanceCastD));
	QASSERT(d.iAmD());

	QDO(d2 = cpgf.cast(r));
	QASSERT(d2.iAmD());
}

#define CASE metagenTest_MultipleInheritanceCast_Cast
#include "do_testcase.h"


GTEST(metagenTest_MultipleInheritanceCast_isInheritedFrom_Lib)
{
	const GMetaClass * metaClassR = findMetaClass("MultipleInheritanceCastR");
	const GMetaClass * metaClassA = findMetaClass("MultipleInheritanceCastA");
	const GMetaClass * metaClassB = findMetaClass("MultipleInheritanceCastB");
	const GMetaClass * metaClassC = findMetaClass("MultipleInheritanceCastC");
	const GMetaClass * metaClassD = findMetaClass("MultipleInheritanceCastD");

	GCHECK(metaClassD->isInheritedFrom(metaClassR));
	GCHECK(metaClassD->isInheritedFrom(metaClassA));
	GCHECK(metaClassD->isInheritedFrom(metaClassB));
	GCHECK(metaClassD->isInheritedFrom(metaClassC));

	GCHECK(metaClassB->isInheritedFrom(metaClassR));
	GCHECK(metaClassC->isInheritedFrom(metaClassR));
	GCHECK(metaClassB->isInheritedFrom(metaClassA));
	GCHECK(metaClassC->isInheritedFrom(metaClassA));
	
	GCHECK(metaClassA->isInheritedFrom(metaClassR));

	GCHECK(! metaClassR->isInheritedFrom(metaClassA));
	GCHECK(! metaClassR->isInheritedFrom(metaClassB));
	GCHECK(! metaClassR->isInheritedFrom(metaClassC));
	GCHECK(! metaClassR->isInheritedFrom(metaClassD));
}

GTEST(metagenTest_MultipleInheritanceCast_isInheritedFrom_API)
{
	GScopedInterface<IMetaService> service(createDefaultMetaService());
	
	GScopedInterface<IMetaClass> metaClassR(service->findClassByName("MultipleInheritanceCastR"));
	GScopedInterface<IMetaClass> metaClassA(service->findClassByName("MultipleInheritanceCastA"));
	GScopedInterface<IMetaClass> metaClassB(service->findClassByName("MultipleInheritanceCastB"));
	GScopedInterface<IMetaClass> metaClassC(service->findClassByName("MultipleInheritanceCastC"));
	GScopedInterface<IMetaClass> metaClassD(service->findClassByName("MultipleInheritanceCastD"));

	GCHECK(metaClassD->isInheritedFrom(metaClassR.get()));
	GCHECK(metaClassD->isInheritedFrom(metaClassA.get()));
	GCHECK(metaClassD->isInheritedFrom(metaClassB.get()));
	GCHECK(metaClassD->isInheritedFrom(metaClassC.get()));

	GCHECK(metaClassB->isInheritedFrom(metaClassR.get()));
	GCHECK(metaClassC->isInheritedFrom(metaClassR.get()));
	GCHECK(metaClassB->isInheritedFrom(metaClassA.get()));
	GCHECK(metaClassC->isInheritedFrom(metaClassA.get()));
	
	GCHECK(metaClassA->isInheritedFrom(metaClassR.get()));

	GCHECK(! metaClassR->isInheritedFrom(metaClassA.get()));
	GCHECK(! metaClassR->isInheritedFrom(metaClassB.get()));
	GCHECK(! metaClassR->isInheritedFrom(metaClassC.get()));
	GCHECK(! metaClassR->isInheritedFrom(metaClassD.get()));
}


}
