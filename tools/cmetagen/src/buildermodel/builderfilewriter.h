#ifndef CPGF_BUILDERFILEWRITER_H
#define CPGF_BUILDERFILEWRITER_H

#include "buildersection.h"
#include "codewriter/cppwriter.h"
#include "cpgf/gclassutil.h"

#include <string>
#include <vector>

namespace metagen {

class BuilderContext;
class Project;
class CodeWriter;
class CppWriter;
class CppSourceFile;

class BuilderFileWriter
{
	GMAKE_NONCOPYABLE(BuilderFileWriter)

private:
	BuilderFileWriter(const CppSourceFile & sourceFile,
		const BuilderContext * builderContext, int fileIndex);
	
public:
	static BuilderFileWriter * createHeaderFile(const CppSourceFile & sourceFile,
		const BuilderContext * builderContext);
	static BuilderFileWriter * createSourceFile(const CppSourceFile & sourceFile,
		const BuilderContext * builderContext, int fileIndex);
	
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
	const CppSourceFile & sourceFile;
	const BuilderContext * builderContext;
	int fileIndex;
	bool overwriteEvenIfNoChange;
	std::string creationFunctionNameCode;
};


} // namespace metagen



#endif
