#include "../include/simpleobject.h"
#include "cpgf/gmetaapi.h"

using namespace std;
using namespace cpgf;


SimpleData::SimpleData(int n, const std::string & s)
	: n(n), s(s), ws(L"")
{
}

SimpleData::SimpleData(const SimpleData & other)
	: n(other.n), s(other.s), ws(other.ws), atom(other.atom)
{
}



SimpleData & SimpleObject::getData()
{
	return this->data;
}

SimpleData * SimpleObject::getDataPtr()
{
	return &this->data;
}

void SimpleObject::setData(const SimpleData & data)
{
	this->data = data;
}

void SimpleObject::setDataPtr(const SimpleData * data)
{
	this->data = *data;
}

bool SimpleObject::checkData(int n) const
{
	return this->data.n == n;
}

bool SimpleObject::checkData(const std::string & s) const
{
	return this->data.s == s;
}

bool SimpleObject::checkAtom(long value) const
{
	return this->data.atom.value == value;
}


cpgf::IMetaClass * getSimpleObjectClass(cpgf::IMetaClass * metaClass)
{
	metaClass->addReference();

	return metaClass;
}

bool metaClassIsSimpleObject(IMetaClass * metaClass)
{
	return string(metaClass->getName()) == "SimpleObject";
}
