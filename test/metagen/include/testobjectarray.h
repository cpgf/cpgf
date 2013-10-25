#ifndef CPGF_TESTOBJECTARRAY_H
#define CPGF_TESTOBJECTARRAY_H

#include "cpgf/gsharedptr.h"

#include <string>


class OAData
{
public:
	OAData()
		: n(0), s()
	{
	}
	
	OAData(int n, const std::string & s)
		: n(n), s(s)
	{
	}
	
	int n;
	std::string s;
};

typedef cpgf::GSharedPointer<OAData> OADataPointer;

class OAObject
{
public:
	OAObject()
		: data(new OAData)
	{
	}

	OAObject(int n, const std::string & s)
		: data(new OAData(n, s))
	{
	}

public:
	OADataPointer data;
};

inline int getOAObjectN(OAObject * obj) {
	return obj->data->n;
}

inline int getOAObjectN(OAObject * obj, int index) {
	return obj[index].data->n;
}

inline std::string getOAObjectS(const OAObject & obj) {
	return obj.data->s;
}


#endif
