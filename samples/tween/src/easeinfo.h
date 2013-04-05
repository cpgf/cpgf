#ifndef __EASEINFO_H
#define __EASEINFO_H

#include "cpgf/tween/easing/quint.h"


struct EaseInfo
{
	const char * name;
	cpgf::GTweenEaseType ease;
};

int getEaseCount();
const EaseInfo * getEase(int index);


#endif
