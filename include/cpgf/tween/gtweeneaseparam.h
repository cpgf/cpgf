#ifndef __GTWEENEASEPARAM_H
#define __GTWEENEASEPARAM_H

#include "cpgf/gcallback.h"


namespace cpgf {

typedef float GTweenNumber;

struct GTweenEaseParam
{
	GTweenNumber current;
	GTweenNumber total;
};

typedef GTweenNumber (*GTweenEaseFunction)(const GTweenEaseParam *);
typedef GCallback<GTweenNumber (*)(const GTweenEaseParam *)> GTweenEaseType;


} // namespace cpgf


#endif
