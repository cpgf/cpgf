#include "application/application.h"

#include "codewriter/cppwriter.h"
#include "codewriter/codewriter.h"

#include <iostream>
using namespace std;
using namespace metagen;

int main(int argc, char * argv[])
{
	Application application(argc, argv);
	application.run();

	return 0;
}
