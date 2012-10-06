#ifndef __SIMPLEOBJECT_H
#define __SIMPLEOBJECT_H

#include "cpgf/gsharedptr.h"

#include <string>


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

public:
	SimpleData data;
};

namespace cpgf {
struct IMetaClass;
}

cpgf::IMetaClass * getSimpleObjectClass(cpgf::IMetaClass * metaClass);
bool metaClassIsSimpleObject(cpgf::IMetaClass * metaClass); 
cpgf::GSharedPointer<SimpleObject> createSharedSimpleObject();
SimpleObject * getSharedSimpleObject(cpgf::GSharedPointer<SimpleObject> sp);
int getSimpleObjectPointerN(SimpleObject * object);
int getSimpleObjectN(const SimpleObject & object);


#endif
