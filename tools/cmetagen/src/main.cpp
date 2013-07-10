#include "application/application.h"

#include "codewriter/cppwriter.h"
#include "codewriter/codewriter.h"

#include <iostream>
using namespace std;
using namespace metagen;

void testCppWriter()
{
	CppWriter cppWriter;
	CodeWriter codeWriter;

	cppWriter.setNamespace("ns");
	cppWriter.include("def.h");
	cppWriter.getCodeBlock()->appendLine("void a();");

	cppWriter.write(&codeWriter);
	cout << codeWriter.getText();
}

int main(int argc, char * argv[])
{
	Application application(argc, argv);
	application.run();


//	testCppWriter();

//	context.getGlobalNamespace()->dump(std::cout, 0);

	return 0;
}
