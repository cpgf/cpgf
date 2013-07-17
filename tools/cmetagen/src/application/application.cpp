#include "application.h"

#include "parser/clangparser.h"
#include "model/cppcontext.h"
#include "model/cppnamespace.h"
#include "buildermodel/buildercontext.h"
#include "project.h"

namespace metagen {


Application::Application(int argc, char * argv[])
{
}

Application::~Application()
{
}

void Application::run()
{
	Project project;
	CppContext context(&project);
	ClangParser parser(&context);
	
	parser.parse("z.h");

	BuilderContext builderContext(&project, "d:\\test.h");
	builderContext.process(&context);

}


} // namespace metagen
