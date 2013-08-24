#include "cppwriter.h"
#include "codewriter.h"
#include "util.h"
#include "constants.h"

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
{
}

CppWriter::~CppWriter()
{
}

void writeIncludeList(const std::vector<std::string> & includeList, CodeWriter * codeWriter)
{
	std::set<std::string> writtenList;
	for(std::vector<std::string>::const_iterator it = includeList.begin(); it != includeList.end(); ++it) {
		const string & include = *it;
		if(writtenList.find(include) == writtenList.end()) {
			writtenList.insert(include);
			if(include.at(0) == '@') {
				codeWriter->writeLine(include.c_str() + 1);
			}
			else {
				codeWriter->writeLine("#include \"" + include + "\"");
			}
		}
	}
	if(! includeList.empty()) {
		codeWriter->ensureBlankLine();
	}
}

void CppWriter::write(CodeWriter * codeWriter, const CppWriterCallback & callback)
{
	codeWriter->writeLine(GeneratedFileMark);
	codeWriter->writeLine("// Auto generated file, don't modify.");
	if(callback) {
		callback(codeWriter, cwsBeginning);
	}
	codeWriter->ensureBlankLine();

	if(! this->headerGuard.empty()) {
		string guard;
		Poco::Path path(this->headerGuard);
		guard = "METADATA_" + path.getBaseName() + "_" + path.getExtension();
		Poco::toUpperInPlace(guard);
		guard = normalizeSymbolName(guard);
		codeWriter->writeLine("#ifndef " + guard);
		codeWriter->writeLine("#define " + guard);
		codeWriter->ensureBlankLine();
	}

	writeIncludeList(this->includeList, codeWriter);

	for(std::set<std::string>::iterator it = this->usedNamespaceList.begin();
		it != this->usedNamespaceList.end();
		++it) {
		codeWriter->writeLine("using namespace " + *it + ";");
	}
	if(! this->usedNamespaceList.empty()) {
		codeWriter->ensureBlankLine();
	}

	if(! this->fileNamespace.empty()) {
		codeWriter->writeLine("namespace " + this->fileNamespace + " {");
		codeWriter->ensureBlankLine();
	}

	if(callback) {
		callback(codeWriter, cwsMainCodeBlock);
	}
	else {
		this->codeBlock.write(codeWriter);
	}

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
	this->includeList.push_back(fileName);
}

void CppWriter::tailInclude(const std::string & fileName)
{
	this->tailIncludeList.push_back(fileName);
}


} // namespace metagen
