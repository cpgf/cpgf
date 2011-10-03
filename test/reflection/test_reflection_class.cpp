#include "test_reflection_common.h"

using namespace std;
using namespace cpgf;

namespace Test_Class { namespace {

bool itemEuqal(IMetaItem * a, IMetaItem * b)
{
	bool equal = a->equals(b);
	
	a->releaseReference();
	b->releaseReference();
	
	return equal;
}

class ClassBase {
public:

};


class ClassAbstract {
public:
	virtual void myAbstract() = 0;

};


GMETA_DEFINE_CLASS(ClassBase, ClassBase, NAME(ClassBase)) {
}

GMETA_DEFINE_CLASS(ClassAbstract, ClassAbstract, NAME(ClassAbstract)) {
	GMETA_METHOD(myAbstract);
}


GTEST(Lib_BasicInfo)
{
	const GMetaClass * metaClass;

	metaClass = findMetaClass(NAME(ClassBase));
	GCHECK(metaClass);

	GCHECK(! metaClass->isGlobal());
	GCHECK(metaClass->canCreateInstance());
	GCHECK(metaClass->canCopyInstance());
	GCHECK(metaClass->isSameClass(findMetaClass(NAME(ClassBase))));
	GCHECK(! metaClass->getBaseClass(0));
	GEQUAL(metaClass->getBaseCount(), 0);
	GCHECK(! metaClass->isInheritedFrom(NULL));
	GCHECK(! metaClass->isInheritedFrom(findMetaClass(NAME(ClassAbstract))));
	
	metaClass = findMetaClass(NAME(ClassAbstract));
	GCHECK(metaClass);

	GCHECK(metaClass->isAbstract());
	GCHECK(! metaClass->canCreateInstance());
	GCHECK(! metaClass->canCopyInstance());
	GCHECK(metaClass->isSameClass(findMetaClass(NAME(ClassAbstract))));
	GCHECK(! metaClass->getBaseClass(0));
	GEQUAL(metaClass->getBaseCount(), 0);
	GCHECK(! metaClass->isInheritedFrom(NULL));
	GCHECK(! metaClass->isInheritedFrom(findMetaClass(NAME(ClassBase))));

}



GTEST(API_BasicInfo)
{
	GApiScopedPointer<IMetaService> service(createMetaService());
	GCHECK(service);

	GApiScopedPointer<IMetaClass> metaClass;
	metaClass.reset(service->findClassByName(NAME(ClassBase)));
	GCHECK(metaClass);
	GApiScopedPointer<IMetaClass> metaClass2;

	GCHECK(! metaClass->isGlobal());
	GCHECK(metaClass->canCreateInstance());
	GCHECK(metaClass->canCopyInstance());
	metaClass2.reset((service->findClassByName(NAME(ClassBase))));
	GCHECK(metaClass->equals(metaClass2.get()));
	GCHECK(! metaClass->getBaseClass(0));
	GEQUAL(metaClass->getBaseCount(), 0);
	GCHECK(! metaClass->isInheritedFrom(NULL));
	metaClass2.reset((service->findClassByName(NAME(ClassAbstract))));
	GCHECK(! metaClass->isInheritedFrom(metaClass2.get()));
	
	metaClass.reset(service->findClassByName(NAME(ClassAbstract)));
	GCHECK(metaClass);

	GCHECK(metaClass->isAbstract());
	GCHECK(! metaClass->canCreateInstance());
	GCHECK(! metaClass->canCopyInstance());
	metaClass2.reset((service->findClassByName(NAME(ClassAbstract))));
	GCHECK(metaClass->equals(metaClass2.get()));
	GCHECK(! metaClass->getBaseClass(0));
	GEQUAL(metaClass->getBaseCount(), 0);
	metaClass2.reset((service->findClassByName(NAME(ClassBase))));
	GCHECK(! metaClass->isInheritedFrom(metaClass2.get()));

}



#define CLASS Class_CreateIntance
class CLASS {
public:
	CLASS() : n(5), s("abc") {
	}
	
	CLASS(const CLASS & other) : n(other.n), s(other.s) {
	}
	
	bool operator == (const CLASS & other) const {
		return this->n == other.n;
		return this->s == other.s;
	}
	
	bool check() const {
		return this->n == 5 && this->s == "abc";
	}
	
public:
	int n;
	string s;
};

GMETA_DEFINE_CLASS(CLASS, CLASS, NAME(CLASS)) {
}


GTEST(Lib_CreateInstance)
{
	const GMetaClass * metaClass;

	metaClass = findMetaClass(NAME(CLASS));
	GCHECK(metaClass);
	GEQUAL(metaClass->getTypeSize(), sizeof(CLASS));
	
	CLASS * pobj;
	char inplace[1000];
	
	pobj = (CLASS *)(metaClass->createInstance());
	GCHECK(pobj);
	GCHECK(pobj->check());
	metaClass->destroyInstance(pobj);
	
	pobj = (CLASS *)(metaClass->createInplace(inplace));
	GCHECK(pobj);
	GCHECK(pobj->check());
	pobj->~CLASS();
	
	CLASS clone;
	clone.n = 100;
	clone.s = "notabc";

	pobj = (CLASS *)(metaClass->cloneInstance(&clone));
	GCHECK(pobj);
	GCHECK(! pobj->check());
	GEQUAL(*pobj, clone);
	GEQUAL(pobj->n, 100);
	metaClass->destroyInstance(pobj);

	clone.n = 199;
	pobj = (CLASS *)(metaClass->cloneInplace(&clone, inplace));
	GCHECK(pobj);
	GCHECK(! pobj->check());
	GEQUAL(*pobj, clone);
	GEQUAL(pobj->n, 199);
	pobj->~CLASS();
}

GTEST(API_CreateInstance)
{
	GApiScopedPointer<IMetaService> service(createMetaService());
	GCHECK(service);

	GApiScopedPointer<IMetaClass> metaClass(service->findClassByName(NAME(CLASS)));
	GCHECK(metaClass);
	GEQUAL(metaClass->getTypeSize(), sizeof(CLASS));
	
	CLASS * pobj;
	char inplace[1000];
	
	pobj = (CLASS *)(metaClass->createInstance());
	GCHECK(pobj);
	GCHECK(pobj->check());
	metaClass->destroyInstance(pobj);
	
	pobj = (CLASS *)(metaClass->createInplace(inplace));
	GCHECK(pobj);
	GCHECK(pobj->check());
	pobj->~CLASS();
	
	CLASS clone;
	clone.n = 100;
	clone.s = "notabc";

	pobj = (CLASS *)(metaClass->cloneInstance(&clone));
	GCHECK(pobj);
	GCHECK(! pobj->check());
	GEQUAL(*pobj, clone);
	GEQUAL(pobj->n, 100);
	metaClass->destroyInstance(pobj);

	clone.n = 199;
	pobj = (CLASS *)(metaClass->cloneInplace(&clone, inplace));
	GCHECK(pobj);
	GCHECK(! pobj->check());
	GEQUAL(*pobj, clone);
	GEQUAL(pobj->n, 199);
	pobj->~CLASS();
}
#undef CLASS




#define CLASS Class_GetConstructor
class CLASS {
public:
	CLASS() {}
	CLASS(int) {}
	CLASS(string) {}
	CLASS(int, string) {}
	CLASS(int, int, int) {}

	char data[5];
};


GMETA_DEFINE_CLASS(CLASS, CLASS, NAME(CLASS)) {
	reflectConstructor<void * ()>();
	reflectConstructor<void * (int)>();
	reflectConstructor<void * (string)>();
	reflectConstructor<void * (int, string)>();
	reflectConstructor<void * (int, int, int)>();
}

GTEST(Lib_GetConstructor)
{
	const GMetaClass * metaClass;

	metaClass = findMetaClass(NAME(CLASS));
	GCHECK(metaClass);
	GEQUAL(metaClass->getTypeSize(), sizeof(CLASS));
	
	GEQUAL(metaClass->getConstructorCount(), 5);
	GEQUAL_NOTNULL(metaClass->getConstructorByParamCount(0), metaClass->getConstructorAt(0));
	GEQUAL_NOTNULL(metaClass->getConstructorByParamCount(1), metaClass->getConstructorAt(1));
	GEQUAL_NOTNULL(metaClass->getConstructorByParamCount(2), metaClass->getConstructorAt(3));
	GEQUAL_NOTNULL(metaClass->getConstructorByParamCount(3), metaClass->getConstructorAt(4));
	GEQUAL(metaClass->getConstructorByParamCount(50), NULL);
}

GTEST(API_GetConstructor)
{
	GApiScopedPointer<IMetaService> service(createMetaService());
	GCHECK(service);

	GApiScopedPointer<IMetaClass> metaClass(service->findClassByName(NAME(CLASS)));
	GCHECK(metaClass);
	GEQUAL(metaClass->getTypeSize(), sizeof(CLASS));
	
	GEQUAL(metaClass->getConstructorCount(), 5);
	GCHECK(itemEuqal(metaClass->getConstructorByParamCount(0), metaClass->getConstructorAt(0)));
	GCHECK(itemEuqal(metaClass->getConstructorByParamCount(1), metaClass->getConstructorAt(1)));
	GCHECK(itemEuqal(metaClass->getConstructorByParamCount(2), metaClass->getConstructorAt(3)));
	GCHECK(itemEuqal(metaClass->getConstructorByParamCount(3), metaClass->getConstructorAt(4)));
	GEQUAL(metaClass->getConstructorByParamCount(50), NULL);
}
#undef CLASS


#define CLASS Class_GetField
class CLASS {
public:
	int f0;
	int f1;
	int f2;
	
	char data[60];
};


GMETA_DEFINE_CLASS(CLASS, CLASS, NAME(CLASS)) {
	GMETA_FIELD(f0);
	GMETA_FIELD(f1);
	GMETA_FIELD(f2);
}

GTEST(Lib_GetField)
{
	const GMetaClass * metaClass;

	metaClass = findMetaClass(NAME(CLASS));
	GCHECK(metaClass);
	GEQUAL(metaClass->getTypeSize(), sizeof(CLASS));
	
	GEQUAL(metaClass->getFieldCount(), 3);
	GEQUAL_NOTNULL(metaClass->getField("f0"), metaClass->getFieldAt(0));
	GEQUAL_NOTNULL(metaClass->getField("f1"), metaClass->getFieldAt(1));
	GEQUAL_NOTNULL(metaClass->getField("f2"), metaClass->getFieldAt(2));
	GEQUAL(metaClass->getField("none"), NULL);
}

GTEST(API_GetField)
{
	GApiScopedPointer<IMetaService> service(createMetaService());
	GCHECK(service);

	GApiScopedPointer<IMetaClass> metaClass(service->findClassByName(NAME(CLASS)));
	GCHECK(metaClass);
	GEQUAL(metaClass->getTypeSize(), sizeof(CLASS));
	
	GEQUAL(metaClass->getFieldCount(), 3);
	GCHECK(itemEuqal(metaClass->getField("f0"), metaClass->getFieldAt(0)));
	GCHECK(itemEuqal(metaClass->getField("f1"), metaClass->getFieldAt(1)));
	GCHECK(itemEuqal(metaClass->getField("f2"), metaClass->getFieldAt(2)));
	GEQUAL(metaClass->getField("none"), NULL);
}
#undef CLASS



#define CLASS Class_GetProperty
class CLASS {
public:
	int p0;
	int p1;
	int p2;
	int p3;
	
	char data[15];
};


GMETA_DEFINE_CLASS(CLASS, CLASS, NAME(CLASS)) {
	GMETA_PROPERTY(p0, p0, p0);
	GMETA_PROPERTY(p1, p1, p1);
	GMETA_PROPERTY(p2, p2, p2);
	GMETA_PROPERTY(p3, p3, p3);
}

GTEST(Lib_GetProperty)
{
	const GMetaClass * metaClass;

	metaClass = findMetaClass(NAME(CLASS));
	GCHECK(metaClass);
	GEQUAL(metaClass->getTypeSize(), sizeof(CLASS));
	
	GEQUAL(metaClass->getPropertyCount(), 4);
	GEQUAL_NOTNULL(metaClass->getProperty("p0"), metaClass->getPropertyAt(0));
	GEQUAL_NOTNULL(metaClass->getProperty("p1"), metaClass->getPropertyAt(1));
	GEQUAL_NOTNULL(metaClass->getProperty("p2"), metaClass->getPropertyAt(2));
	GEQUAL_NOTNULL(metaClass->getProperty("p3"), metaClass->getPropertyAt(3));
	GEQUAL(metaClass->getProperty("none"), NULL);
}

GTEST(API_GetProperty)
{
	GApiScopedPointer<IMetaService> service(createMetaService());
	GCHECK(service);

	GApiScopedPointer<IMetaClass> metaClass(service->findClassByName(NAME(CLASS)));
	GCHECK(metaClass);
	GEQUAL(metaClass->getTypeSize(), sizeof(CLASS));
	
	GEQUAL(metaClass->getPropertyCount(), 4);
	GCHECK(itemEuqal(metaClass->getProperty("p0"), metaClass->getPropertyAt(0)));
	GCHECK(itemEuqal(metaClass->getProperty("p1"), metaClass->getPropertyAt(1)));
	GCHECK(itemEuqal(metaClass->getProperty("p2"), metaClass->getPropertyAt(2)));
	GCHECK(itemEuqal(metaClass->getProperty("p3"), metaClass->getPropertyAt(3)));
	GEQUAL(metaClass->getProperty("none"), NULL);
}
#undef CLASS



#define CLASS Class_GetMethod
class CLASS {
public:
	void m0() {}
	int m1() { return 0; }
	int m2(string) { return 0; }
	
	char data[18];
};


GMETA_DEFINE_CLASS(CLASS, CLASS, NAME(CLASS)) {
	GMETA_METHOD(m0);
	GMETA_METHOD(m1);
	GMETA_METHOD(m2);
}

GTEST(Lib_GetMethod)
{
	const GMetaClass * metaClass;

	metaClass = findMetaClass(NAME(CLASS));
	GCHECK(metaClass);
	GEQUAL(metaClass->getTypeSize(), sizeof(CLASS));
	
	GEQUAL(metaClass->getMethodCount(), 3);
	GEQUAL_NOTNULL(metaClass->getMethod("m0"), metaClass->getMethodAt(0));
	GEQUAL_NOTNULL(metaClass->getMethod("m1"), metaClass->getMethodAt(1));
	GEQUAL_NOTNULL(metaClass->getMethod("m2"), metaClass->getMethodAt(2));
	GEQUAL(metaClass->getMethod("none"), NULL);
}

GTEST(API_GetMethod)
{
	GApiScopedPointer<IMetaService> service(createMetaService());
	GCHECK(service);

	GApiScopedPointer<IMetaClass> metaClass(service->findClassByName(NAME(CLASS)));
	GCHECK(metaClass);
	GEQUAL(metaClass->getTypeSize(), sizeof(CLASS));
	
	GEQUAL(metaClass->getMethodCount(), 3);
	GCHECK(itemEuqal(metaClass->getMethod("m0"), metaClass->getMethodAt(0)));
	GCHECK(itemEuqal(metaClass->getMethod("m1"), metaClass->getMethodAt(1)));
	GCHECK(itemEuqal(metaClass->getMethod("m2"), metaClass->getMethodAt(2)));
	GEQUAL(metaClass->getMethod("none"), NULL);
}
#undef CLASS



#define CLASS Class_GetOperator
class CLASS {
public:
	void operator + (int) {}
	void operator - (int) {}
	void operator * (int) {}
	
	char data[199];
};


GMETA_DEFINE_CLASS(CLASS, CLASS, NAME(CLASS)) {
	reflectOperator<void (GMetaSelf, int)>(mopHolder + mopHolder);
	reflectOperator<void (GMetaSelf, int)>(mopHolder - mopHolder);
	reflectOperator<void (GMetaSelf, int)>(mopHolder * mopHolder);
}

GTEST(Lib_GetOperator)
{
	const GMetaClass * metaClass;

	metaClass = findMetaClass(NAME(CLASS));
	GCHECK(metaClass);
	GEQUAL(metaClass->getTypeSize(), sizeof(CLASS));
	
	GEQUAL(metaClass->getOperatorCount(), 3);
	GEQUAL_NOTNULL(metaClass->getOperator(mopHolder + mopHolder), metaClass->getOperatorAt(0));
	GEQUAL_NOTNULL(metaClass->getOperator(mopHolder - mopHolder), metaClass->getOperatorAt(1));
	GEQUAL_NOTNULL(metaClass->getOperator(mopHolder * mopHolder), metaClass->getOperatorAt(2));
	GEQUAL(metaClass->getOperator(static_cast<GMetaOpType>(1000)), NULL);
}

GTEST(API_GetOperator)
{
	GApiScopedPointer<IMetaService> service(createMetaService());
	GCHECK(service);

	GApiScopedPointer<IMetaClass> metaClass(service->findClassByName(NAME(CLASS)));
	GCHECK(metaClass);
	GEQUAL(metaClass->getTypeSize(), sizeof(CLASS));
	
	GEQUAL(metaClass->getOperatorCount(), 3);
	GCHECK(itemEuqal(metaClass->getOperator(mopHolder + mopHolder), metaClass->getOperatorAt(0)));
	GCHECK(itemEuqal(metaClass->getOperator(mopHolder - mopHolder), metaClass->getOperatorAt(1)));
	GCHECK(itemEuqal(metaClass->getOperator(mopHolder * mopHolder), metaClass->getOperatorAt(2)));
	GEQUAL(metaClass->getOperator(static_cast<GMetaOpType>(1000)), NULL);
}
#undef CLASS



#define CLASS Class_GetEnum
class CLASS {
public:
	enum E0 {};
	enum E1 {};
	enum E2 {};
	
	char data[19];
};


GMETA_DEFINE_CLASS(CLASS, CLASS, NAME(CLASS)) {
	reflectEnum<CLASS::E0>("E0");
	reflectEnum<CLASS::E1>("E1");
	reflectEnum<CLASS::E2>("E2");
}

GTEST(Lib_GetEnum)
{
	const GMetaClass * metaClass;

	metaClass = findMetaClass(NAME(CLASS));
	GCHECK(metaClass);
	GEQUAL(metaClass->getTypeSize(), sizeof(CLASS));
	
	GEQUAL(metaClass->getEnumCount(), 3);
	GEQUAL_NOTNULL(metaClass->getEnum("E0"), metaClass->getEnumAt(0));
	GEQUAL_NOTNULL(metaClass->getEnum("E1"), metaClass->getEnumAt(1));
	GEQUAL_NOTNULL(metaClass->getEnum("E2"), metaClass->getEnumAt(2));
	GEQUAL(metaClass->getEnum("none"), NULL);
}

GTEST(API_GetEnum)
{
	GApiScopedPointer<IMetaService> service(createMetaService());
	GCHECK(service);

	GApiScopedPointer<IMetaClass> metaClass(service->findClassByName(NAME(CLASS)));
	GCHECK(metaClass);
	GEQUAL(metaClass->getTypeSize(), sizeof(CLASS));
	
	GEQUAL(metaClass->getEnumCount(), 3);
	GCHECK(itemEuqal(metaClass->getEnum("E0"), metaClass->getEnumAt(0)));
	GCHECK(itemEuqal(metaClass->getEnum("E1"), metaClass->getEnumAt(1)));
	GCHECK(itemEuqal(metaClass->getEnum("E2"), metaClass->getEnumAt(2)));
	GEQUAL(metaClass->getEnum("none"), NULL);
}
#undef CLASS



#define CLASS Class_GetClass
class CLASS {
public:
	class C0 {};
	class C1 {};
	class C2 {};
	
	char data[8];
};


GMETA_DEFINE_CLASS(CLASS::C0, NestClass_C0, NAME(CLASS::C0)) {
}

GMETA_DEFINE_CLASS(CLASS::C1, NestClass_C1, NAME(CLASS::C1)) {
}

GMETA_DEFINE_CLASS(CLASS::C2, NestClass_C2, NAME(CLASS::C2)) {
}

GMETA_DEFINE_CLASS(CLASS, CLASS, NAME(CLASS)) {
	GMETA_CLASS(C0);
	GMETA_CLASS(C1);
	GMETA_CLASS(C2);
}

GTEST(Lib_GetClass)
{
	const GMetaClass * metaClass;

	metaClass = findMetaClass(NAME(CLASS));
	GCHECK(metaClass);
	GEQUAL(metaClass->getTypeSize(), sizeof(CLASS));
	
	GEQUAL(metaClass->getClassCount(), 3);
	GEQUAL_NOTNULL(metaClass->getClass(NAME(CLASS::C0)), metaClass->getClassAt(0));
	GEQUAL_NOTNULL(metaClass->getClass(NAME(CLASS::C1)), metaClass->getClassAt(1));
	GEQUAL_NOTNULL(metaClass->getClass(NAME(CLASS::C2)), metaClass->getClassAt(2));
	GEQUAL(metaClass->getClass("none"), NULL);
}

GTEST(API_GetClass)
{
	GApiScopedPointer<IMetaService> service(createMetaService());
	GCHECK(service);

	GApiScopedPointer<IMetaClass> metaClass(service->findClassByName(NAME(CLASS)));
	GCHECK(metaClass);
	GEQUAL(metaClass->getTypeSize(), sizeof(CLASS));
	
	GEQUAL(metaClass->getClassCount(), 3);
	GCHECK(itemEuqal(metaClass->getClass(NAME(CLASS::C0)), metaClass->getClassAt(0)));
	GCHECK(itemEuqal(metaClass->getClass(NAME(CLASS::C1)), metaClass->getClassAt(1)));
	GCHECK(itemEuqal(metaClass->getClass(NAME(CLASS::C2)), metaClass->getClassAt(2)));
	GEQUAL(metaClass->getClass("none"), NULL);
}
#undef CLASS





} }

