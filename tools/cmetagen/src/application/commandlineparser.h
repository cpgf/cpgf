#ifndef __COMMANDLINEPARSER_H
#define __COMMANDLINEPARSER_H

#include "Poco/Util/OptionSet.h"

#include <string>

namespace metagen {

class Project;

class CommandLineParser
{
public:
	explicit CommandLineParser(Project * project);
	void parse(int argc, char * argv[]);

private:
	void initializeProject();
	void setupOptions(Poco::Util::OptionSet * optionSet);

	void onOptionProject(const std::string & name, const std::string & value);

private:
	Project * project;
	std::string projectFileName;
};


} // namespace metagen


#endif
