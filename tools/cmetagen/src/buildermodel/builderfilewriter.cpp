#include "builderfilewriter.h"
#include "buildercontext.h"
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

BuilderFileWriter::BuilderFileWriter(const std::string & sourceFileName, const BuilderContext * builderContext, int fileIndex)
	: sourceFileName(sourceFileName), builderContext(builderContext), fileIndex(fileIndex)
{
}
	
BuilderFileWriter * BuilderFileWriter::createHeaderFile(const std::string & sourceFileName,
	const BuilderContext * builderContext)
{
	return new BuilderFileWriter(sourceFileName, builderContext, -1);
}

BuilderFileWriter * BuilderFileWriter::createSourceFile(const std::string & sourceFileName,
	const BuilderContext * builderContext, int fileIndex)
{
	return new BuilderFileWriter(sourceFileName, builderContext, fileIndex);
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
		string header = Poco::Path(this->getProject()->getOutputHeaderFileName(this->sourceFileName)).getFileName();
		cppWriter->include(normalizeFile(this->getProject()->getHeaderIncludePrefix() + header));
		cppWriter->useNamespace("cpgf");
	}
	else {
		cppWriter->setHeaderGuard(this->getProject()->getOutputHeaderFileName(this->sourceFileName));

		string header = this->getProject()->replaceHeaderByScript(this->sourceFileName);
		cppWriter->include(normalizeFile(header));
		cppWriter->include(includeMetaDefine);
		cppWriter->include(includeMetaPolicy);
		cppWriter->include(includeMetaDataHeader);
		cppWriter->tailInclude(includeMetaDataFooter);

		cppWriter->include(includeScriptBindUtil);
		cppWriter->include(includeScriptWrapper);
		cppWriter->include(includeScopedInterface);
		cppWriter->include(includeSelectFunctionByArity);
	}
}

void BuilderFileWriter::output()
{
	sortSectionList(&this->sectionList);

	string outputFileName = this->isSourceFile() ?
		this->getProject()->getOutputSourceFileName(this->sourceFileName, this->fileIndex)
		: this->getProject()->getOutputHeaderFileName(this->sourceFileName);
	Poco::File(Poco::Path(outputFileName).parent()).createDirectories();

	CppWriter cppWriter;
	CodeWriter codeWriter;

	this->initializeCppWriter(&cppWriter);
	cppWriter.write(&codeWriter, makeCallback(this, &BuilderFileWriter::callbackCppWriter));

	writeStringToFile(outputFileName, codeWriter.getText());
}


} // namespace metagen

