#include "application.h"

#include "model/cppcontext.h"
#include "model/cppnamespace.h"
#include "buildermodel/buildercontext.h"
#include "exception.h"
#include "logger.h"
#include "util.h"

#include "Poco/Glob.h"
#include "Poco/File.h"
#include "Poco/Format.h"

#include <set>

#include <iostream>

using namespace std;

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
		logger.error(string("\n\n") + e.what());
	}
}

void Application::doRun()
{
	this->project.loadProject("zzz.js");

	this->processFiles();
}

void Application::processFiles()
{
	for(StringArrayType::const_iterator it = this->project.getFiles().begin();
		it != this->project.getFiles().end();
		++it) {
		this->processOnePath(*it);
	}
}

void Application::processOnePath(const std::string & path)
{
	set<string> fileSet;
	string absolutePath = this->project.getAbsoluteFileName(path);

	Poco::Glob::glob(absolutePath, fileSet, Poco::Glob::GLOB_DOT_SPECIAL);
	for(set<string>::iterator it = fileSet.begin(); it != fileSet.end(); ++it) {
		if(Poco::File(*it).isDirectory()) {
			continue;
		}
		else {
			this->processOneFile(*it);
		}
	}
}

void Application::processOneFile(const std::string & file)
{
	if(this->project.doesForce()
		|| shouldTargetFileBeUpdated(file, this->project.getOutputHeaderFileName(file))) {
		logger.info(Poco::format("Generate for file %s...\n", file));

		CppContext context(&this->project);
		string absoluteFileName = this->project.getAbsoluteFileName(file);
		context.process(absoluteFileName);

		BuilderContext builderContext(&this->project, absoluteFileName);
		builderContext.process(&context);

		logger.info("done.\n");
	}
	else {
		logger.info(Poco::format("File %s is up to date, skipped.\n", file));
	}
}



} // namespace metagen
