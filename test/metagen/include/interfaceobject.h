#ifndef CPGF_INTERFACEOBJECT_H
#define CPGF_INTERFACEOBJECT_H

#include "cpgf/gapiutil.h"
#include "cpgf/gsharedinterface.h"


class MyInterfaceObject : public cpgf::IObject
{
	G_INTERFACE_IMPL_OBJECT
	
public:
	virtual ~MyInterfaceObject() {}
};

class MyInterfaceHolder
{
public:
	explicit MyInterfaceHolder(cpgf::IObject * object)
		: object(object)
	{
	}
	
private:
	cpgf::GSharedInterface<cpgf::IObject> object;
};


#endif
