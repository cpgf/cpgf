#include "application.h"

#include "model/cppcontext.h"
#include "model/cppnamespace.h"
#include "buildermodel/buildercontext.h"
#include "buildermodel/builderutil.h"
#include "codewriter/cppwriter.h"
#include "codewriter/codewriter.h"
#include "codewriter/codeblock.h"
#include "exception.h"
#include "logger.h"
#include "util.h"
#include "constants.h"
#include "commandlineparser.h"

#include "Poco/Glob.h"
#include "Poco/File.h"
#include "Poco/Format.h"
#include "Poco/Exception.h"

#include <set>

#include <iostream>

using namespace std;
using namespace cpgf;

namespace metagen {


Application::Application()
{
}

Application::~Application()
{
}

int Application::run(int argc, char * argv[])
{
	int exitCode = 1;
	try {
		this->doRun(argc, argv);
		exitCode = 0;
	}
	catch(const AbortException &) {
	}
	catch(const Exception & e) {
		getLogger().error(string("\n") + e.what());
	}
	catch(const Poco::Exception & e) {
		getLogger().error(string("\n") + e.displayText());
	}
	cout << endl;
	return exitCode;
}

void Application::doRun(int argc, char * argv[])
{
	CommandLineParser commandLineParser(&this->project);
	commandLineParser.parse(argc, argv);

	this->processFiles();
	generateMainRegisterFiles(this->creationFunctionNameList, &this->project);
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
		getLogger().info(Poco::format("Generate for file %s...", file));

		CppContext context(&this->project);
		string absoluteFileName = this->project.getAbsoluteFileName(file);
		context.process(absoluteFileName);

		BuilderContext builderContext(&this->project, absoluteFileName,
			makeCallback(this, &Application::onGenerateCreationFunction));
		builderContext.process(&context);

		getLogger().info("done.\n");
	}
	else {
		getLogger().info(Poco::format("File %s is up to date, skipped.\n", file));
	}
}

void Application::onGenerateCreationFunction(const BuilderContext * builderContext, BuilderSection * section)
{
	// nested class, don't register to global
	if(section->getCppItem()->getParent() != NULL
		&& section->getCppItem()->getParent()->isClass()) {
		return;
	}

	string name = getCreationFunctionName(builderContext, section);
	if(this->creationFunctionNameList.find(name) != this->creationFunctionNameList.end()) {
		getLogger().warn(Poco::format("Creation function %s is duplicated.", name));
	}
	this->creationFunctionNameList.insert(name);
}


} // namespace metagen
