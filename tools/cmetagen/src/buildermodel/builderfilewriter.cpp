#include "builderfilewriter.h"
#include "buildercontext.h"
#include "codewriter/codeblock.h"
#include "codewriter/codewriter.h"
#include "project.h"

#include "Poco/Format.h"
#include "Poco/Path.h"

using namespace std;

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
	return this->getProject()->getAbsoluteFileName(fileName);
}

const Project * BuilderFileWriter::getProject() const
{
	return this->builderContext->getProject();
}

void BuilderFileWriter::output()
{
	printf("\nFile %s\n", getOutputFileName().c_str());

	for(BuilderSectionListType::iterator it = this->sectionList.begin(); it != this->sectionList.end(); ++it) {
		CodeWriter codeWriter;
		(*it)->getCodeBlock()->write(&codeWriter);
		printf("%s", codeWriter.getText().c_str());
	}
}


} // namespace metagen

