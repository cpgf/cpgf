#ifndef CPGF_SIMPLEOBJECT_H
#define CPGF_SIMPLEOBJECT_H

#include "cpgf/gsharedptr.h"

#if ! defined(_MSC_VER)
	#if defined(SUPPORT_CPP_11) && SUPPORT_CPP_11
		#define TEST_CPP11_SHARED_PTR
	#endif
#endif

#ifdef TEST_CPP11_SHARED_PTR
#include "cpgf/metatraits/gmetasharedptrtraits_cpp11_shared_ptr.h"
#endif

#if ENABLE_BOOST
#include "cpgf/metatraits/gmetasharedptrtraits_boost_shared_ptr.h"
#endif

#include <string>
#include <memory>


struct SimpleAtom
{
	long value;
};

struct SimpleData
{
	SimpleData(int n = 0, const std::string & s = "");
	SimpleData(const SimpleData & other);

	int n;
	std::string s;
	std::wstring ws;
	SimpleAtom atom;
};

class SimpleObject
{
public:
	SimpleData & getData();
	SimpleData * getDataPtr();

	void setData(const SimpleData & data);
	void setDataPtr(const SimpleData * data);

	bool checkData(int n) const;
	bool checkData(const std::string & s) const;
	bool checkAtom(long value) const;

	SimpleData & operator() ();
	const SimpleData & operator() () const;

public:
	SimpleData data;
	SimpleObject * pobj;
};

namespace cpgf {
struct IMetaClass;
}

cpgf::IMetaClass * getSimpleObjectClass(cpgf::IMetaClass * metaClass);
bool metaClassIsSimpleObject(cpgf::IMetaClass * metaClass);

cpgf::GSharedPointer<SimpleObject> createSharedSimpleObject();
SimpleObject * getSharedSimpleObject(cpgf::GSharedPointer<SimpleObject> sp);

#ifdef TEST_CPP11_SHARED_PTR
typedef std::shared_ptr<SimpleObject> CPP11_SP;
#else
typedef cpgf::GSharedPointer<SimpleObject> CPP11_SP;
#endif
CPP11_SP createCpp11SharedSimpleObject();
SimpleObject * getCpp11SharedSimpleObject(CPP11_SP sp);

#if ENABLE_BOOST
typedef boost::shared_ptr<SimpleObject> BOOST_SP;
#else
typedef cpgf::GSharedPointer<SimpleObject> BOOST_SP;
#endif
BOOST_SP createBoostSharedSimpleObject();
SimpleObject * getBoostSharedSimpleObject(BOOST_SP sp);

int getSimpleObjectPointerN(SimpleObject * object);
int getSimpleObjectN(const SimpleObject & object);

void setViaConstRefPtrApi(SimpleObject * const &input);
SimpleObject* &returnFromConstRefPtrApi();

#endif
