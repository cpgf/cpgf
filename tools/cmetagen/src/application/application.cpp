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
	Config config;
	CppContext context(&config);
	ClangParser parser(&context);
	
	parser.parse("z.h");

	BuilderContext builderContext(&config);
	builderContext.process(&context);

}

