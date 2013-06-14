#include "cppclass.h"
#include "cppconstructor.h"
#include "cppdestructor.h"

#include "../util.h"


CppClass::CppClass()
	: super(), destructor(NULL)
{
}

CppClass::~CppClass()
{
	clearPointerContainer(this->baseClassList);
}

void CppClass::doAddItem(CppItem * item)
{
	switch(item->getCategory()) {
		case icConstructor:
			this->constructorList.push_back(static_cast<CppConstructor *>(item));
			break;

		case icDestructor:
			this->destructor = static_cast<CppDestructor *>(item);
			break;
			
		default:
			super::doAddItem(item);
			break;
	}
}

