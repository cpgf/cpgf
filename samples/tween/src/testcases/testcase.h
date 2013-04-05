#ifndef __TESTCASE_H
#define __TESTCASE_H

#include "cpgf/gsharedptr.h"

class TestCase
{
public:
	virtual void render(int viewWidth, int viewHeight) = 0;
	virtual void setEase(int easeIndex) = 0;
	virtual bool shouldShowEaseButtons() {
		return true;
	}
};

typedef cpgf::GSharedPointer<TestCase> TestCasePtr;

#endif
