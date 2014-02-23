#include "builderfilewriter.h"
#include "buildercontext.h"
#include "model/cppsourcefile.h"
#include "codewriter/codewriter.h"
#include "codewriter/codeblock.h"
#include "project.h"
#include "util.h"
#include "constants.h"

#include "Poco/Format.h"
#include "Poco/Path.h"
#include "Poco/File.h"

using namespace std;
using namespace cpgf;

namespace metagen {

BuilderFileWriter::BuilderFileWriter(const CppSourceFile & sourceFile,
	const BuilderContext * builderContext, int fileIndex)
	: sourceFile(sourceFile), builderContext(builderContext), fileIndex(fileIndex)
{
}
	
BuilderFileWriter * BuilderFileWriter::createHeaderFile(const CppSourceFile & sourceFile,
	const BuilderContext * builderContext)
{
	return new BuilderFileWriter(sourceFile, builderContext, -1);
}

BuilderFileWriter * BuilderFileWriter::createSourceFile(const CppSourceFile & sourceFile,
	const BuilderContext * builderContext, int fileIndex)
{
	return new BuilderFileWriter(sourceFile, builderContext, fileIndex);
}

void BuilderFileWriter::setCreationFunctionNameCode(const std::string & code)
{
	this->creationFunctionNameCode = code;
}

void BuilderFileWriter::addSection(BuilderSection * builderSection)
{
	this->sectionList.push_back(builderSection);
}

bool BuilderFileWriter::isSourceFile() const
{
	return this->fileIndex >= 0;
}

const Project * BuilderFileWriter::getProject() const
{
	return this->builderContext->getProject();
}

void BuilderFileWriter::callbackCppWriter(CodeWriter * codeWriter, CppWriterStage stage) const
{
	switch(stage) {
		case cwsBeginning:
			if(! this->isSourceFile() && ! this->creationFunctionNameCode.empty()) {
				codeWriter->ensureBlankLine();
				codeWriter->writeLine(GeneratedCreationFunctionBeginMark);
				codeWriter->writeLine(this->creationFunctionNameCode);
				codeWriter->writeLine(GeneratedCreationFunctionEndMark);
			}
			break;

		case cwsMainCodeBlock:
			for(BuilderSectionListType::const_iterator it = this->sectionList.begin(); it != this->sectionList.end(); ++it) {
				(*it)->getCodeBlock()->write(codeWriter);
			}
			break;
			
		default:
			break;
	}
}

void BuilderFileWriter::initializeCppWriter(CppWriter * cppWriter) const
{
	cppWriter->setNamespace(this->getProject()->getCppNamespace());

	if(this->isSourceFile()) {
		string header = Poco::Path(this->getProject()->getOutputHeaderFileName(this->sourceFile.getFileName())).getFileName();
		cppWriter->include(normalizeFile(this->getProject()->getHeaderIncludePrefix() + header));
		cppWriter->useNamespace("cpgf");
	}
	else {
		cppWriter->setHeaderGuard(this->getProject()->getOutputHeaderFileName(this->sourceFile.getFileName()));

		for(vector<string>::const_iterator it = this->sourceFile.getIncludeList().begin();
			it != this->sourceFile.getIncludeList().end();
			++it) {
			cppWriter->include(*it);
		}

		for(vector<string>::const_iterator it = this->sourceFile.getMetaIncludeList().begin();
			it != this->sourceFile.getMetaIncludeList().end();
			++it) {
			cppWriter->include(*it);
		}

		string header = this->getProject()->replaceHeaderByScript(this->sourceFile.getFileName());
		cppWriter->include(normalizeFile(header));

		for(int i = 0; metaHeaderIncludeList[i] != NULL; ++i) {
			cppWriter->include(metaHeaderIncludeList[i]);
		}

		cppWriter->tailInclude(includeMetaDataFooter);
	}
}

void BuilderFileWriter::output()
{
	sortSectionList(&this->sectionList);

	string outputFileName = this->isSourceFile() ?
		this->getProject()->getOutputSourceFileName(this->sourceFile.getFileName(), this->fileIndex)
		: this->getProject()->getOutputHeaderFileName(this->sourceFile.getFileName());
	Poco::File(Poco::Path(outputFileName).parent()).createDirectories();

	CppWriter cppWriter;
	CodeWriter codeWriter;

	this->initializeCppWriter(&cppWriter);
	cppWriter.write(&codeWriter, makeCallback(this, &BuilderFileWriter::callbackCppWriter));

	if(! this->builderContext->shouldOverwriteEvenIfNoChange()) {
		string fileContent;
		if(readStringFromFile(outputFileName, &fileContent)) {
			if(codeWriter.getText() == fileContent) {
				return;
			}
		}
	}

	writeStringToFile(outputFileName, codeWriter.getText());
}


} // namespace metagen

