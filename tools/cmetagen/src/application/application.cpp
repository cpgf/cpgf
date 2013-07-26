#include "application.h"

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
	const char * sourceFileName = "z.h";

	Project project;
	project.loadProject("zzz.js");
	
	CppContext context(&project);
	context.process(project.getAbsoluteFileName(sourceFileName));

	BuilderContext builderContext(&project, project.getAbsoluteFileName(sourceFileName));
	builderContext.process(&context);

}


} // namespace metagen
