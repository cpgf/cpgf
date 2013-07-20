#include "test_reflection_common.h"

using namespace std;
using namespace cpgf;

namespace Test_ClassHierarchy { namespace {


class ClassA {
public:
	int fieldA;
	int fieldP;
	
	virtual ~ClassA() {}

	virtual int getID() { return 1; }

	void methodA() {}
	
	int operator + (int) const { return 1; }
	int operator * (int) const { return 10; }
};


G_AUTO_RUN_BEFORE_MAIN()
{
	using namespace cpgf;

	GDefineMetaClass<ClassA>
		::define(NAME(ClassA))
		
		._field("fieldA", &ClassA::fieldA)
		._field("fieldP", &ClassA::fieldP)

		._method("getID", &ClassA::getID)
		._method("methodA", &ClassA::methodA)
	
		._operator<int (const GMetaSelf &, int)>(mopHolder + mopHolder)
		._operator<int (const GMetaSelf &, int)>(mopHolder * mopHolder)
	;
}


class ClassB : public ClassA {
public:
	int fieldB;
	int fieldP;
	
	virtual int getID() { return 2; }
};
int operator + (const ClassB &, int) { return 2; }


G_AUTO_RUN_BEFORE_MAIN()
{
	using namespace cpgf;

	GDefineMetaClass<ClassB, ClassA>
		::define(NAME(ClassB))
		
		._field("fieldB", &ClassB::fieldB)
		._field("fieldP", &ClassB::fieldP)

		._method("getID", &ClassB::getID)
	
		._operator<int (const GMetaSelf &, int)>(mopHolder + mopHolder)
	;
}


GTEST(Lib_ClassHierarchy_ClassA)
{
	const GMetaClass * metaClass = findMetaClass(NAME(ClassA));
	GCHECK(metaClass);
	
	const GMetaMethod * method;
	const GMetaField * field;
	const GMetaOperator * opera;
	
	ClassA instanceA;
	void * pobj = &instanceA;
	
	GEQUAL(metaClass->getBaseCount(), 0);
	
	FIELD(fieldA);
	GCHECK(field);
	GEQUAL(&instanceA.fieldA, field->getAddress(pobj));

	FIELD(fieldP);
	GCHECK(field);
	GEQUAL(&instanceA.fieldP, field->getAddress(pobj));

	METHOD_HI(getID);
	GCHECK(method);
	GEQUAL(fromVariant<int>(method->invoke(pobj)), 1);

	METHOD(methodA);
	GCHECK(method);

	pobj = metaClass->createInstance();
	METHOD_HI(getID);
	GCHECK(method);
	GEQUAL(fromVariant<int>(method->invoke(pobj)), 1);
	metaClass->destroyInstance(pobj);

	OPERA(mopHolder + mopHolder);	
	GCHECK(opera);
	GEQUAL(fromVariant<int>(opera->invokeBinary(pobj, 0)), 1);

	OPERA(mopHolder * mopHolder);	
	GCHECK(opera);
	GEQUAL(fromVariant<int>(opera->invokeBinary(pobj, 0)), 10);
}

GTEST(Lib_ClassHierarchy_ClassB)
{
	const GMetaClass * metaClass = findMetaClass(NAME(ClassB));
	GCHECK(metaClass);
	
	GCHECK(metaClass->isInheritedFrom(findMetaClass(NAME(ClassA))));
	
	const GMetaMethod * method;
	const GMetaField * field;
	const GMetaOperator * opera;
	
	ClassB instanceB;
	void * pobj = &instanceB;
	
	GEQUAL(metaClass->getBaseCount(), 1);
	
	FIELD_HI(fieldB);
	GCHECK(field);
	GEQUAL(&instanceB.ClassB::fieldB, field->getAddress(pobj));
	
	FIELD(fieldA);
	GCHECK(!field);

	FIELD_HI(fieldA);
	GCHECK(field);

	FIELD(fieldP);
	GCHECK(field);

	FIELD_HI(fieldP);
	GCHECK(field);
	GEQUAL(&instanceB.ClassB::fieldP, field->getAddress(pobj));

	METHOD_HI(getID);
	GCHECK(method);
	GEQUAL(fromVariant<int>(method->invoke(pobj)), 2);

	METHOD(methodA);
	GCHECK(!method);

	METHOD_HI(methodA);
	GCHECK(method);

	pobj = metaClass->createInstance();
	METHOD_HI(getID);
	GCHECK(method);
	GEQUAL(fromVariant<int>(method->invoke(pobj)), 2);
	
	OPERA(mopHolder + mopHolder);	
	GCHECK(opera);
	GEQUAL(fromVariant<int>(opera->invokeBinary(pobj, 0)), 2);

	OPERA(mopHolder * mopHolder);	
	GCHECK(!opera);

	OPERA_HI(mopHolder * mopHolder);	
	GCHECK(opera);
	GEQUAL(fromVariant<int>(opera->invokeBinary(pobj, 0)), 10);
	metaClass->destroyInstance(pobj);
}




} }

