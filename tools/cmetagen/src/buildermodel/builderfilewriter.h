#ifndef __BUILDERFILEWRITER_H
#define __BUILDERFILEWRITER_H

#include "buildersection.h"

#include <string>
#include <vector>

namespace metagen {

class BuilderContext;
class Project;

class BuilderFileWriter
{
private:
	BuilderFileWriter(const std::string & sourceFileName, const BuilderContext * builderContext, int fileIndex);
	
public:
	static BuilderFileWriter * createHeaderFile(const std::string & sourceFileName, const BuilderContext * builderContext);
	static BuilderFileWriter * createSourceFile(const std::string & sourceFileName, const BuilderContext * builderContext, int fileIndex);
	
public:
	void addSection(BuilderSection * builderSection);
	void output();
	
private:
	bool isSourceFile() const;
	std::string getOutputFileName() const;
	const Project * getProject() const;
	
private:
	BuilderSectionListType sectionList;
	std::string sourceFileName;
	const BuilderContext * builderContext;
	int fileIndex;
};


} // namespace metagen



#endif
