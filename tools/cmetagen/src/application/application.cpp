#include "application.h"

#include "parser/clangparser.h"
#include "model/cppcontext.h"
#include "model/cppnamespace.h"
#include "buildermodel/buildercontext.h"
#include "project.h"
#include "exception.h"

#include <iostream>

namespace metagen {


Application::Application(int argc, char * argv[])
{
}

Application::~Application()
{
}

void Application::run()
{
	try {
		this->doRun();
	}
	catch(const Exception & e) {
		std::cerr << e.what() << std::endl;
	}
}

void Application::doRun()
{
	Project project;
	CppContext context(&project);
	ClangParser parser(&context);
	
	parser.parse("z.h");

	BuilderContext builderContext(&project, "d:\\test.h");
	builderContext.process(&context);

}


} // namespace metagen
