#include "builderfilewriter.h"
#include "buildercontext.h"
#include "codewriter/codewriter.h"
#include "codewriter/cppwriter.h"
#include "codewriter/codeblock.h"
#include "project.h"
#include "util.h"

#include "Poco/Format.h"
#include "Poco/Path.h"
#include "Poco/File.h"

using namespace std;
using namespace cpgf;

namespace metagen {

BuilderFileWriter::BuilderFileWriter(const std::string & sourceFileName, const BuilderContext * builderContext, int fileIndex)
	: sourceFileName(sourceFileName), builderContext(builderContext), fileIndex(fileIndex)
{
}
	
BuilderFileWriter * BuilderFileWriter::createHeaderFile(const std::string & sourceFileName, const BuilderContext * builderContext)
{
	return new BuilderFileWriter(sourceFileName, builderContext, -1);
}

BuilderFileWriter * BuilderFileWriter::createSourceFile(const std::string & sourceFileName, const BuilderContext * builderContext, int fileIndex)
{
	return new BuilderFileWriter(sourceFileName, builderContext, fileIndex);
}

void BuilderFileWriter::addSection(BuilderSection * builderSection)
{
	this->sectionList.push_back(builderSection);
}

bool BuilderFileWriter::isSourceFile() const
{
	return this->fileIndex >= 0;
}

std::string BuilderFileWriter::getOutputFileName() const
{
	string postfix = "";
	if(this->fileIndex > 0) {
		postfix = Poco::format("_%d", this->fileIndex);
	}
	if(this->getProject()->shouldIncludeExtensionInFileName()) {
		postfix = Poco::format("%s%s", Poco::Path(this->sourceFileName).getExtension(), postfix);
	}
	string extension;
	if(this->isSourceFile()) {
		extension = this->getProject()->getSourceFileExtension();
	}
	else {
		extension = this->getProject()->getHeaderFileExtension();
	}
	if(! extension.empty() && extension[0] != '.') {
		extension = "." + extension;
	}

	string fileName = Poco::format("%s%s%s", Poco::Path(this->sourceFileName).getBaseName(), postfix, extension);
	string outputPath;
	if(this->isSourceFile()) {
		extension = this->getProject()->getSourceOutputPath();
	}
	else {
		extension = this->getProject()->getHeaderOutputPath();
	}
	return this->getProject()->getAbsoluteFileName(normalizePath(extension) + fileName);
}

const Project * BuilderFileWriter::getProject() const
{
	return this->builderContext->getProject();
}

void BuilderFileWriter::callbackCppWriter(CodeWriter * codeWriter) const
{
	for(BuilderSectionListType::const_iterator it = this->sectionList.begin(); it != this->sectionList.end(); ++it) {
		(*it)->getCodeBlock()->write(codeWriter);
	}
}

void BuilderFileWriter::output()
{
	string outputFileName = getOutputFileName();
	Poco::File(Poco::Path(outputFileName).parent()).createDirectories();

	CppWriter cppWriter;
	CodeWriter codeWriter;

	cppWriter.write(&codeWriter, makeCallback(this, &BuilderFileWriter::callbackCppWriter));

	string fileContent;
	if(readStringFromFile(outputFileName, &fileContent)) {
		if(codeWriter.getText() == fileContent) {
			return;
		}
	}

	writeStringToFile(outputFileName, codeWriter.getText());
}


} // namespace metagen

