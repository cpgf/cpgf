#include "unittestbase.h"

#include <iostream>

extern int totalPythonObjectCount;
extern int pythonObjectCount;

int main()
{
	int result = UnitTest::RunAllTests();

//	std::cout << "totalPythonObjectCount " << totalPythonObjectCount << std::endl;
//	std::cout << "pythonObjectCount " << pythonObjectCount << std::endl;

	return result;
}

