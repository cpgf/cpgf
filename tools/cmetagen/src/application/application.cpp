#include "application.h"

#include "parser/clangparser.h"
#include "model/cppcontext.h"
#include "model/cppnamespace.h"
#include "buildermodel/buildercontext.h"
#include "config.h"


Application::Application(int argc, char * argv[])
{
}

Application::~Application()
{
}

void Application::run()
{
	CppContext context;
	ClangParser parser(&context);
	
	parser.parse("z.h");

	Config config;
	BuilderContext builderContext(&config);
	builderContext.process(&context);

}

