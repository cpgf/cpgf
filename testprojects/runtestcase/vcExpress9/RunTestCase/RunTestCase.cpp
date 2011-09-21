//#include "vld.h"

#include "stdafx.h"

#include <stdlib.h>
#include <crtdbg.h>

#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif 

extern int runTestMain();

int _tmain(int argc, _TCHAR* argv[])
{
	(void)argc; (void)argv;

	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

	return runTestMain();
}


