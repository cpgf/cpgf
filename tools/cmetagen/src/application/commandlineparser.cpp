#include "config.h"
#include "commandlineparser.h"
#include "project.h"
#include "exception.h"
#include "constants.h"
#include "util.h"

#include "cpgf/scriptbind/gscriptvalue.h"
#include "cpgf/gmetaapiutil.h"
#include "cpgf/gscopedinterface.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable: 4512 4244)
#endif

#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/OptionProcessor.h"
#include "Poco/Util/HelpFormatter.h"
#include "Poco/Util/OptionCallback.h"
#include "Poco/Format.h"
#include "Poco/Path.h"
#include "Poco/String.h"
#include "Poco/NumberParser.h"

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#include <functional>

using namespace Poco::Util;
using namespace std;
using namespace cpgf;

namespace metagen {

const string optionProject("project");
const string optionHelp("help");

void CommandLineParser::ProjectOption::verify(const std::string & name, const std::string & value)
{
	if(vtIsBoolean(this->vt)) {
		bool b;
		if(! this->getAsBoolean(value, &b)) {
			fatalError(Poco::format("The value of command option %s must be true, false, 0, or 1", name));
		}
	}
	else if(vtIsInteger(this->vt)) {
		if(std::find_if(value.begin(), value.end(), std::not1(std::ptr_fun((int (*)(int))&isdigit))) != value.end()) {
			fatalError(Poco::format("The value of command option %s must be integer", name));
		}
	}
}

bool CommandLineParser::ProjectOption::getAsBoolean(const std::string & value, bool * result)
{
	string v = Poco::toLower(value);
	if(v == "0" || v == "false") {
		*result = false;
		return true;
	}
	if(v == "1" || v == "true") {
		*result = true;
		return true;
	}
	return false;
}

void CommandLineParser::ProjectOption::setToField(Project * project, IMetaField * field)
{
	if(vtIsBoolean(this->vt)) {
		bool b;
		this->getAsBoolean(this->values[0], &b);
		metaSetValue(field, project, b);
	}
	else if(vtIsInteger(this->vt)) {
		metaSetValue(field, project, Poco::NumberParser::parse(this->values[0]));
	}
	else {
		const string name = field->getName();
		if(name == scriptFieldFiles) {
			appendFileNames(&project->files, this->values);
		}
		else if(this->vt == vtString) {
			metaSetValue(field, project, this->values[0].c_str());
		}
	}
}

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
		if(name == optionProject) {
			this->projectFileName = value;
		}
		else if(name == optionHelp) {
		}
		else {
			ProjectOptionMapType::iterator it = this->projectOptionMap.find(name);
			if(it != this->projectOptionMap.end()) {
				it->second.verify(name, value);
				it->second.values.push_back(value);
			}
		}
	}
	optionProcessor.checkRequired();
	this->processProjectOptions();

	this->initializeProject();
}

bool CommandLineParser::projectVisitorFilter(const std::string & fieldName)
{
	ProjectOptionMapType::iterator it = this->projectOptionMap.find(fieldName);
	if(it != this->projectOptionMap.end()) {
		return it->second.values.empty();
	}

	return true;
}

void CommandLineParser::initializeProject()
{
	this->project->loadProject(this->projectFileName, makeCallback(this, &CommandLineParser::projectVisitorFilter));
}

class ProjectOptionVisitor : public ProjectVisitor
{
public:
	explicit ProjectOptionVisitor(Project * project, OptionSet * optionSet, CommandLineParser::ProjectOptionMapType * projectOptionMap)
		: ProjectVisitor(project), optionSet(optionSet), projectOptionMap(projectOptionMap) {
	}

	virtual void visitStringArray(cpgf::IMetaField * /*field*/, const std::string & fieldName, const cpgf::GScriptValue & /*fieldValue*/,
		StringArrayType * /*stringArray*/) {
		CommandLineParser::ProjectOption projectOption;
		projectOption.vt = vtString;
		projectOptionMap->insert(make_pair(fieldName, projectOption));

		this->optionSet->addOption(
			Option(fieldName, "", "")
				.required(false)
				.repeatable(true)
				.argument("argument")
		);
	}

	virtual void visitTemplateInstantiations(cpgf::IMetaField * /*field*/, const std::string & /*fieldName*/,
		const cpgf::GScriptValue & /*fieldValue*/) {
	}

	virtual void visitScriptCallback(cpgf::IMetaField * /*field*/, const std::string & /*fieldName*/,
		const cpgf::GScriptValue & /*fieldValue*/,
		cpgf::GSharedInterface<cpgf::IScriptFunction> * /*callback*/) {
	}

	virtual void visitString(cpgf::IMetaField * /*field*/, const std::string & fieldName, const cpgf::GScriptValue & /*fieldValue*/) {
		CommandLineParser::ProjectOption projectOption;
		projectOption.vt = vtString;
		projectOptionMap->insert(make_pair(fieldName, projectOption));

		this->optionSet->addOption(
			Option(fieldName, "", "")
				.required(false)
				.repeatable(false)
				.argument("argument")
		);
	}

	virtual void visitInteger(cpgf::IMetaField * field, const std::string & fieldName, const cpgf::GScriptValue & /*fieldValue*/) {
		CommandLineParser::ProjectOption projectOption;
		projectOption.vt = metaGetItemType(field).getVariantType();
		projectOptionMap->insert(make_pair(fieldName, projectOption));

		this->optionSet->addOption(
			Option(fieldName, "", "")
				.required(false)
				.repeatable(false)
				.argument("argument")
		);
	}

private:
	OptionSet * optionSet;
	CommandLineParser::ProjectOptionMapType * projectOptionMap;
};

void CommandLineParser::setupOptions(OptionSet * optionSet)
{
	optionSet->addOption(
		Option(optionProject, "", "")
			.required(false)
			.argument("project script file")
	);
	optionSet->addOption(
		Option(optionHelp, "", "")
			.required(false)
	);

	ProjectOptionVisitor optionVisitor(this->project, optionSet, &this->projectOptionMap);
	this->project->visitProject(&optionVisitor);
}

void CommandLineParser::processProjectOptions()
{
	GScopedInterface<IMetaClass> projectClass(this->project->getProjectClass());
	GScopedInterface<IMetaField> field;

	for(ProjectOptionMapType::iterator it = this->projectOptionMap.begin();
		it != this->projectOptionMap.end();
		++it) {
		if(it->second.values.empty()) {
			continue;
		}
		field.reset(projectClass->getField(it->first.c_str()));
		it->second.setToField(this->project, field.get());
	}
}


} // namespace metagen
