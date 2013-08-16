#ifndef __BUILDERFILEWRITER_H
#define __BUILDERFILEWRITER_H

#include "buildersection.h"
#include "codewriter/cppwriter.h"

#include <string>
#include <vector>

namespace metagen {

class BuilderContext;
class Project;
class CodeWriter;
class CppWriter;

class BuilderFileWriter
{
private:
	BuilderFileWriter(const std::string & sourceFileName, const BuilderContext * builderContext, int fileIndex);
	
public:
	static BuilderFileWriter * createHeaderFile(const std::string & sourceFileName, const BuilderContext * builderContext);
	static BuilderFileWriter * createSourceFile(const std::string & sourceFileName, const BuilderContext * builderContext, int fileIndex);
	
public:
	void setCreationFunctionNameCode(const std::string & code);
	void addSection(BuilderSection * builderSection);
	void output();
	
private:
	bool isSourceFile() const;
	const Project * getProject() const;
	void callbackCppWriter(CodeWriter * codeWriter, CppWriterStage stage) const;
	void initializeCppWriter(CppWriter * cppWriter) const;
	
private:
	BuilderSectionListType sectionList;
	std::string sourceFileName;
	const BuilderContext * builderContext;
	int fileIndex;
	std::string creationFunctionNameCode;
};


} // namespace metagen



#endif
