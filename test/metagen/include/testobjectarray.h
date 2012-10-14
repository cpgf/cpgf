#ifndef __TESTOBJECTARRAY_H
#define __TESTOBJECTARRAY_H

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




#endif
