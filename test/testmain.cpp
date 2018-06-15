#include "cpgf/gcompiler.h"
#include "unittestbase.h"

#include <iostream>
#include <stdio.h>

int main()
{
#if G_DEBUG
#ifdef G_COMPILER_VC
	_CrtSetDbgFlag (_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
#endif

//	printf("Press any key to start..."); getchar();

	int result = UnitTest::RunAllTests();

	return result;
}

