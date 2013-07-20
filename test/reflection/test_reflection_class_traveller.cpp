#include "test_reflection_common.h"

#include "cpgf/gmetaclasstraveller.h"


#define MM(cls) GPP_STRINGIZE(TestTraveller_ ## cls)

using namespace std;
using namespace cpgf;

namespace Test_Class_Traveller { namespace {

bool itemEuqal(IMetaItem * a, IMetaItem * b)
{
	bool equal = a->equals(b);
	
	a->releaseReference();
	b->releaseReference();
	
	return equal;
}

class XA
{
	int a;
public:
	virtual ~XA() {}
	virtual void x() {}
};


class XB
{
	double b;
};

class XC : virtual public XA, virtual public XB
{
	char c;
};

class XD : virtual public XA
{
	short d;
};

class XE : public XC, public XD
{
	long double e;
};


G_AUTO_RUN_BEFORE_MAIN()
{
	using namespace cpgf;

	GDefineMetaClass<XA>
		::define(MM(XA))
	;

	GDefineMetaClass<XB>
		::define(MM(XB))
	;

	GDefineMetaClass<XC, XA, XB>
		::define(MM(XC))
	;

	GDefineMetaClass<XD, XA>
		::define(MM(XD))
	;

	GDefineMetaClass<XE, XC, XD>
		::define(MM(XE))
	;
}


GTEST(API_TestTraveller)
{
	GScopedInterface<IMetaService> service(createDefaultMetaService());
	GCHECK(service);

	GScopedInterface<IMetaClass> metaClass(service->findClassByName(MM(XE)));
	GCHECK(metaClass);

	GMetaClassTraveller traveller(metaClass.get(), NULL);
	void * nextInstance;
	IMetaClass * nextClass;
	
	nextClass = traveller.next(&nextInstance);
	GCHECK(itemEuqal(nextClass, service->findClassByName(MM(XE))));
	
	nextClass = traveller.next(&nextInstance);
	GCHECK(itemEuqal(nextClass, service->findClassByName(MM(XC))));
	
	nextClass = traveller.next(&nextInstance);
	GCHECK(itemEuqal(nextClass, service->findClassByName(MM(XD))));
	
	nextClass = traveller.next(&nextInstance);
	GCHECK(itemEuqal(nextClass, service->findClassByName(MM(XA))));
	
	nextClass = traveller.next(&nextInstance);
	GCHECK(itemEuqal(nextClass, service->findClassByName(MM(XB))));
	
	nextClass = traveller.next(&nextInstance);
	GCHECK(itemEuqal(nextClass, service->findClassByName(MM(XA))));

	nextClass = traveller.next(&nextInstance);
	GCHECK(nextClass == NULL);
}




} }

