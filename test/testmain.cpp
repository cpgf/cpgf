#include "cpgf/gcompiler.h"
#include "unittestbase.h"

#include <iostream>

int main()
{
#if G_DEBUG
#ifdef G_COMPILER_VC
	_CrtSetDbgFlag (_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
#endif

	int result = UnitTest::RunAllTests();

	return result;
}

