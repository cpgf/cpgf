#include "test_reflection_common.h"

using namespace std;
using namespace cpgf;

#define PTREQUAL(pa, pb) GEQUAL((void *)pa, (void *)pb)
#define PTRDIFF(pa, pb) GDIFF((void *)pa, (void *)pb)


namespace Test_Cast { namespace {

class A
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
	define
		._class(GDefineMetaClass<A>::declare("A"))
		._class(GDefineMetaClass<B, A>::declare("B"))
		._class(GDefineMetaClass<C, A>::declare("C"))
		._class(GDefineMetaClass<D, B, C>::declare("D"))
	;
}


// not really test, but to ensure we can have different pointers on same object
GTEST(testCastForLayout)
{
	D d;
	D * pd = &d;
	C * pc = pd;
	B * pb = pd;
	A * pa = pd;

	PTRDIFF(pa, pd);
	PTRDIFF(pb, pd);
	PTRDIFF(pc, pd);
}

GTEST(testCastBwteenBase)
{
	GDefineMetaNamespace ns = GDefineMetaNamespace::declare("ns");
	reflectCast(ns);
	const GMetaClass * nsClass = ns.getMetaClass();
//	const GMetaClass * classA = nsClass->getClass("A");
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

GTEST(testCastBwteenDerived)
{
	GDefineMetaNamespace ns = GDefineMetaNamespace::declare("ns");
	reflectCast(ns);
	const GMetaClass * nsClass = ns.getMetaClass();
	const GMetaClass * classA = nsClass->getClass("A");
	const GMetaClass * classB = nsClass->getClass("B");
	const GMetaClass * classC = nsClass->getClass("C");
//	const GMetaClass * classD = nsClass->getClass("D");

	D d;
	D * pd = &d;
	A * pa = pd;

	void * p;

	p = pa;
	p = classA->castToDerived(p, 0); // B
	PTREQUAL(p, dynamic_cast<B *>(pa));
	p = classB->castToDerived(p, 0); // D
	PTREQUAL(p, pd);
}




} }
