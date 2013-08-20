#include "config.h"
#include "commandlineparser.h"
#include "project.h"
#include "exception.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable: 4512)
#endif

#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/OptionProcessor.h"
#include "Poco/Util/HelpFormatter.h"
#include "Poco/Util/OptionCallback.h"
#include "Poco/Format.h"
#include "Poco/Path.h"

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

using namespace Poco::Util;
using namespace std;

namespace metagen {

CommandLineParser::CommandLineParser(Project * project)
	: project(project)
{
}

void CommandLineParser::parse(int argc, char * argv[])
{
	OptionSet optionSet;
	setupOptions(&optionSet);

	HelpFormatter helpFormatter(optionSet);
	string usage = Poco::format(
		"Usage: %s --project=PROJECT.js\n  --help for more information.",
		Poco::Path(argv[0]).getBaseName()
	);
	helpFormatter.setUsage(usage);

	if(argc <= 1) {
		fatalError(helpFormatter.getUsage());
	}

	OptionProcessor optionProcessor(optionSet);
	optionProcessor.setUnixStyle(true);
	for(int i = 1; i < argc; ++i) {
		string name;
		string value;
		optionProcessor.process(argv[i], name, value);
		const Option& option = optionSet.getOption(name);
		if(option.callback()) {
			option.callback()->invoke(name, value);
		}
	}
	optionProcessor.checkRequired();

	this->initializeProject();
}

void CommandLineParser::initializeProject()
{
	this->project->loadProject(this->projectFileName);
}

void CommandLineParser::setupOptions(OptionSet * optionSet)
{
	optionSet->addOption(
		Option("project", "", "The project file")
			.required(false)
			.argument("project script file")
			.callback(OptionCallback<CommandLineParser>(this, &CommandLineParser::onOptionProject))
	);
	optionSet->addOption(
		Option("help", "", "Help information")
			.required(false)
	);
}

void CommandLineParser::onOptionProject(const std::string & /*name*/, const std::string & value)
{
	this->projectFileName = value;
}


} // namespace metagen
