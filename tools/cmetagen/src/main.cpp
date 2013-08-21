#include "application/application.h"

#include "codewriter/cppwriter.h"
#include "codewriter/codewriter.h"

int main(int argc, char * argv[])
{
	metagen::Application application;
	return application.run(argc, argv);
}
