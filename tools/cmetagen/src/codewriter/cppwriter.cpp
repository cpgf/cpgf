#include "cppwriter.h"
#include "codewriter.h"
#include "util.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4244)
#endif

#include "Poco/Path.h"
#include "Poco/String.h"

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#include <algorithm>


using namespace std;

namespace metagen {


CppWriter::CppWriter()
	: codeBlock()
{
}

CppWriter::~CppWriter()
{
}

void writeIncludeList(const std::set<std::string> & includeList, CodeWriter * codeWriter)
{
	for(std::set<std::string>::const_iterator it = includeList.begin(); it != includeList.end(); ++it) {
		codeWriter->writeLine("#include \"" + *it + "\"");
	}
	if(! includeList.empty()) {
		codeWriter->ensureBlankLine();
	}
}

void CppWriter::write(CodeWriter * codeWriter)
{
	codeWriter->writeLine("// Auto generated file, don't modify.");
	codeWriter->ensureBlankLine();

	if(! this->headerGuard.empty()) {
		string guard;
		Poco::Path path(this->headerGuard);
		guard = "__" + path.getBaseName() + "_" + path.getExtension();
		Poco::toUpperInPlace(guard);
		codeWriter->writeLine("#ifndef " + guard);
		codeWriter->writeLine("#define " + guard);
		codeWriter->ensureBlankLine();
	}

	writeIncludeList(this->includeList, codeWriter);

	for(StringSetType::iterator it = this->usedNamespaceList.begin(); it != this->usedNamespaceList.end(); ++it) {
		codeWriter->writeLine("using namespace " + *it + ";");
	}
	if(! this->usedNamespaceList.empty()) {
		codeWriter->ensureBlankLine();
	}

	if(! this->fileNamespace.empty()) {
		codeWriter->writeLine("namespace " + this->fileNamespace + "{");
		codeWriter->ensureBlankLine();
	}

	this->codeBlock.write(codeWriter);

	if(! this->fileNamespace.empty()) {
		codeWriter->ensureBlankLine();
		codeWriter->writeLine("} // namespace " + this->fileNamespace);
	}

	writeIncludeList(this->tailIncludeList, codeWriter);

	if(! this->headerGuard.empty()) {
		codeWriter->ensureBlankLine();
		codeWriter->writeLine("#endif");
	}
}

void CppWriter::setHeaderGuard(const std::string & headerGuard)
{
	this->headerGuard = headerGuard;
}

void CppWriter::setNamespace(const std::string & ns)
{
	this->fileNamespace = ns;
}

void CppWriter::useNamespace(const std::string & ns)
{
	this->usedNamespaceList.insert(ns);
}

void CppWriter::include(const std::string & fileName)
{
	this->includeList.insert(fileName);
}

void CppWriter::tailIncldue(const std::string & fileName)
{
	this->tailIncludeList.insert(fileName);
}

CodeBlock * CppWriter::getCodeBlock()
{
	return &this->codeBlock;
}


} // namespace metagen
