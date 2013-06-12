#ifndef __CPPDESTRUCTOR_H
#define __CPPDESTRUCTOR_H

#include "cppitem.h"


class CppDestructor : public CppItem
{
protected:
	virtual ItemCategory getCategory() const { return icDestructor; }
};


#endif
