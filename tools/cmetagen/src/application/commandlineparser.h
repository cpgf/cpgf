#ifndef __COMMANDLINEPARSER_H
#define __COMMANDLINEPARSER_H

#include "cpgf/gvariant.h"

#include "Poco/Util/OptionSet.h"

#include <string>
#include <map>
#include <vector>

namespace cpgf {
struct IMetaField;
}

namespace metagen {

class Project;

class CommandLineParser
{
private:
	struct ProjectOption {
		cpgf::GVariantType vt;
		std::vector<std::string> values;

		void verify(const std::string & name, const std::string & value);
		bool getAsBoolean(const std::string & value, bool * result);
		void setToField(Project * project, cpgf::IMetaField * field);
	};

	typedef std::map<std::string, ProjectOption> ProjectOptionMapType;

public:
	explicit CommandLineParser(Project * project);
	void parse(int argc, char * argv[]);

private:
	void initializeProject();
	void setupOptions(Poco::Util::OptionSet * optionSet);
	void processProjectOptions();

private:
	Project * project;
	std::string projectFileName;
	ProjectOptionMapType projectOptionMap;

	friend class ProjectOptionVisitor;
};


} // namespace metagen


#endif
