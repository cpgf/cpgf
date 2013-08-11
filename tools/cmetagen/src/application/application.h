#ifndef __APPLICATION_H
#define __APPLICATION_H

#include "project.h"

#include <string>
#include <set>

namespace metagen {

class BuilderSection;
class BuilderContext;

class Application
{
private:
	typedef std::set<std::string> CreationFunctionNameListType;

public:
	Application();
	~Application();
	
	int run(int argc, char * argv[]);

private:
	void doRun(int argc, char * argv[]);
	void processFiles();
	void processOnePath(const std::string & path);
	void processOneFile(const std::string & file);

	void onGenerateCreationFunction(const BuilderContext * builderContext, BuilderSection * section);

	void generateMainRegisterHeaderFile() const;
	void generateMainRegisterSourceFile() const;

private:	
	Project project;
	CreationFunctionNameListType creationFunctionNameList;
};


} // namespace metagen


#endif
