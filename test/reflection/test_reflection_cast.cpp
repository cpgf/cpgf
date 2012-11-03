#include "test_reflection_common.h"

using namespace std;
using namespace cpgf;

#define PTREQUAL(pa, pb) GEQUAL((void *)pa, (void *)pb)
#define PTRDIFF(pa, pb) GDIFF((void *)pa, (void *)pb)


namespace Test_Cast { namespace {

class R
{
public:
	virtual ~R() {}
	int r[20];
};

class A : public R
{
public:
	virtual ~A() {}

	int a;
};

class B : virtual public A
{
public:
	virtual ~B() {}
	virtual void c1() {}

	double c;
};

class C : virtual public A
{
public:
	virtual ~C() {}

	long long d[20];
};

class D : virtual public B, virtual public C
{
public:
	virtual ~D() {}

	int e[10];
};

template <typename Meta>
void reflectCast(Meta define)
{
	define._class(GDefineMetaClass<R>::declare("R"));
	define._class(GDefineMetaClass<A, R>::declare("A"));
	define._class(GDefineMetaClass<B, A>::declare("B"));
	define._class(GDefineMetaClass<C, A>::declare("C"));
	define._class(GDefineMetaClass<D, B, C>::declare("D"));
}


// not really test, but to ensure we can have different pointers on same object
GTEST(testCastForLayout)
{
	GCHECK(IsPolymorphic<A>::Result);
	GCHECK(IsPolymorphic<B>::Result);
	GCHECK(IsPolymorphic<C>::Result);
	GCHECK(IsPolymorphic<D>::Result);

	D d;
	D * pd = &d;
	C * pc = pd;
	B * pb = pd;
	A * pa = pd;

	PTRDIFF(pa, pd);
	PTRDIFF(pb, pd);
	PTRDIFF(pc, pd);
}

GTEST(testCastBetweenBaseLib)
{
	GDefineMetaNamespace ns = GDefineMetaNamespace::declare("ns");
	reflectCast(ns);
	
	const GMetaClass * nsClass = ns.getMetaClass();
	const GMetaClass * classB = nsClass->getClass("B");
	const GMetaClass * classC = nsClass->getClass("C");
	const GMetaClass * classD = nsClass->getClass("D");

	D d;
	D * pd = &d;
	A * pa = pd;

	void * p;

	p = pd;
	p = classD->castToBase(p, 0); // B
	PTREQUAL(p, (B *)pd);
	p = classB->castToBase(p, 0); // A
	PTREQUAL(p, pa);

	p = pd;
	p = classD->castToBase(p, 1); // C
	PTREQUAL(p, (C *)pd);
	p = classC->castToBase(p, 0); // A
	PTREQUAL(p, pa);

	p = pa;
	p = classB->castFromBase(p, 0); // B
	PTREQUAL(p, dynamic_cast<B *>(pa));
	p = classD->castFromBase(p, 0); // D
	PTREQUAL(p, pd);

	p = pa;
	p = classC->castFromBase(p, 0); // C
	PTREQUAL(p, dynamic_cast<C *>(pa));
	p = classD->castFromBase(p, 1); // D
	PTREQUAL(p, pd);
}

GTEST(testCastBetweenBaseLibApi)
{
	GDefineMetaNamespace ns = GDefineMetaNamespace::declare("ns");
	reflectCast(ns);
	
	GScopedInterface<IMetaClass> nsClass(static_cast<IMetaClass *>(metaItemToInterface(ns.getMetaClass())));
	GScopedInterface<IMetaClass> classB(nsClass->getClass("B"));
	GScopedInterface<IMetaClass> classC(nsClass->getClass("C"));
	GScopedInterface<IMetaClass> classD(nsClass->getClass("D"));

	D d;
	D * pd = &d;
	A * pa = pd;

	void * p;

	p = pd;
	p = classD->castToBase(p, 0); // B
	PTREQUAL(p, (B *)pd);
	p = classB->castToBase(p, 0); // A
	PTREQUAL(p, pa);

	p = pd;
	p = classD->castToBase(p, 1); // C
	PTREQUAL(p, (C *)pd);
	p = classC->castToBase(p, 0); // A
	PTREQUAL(p, pa);

	p = pa;
	p = classB->castFromBase(p, 0); // B
	PTREQUAL(p, dynamic_cast<B *>(pa));
	p = classD->castFromBase(p, 0); // D
	PTREQUAL(p, pd);

	p = pa;
	p = classC->castFromBase(p, 0); // C
	PTREQUAL(p, dynamic_cast<C *>(pa));
	p = classD->castFromBase(p, 1); // D
	PTREQUAL(p, pd);
}

GTEST(testCastBetweenDerivedLib)
{
	GDefineMetaNamespace ns = GDefineMetaNamespace::declare("ns");
	reflectCast(ns);
	
	const GMetaClass * nsClass = ns.getMetaClass();
	const GMetaClass * classA = nsClass->getClass("A");
	const GMetaClass * classB = nsClass->getClass("B");
	const GMetaClass * classC = nsClass->getClass("C");

	D d;
	D * pd = &d;
	A * pa = pd;

	void * p;

	p = pa;
	p = classA->castToDerived(p, 0); // B
	PTREQUAL(p, dynamic_cast<B *>(pa));
	p = classB->castToDerived(p, 0); // D
	PTREQUAL(p, pd);

	p = pa;
	p = classA->castToDerived(p, 1); // C
	PTREQUAL(p, dynamic_cast<C *>(pa));
	p = classC->castToDerived(p, 0); // D
	PTREQUAL(p, pd);

	p = pd;
	p = classC->castFromDerived(p, 0); // C
	PTREQUAL(p, dynamic_cast<C *>(pa));
	p = classA->castFromDerived(p, 1); // A
	PTREQUAL(p, pa);

	p = pd;
	p = classB->castFromDerived(p, 0); // B
	PTREQUAL(p, dynamic_cast<B *>(pa));
	p = classA->castFromDerived(p, 0); // A
	PTREQUAL(p, pa);
}

GTEST(testCastBetweenDerivedApi)
{
	GDefineMetaNamespace ns = GDefineMetaNamespace::declare("ns");
	reflectCast(ns);
	
	GScopedInterface<IMetaClass> nsClass(static_cast<IMetaClass *>(metaItemToInterface(ns.getMetaClass())));
	GScopedInterface<IMetaClass> classA(nsClass->getClass("A"));
	GScopedInterface<IMetaClass> classB(nsClass->getClass("B"));
	GScopedInterface<IMetaClass> classC(nsClass->getClass("C"));

	D d;
	D * pd = &d;
	A * pa = pd;

	void * p;

	p = pa;
	p = classA->castToDerived(p, 0); // B
	PTREQUAL(p, dynamic_cast<B *>(pa));
	p = classB->castToDerived(p, 0); // D
	PTREQUAL(p, pd);

	p = pa;
	p = classA->castToDerived(p, 1); // C
	PTREQUAL(p, dynamic_cast<C *>(pa));
	p = classC->castToDerived(p, 0); // D
	PTREQUAL(p, pd);

	p = pd;
	p = classC->castFromDerived(p, 0); // C
	PTREQUAL(p, dynamic_cast<C *>(pa));
	p = classA->castFromDerived(p, 1); // A
	PTREQUAL(p, pa);

	p = pd;
	p = classB->castFromDerived(p, 0); // B
	PTREQUAL(p, dynamic_cast<B *>(pa));
	p = classA->castFromDerived(p, 0); // A
	PTREQUAL(p, pa);
}

GTEST(testFindAppropriateDerivedClassLib)
{
	GDefineMetaNamespace ns = GDefineMetaNamespace::declare("ns");
	reflectCast(ns);
	
	const GMetaClass * nsClass = ns.getMetaClass();
	const GMetaClass * classR = nsClass->getClass("R");
	const GMetaClass * classA = nsClass->getClass("A");
	const GMetaClass * classD = nsClass->getClass("D");
	const GMetaClass * resultClass;

	R r;
	A a;
	D d;

	R * pr = &r;
	A * pa = &a;
	D * pd = &d;

	void * p;
	void * result;

	p = pr;
	resultClass = findAppropriateDerivedClass(p, classR, &result);
	GCHECK(resultClass->equals(classR));
	PTREQUAL(result, pr);

	p = pa;
	resultClass = findAppropriateDerivedClass(p, classA, &result);
	GCHECK(resultClass->equals(classA));
	PTREQUAL(result, pa);

	p = (A *)pd; // must cast to A * first, otherwise, p will point to D directly, not point to A
	resultClass = findAppropriateDerivedClass(p, classA, &result);
	GCHECK(resultClass->equals(classD));
	PTREQUAL(result, pd);
}

GTEST(testFindAppropriateDerivedClassApi)
{
	GDefineMetaNamespace ns = GDefineMetaNamespace::declare("ns");
	reflectCast(ns);
	
	GScopedInterface<IMetaClass> nsClass(static_cast<IMetaClass *>(metaItemToInterface(ns.getMetaClass())));
	GScopedInterface<IMetaClass> classR(nsClass->getClass("R"));
	GScopedInterface<IMetaClass> classA(nsClass->getClass("A"));
	GScopedInterface<IMetaClass> classB(nsClass->getClass("B"));
	GScopedInterface<IMetaClass> classC(nsClass->getClass("C"));
	GScopedInterface<IMetaClass> classD(nsClass->getClass("D"));
	GScopedInterface<IMetaClass> resultClass;

	R r;
	A a;
	D d;

	R * pr = &r;
	A * pa = &a;
	D * pd = &d;

	void * p;
	void * result;

	p = pr;
	resultClass.reset(findAppropriateDerivedClass(p, classR.get(), &result));
	GCHECK(resultClass->equals(classR.get()));
	PTREQUAL(result, pr);

	p = pa;
	resultClass.reset(findAppropriateDerivedClass(p, classA.get(), &result));
	GCHECK(resultClass->equals(classA.get()));
	PTREQUAL(result, pa);

	p = (A *)pd; // must cast to A * first, otherwise, p will point to D directly, not point to A
	resultClass.reset(findAppropriateDerivedClass(p, classA.get(), &result));
	GCHECK(resultClass->equals(classD.get()));
	PTREQUAL(result, pd);
}

GTEST(testMetaCastToBase)
{
	GDefineMetaNamespace ns = GDefineMetaNamespace::declare("ns");
	reflectCast(ns);
	
	GScopedInterface<IMetaClass> nsClass(static_cast<IMetaClass *>(metaItemToInterface(ns.getMetaClass())));
	GScopedInterface<IMetaClass> classR(nsClass->getClass("R"));
	GScopedInterface<IMetaClass> classA(nsClass->getClass("A"));
	GScopedInterface<IMetaClass> classB(nsClass->getClass("B"));
	GScopedInterface<IMetaClass> classC(nsClass->getClass("C"));
	GScopedInterface<IMetaClass> classD(nsClass->getClass("D"));

	D d;

	R * pr = &d;
	A * pa = &d;
	B * pb = &d;
	C * pc = &d;
	D * pd = &d;

	void * result;

	result = metaCastToBase(pd, classD.get(), classR.get());
	PTREQUAL(result, pr);

	result = metaCastToBase(pd, classD.get(), classA.get());
	PTREQUAL(result, pa);

	result = metaCastToBase(pd, classD.get(), classB.get());
	PTREQUAL(result, pb);

	result = metaCastToBase(pd, classD.get(), classC.get());
	PTREQUAL(result, pc);
}

GTEST(testMetaCastToDerived)
{
	GDefineMetaNamespace ns = GDefineMetaNamespace::declare("ns");
	reflectCast(ns);
	
	GScopedInterface<IMetaClass> nsClass(static_cast<IMetaClass *>(metaItemToInterface(ns.getMetaClass())));
	GScopedInterface<IMetaClass> classR(nsClass->getClass("R"));
	GScopedInterface<IMetaClass> classA(nsClass->getClass("A"));
	GScopedInterface<IMetaClass> classB(nsClass->getClass("B"));
	GScopedInterface<IMetaClass> classC(nsClass->getClass("C"));
	GScopedInterface<IMetaClass> classD(nsClass->getClass("D"));

	D d;

	R * pr = &d;
	A * pa = &d;
	B * pb = &d;
	C * pc = &d;
	D * pd = &d;

	void * result;

	result = metaCastToDerived(pr, classR.get(), classA.get());
	PTREQUAL(result, pa);

	result = metaCastToDerived(pr, classR.get(), classB.get());
	PTREQUAL(result, pb);

	result = metaCastToDerived(pr, classR.get(), classC.get());
	PTREQUAL(result, pc);

	result = metaCastToDerived(pr, classR.get(), classD.get());
	PTREQUAL(result, pd);
}



} }

