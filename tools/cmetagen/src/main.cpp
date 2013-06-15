#include "application/application.h"

#include "parser/clangparser.h"
#include "model/cppcontext.h"
#include "model/cppnamespace.h"
#include "buildermodel/buildercontext.h"
#include "application/config.h"

#include "codewriter/cppwriter.h"
#include "codewriter/codewriter.h"

#include <iostream>
using namespace std;

void testCppWriter()
{
	CppWriter cppWriter("abc.h");
	CodeWriter codeWriter;

	cppWriter.setNamespace("ns");
	cppWriter.include("def.h");
	cppWriter.getMainBlock()->addLine("void a();");

	cppWriter.write(&codeWriter);
	cout << codeWriter.getText();
}

int main(int argc, char * argv[])
{
	Application application(argc, argv);
	application.run();


	CppContext context;
	ClangParser parser(&context);
	
	parser.parse("z.h");

	Config config;
	BuilderContext builderContext(&config);
	builderContext.process(&context);

	testCppWriter();

//	context.getGlobalNamespace()->dump(std::cout, 0);

	return 0;
}
