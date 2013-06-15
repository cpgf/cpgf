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


CppCodeItem::CppCodeItem()
	: indent(0)
{
}

CppCodeItem::~CppCodeItem()
{
}

int CppCodeItem::getIndent() const
{
	return this->indent;
}

void CppCodeItem::incIndent()
{
	++this->indent;
}

void CppCodeItem::decIndent()
{
	if(this->indent > 0) {
		--this->indent;
	}
}


CppCodeLine::CppCodeLine(const std::string & code)
	: super(), code(code)
{
}

void CppCodeLine::write(CodeWriter * codeWriter)
{
	codeWriter->incIndent(this->getIndent());

	codeWriter->writeLine(this->code);
	
	codeWriter->decIndent(this->getIndent());
}

void CppCodeLine::append(const std::string & code)
{
	this->code.append(code);
}


CppCodeBlock::CppCodeBlock()
	: super(), withBracket(true), indentBlock(true)
{
}

CppCodeBlock::~CppCodeBlock()
{
	clearPointerContainer(this->codeList);
}

void CppCodeBlock::write(CodeWriter * codeWriter)
{
	codeWriter->incIndent(this->getIndent());
	
	if(this->withBracket) {
		codeWriter->writeLine("{");
	}
	
	if(this->indentBlock) {
		codeWriter->incIndent();
	}

	for(CodeListType::iterator it = this->codeList.begin(); it != this->codeList.end(); ++it) {
		(*it)->write(codeWriter);
	}

	if(this->indentBlock) {
		codeWriter->decIndent();
	}

	if(this->withBracket) {
		codeWriter->writeLine("}");
	}
	
	codeWriter->decIndent(this->getIndent());
}

CppCodeLine * CppCodeBlock::addLine()
{
	return this->addLine("");
}

CppCodeLine * CppCodeBlock::addLine(const std::string & code)
{
	CppCodeLine * line = new CppCodeLine(code);
	this->codeList.push_back(line);
	return line;
}

CppCodeBlock * CppCodeBlock::addBlockWithoutBracket()
{
	CppCodeBlock * block = new CppCodeBlock();
	this->codeList.push_back(block);
	block->setUseBracket(false);
	return block;
}

CppCodeBlock * CppCodeBlock::addBlock()
{
	CppCodeBlock * block = new CppCodeBlock();
	this->codeList.push_back(block);
	block->setUseBracket(true);
	return block;
}

CppCodeBlock * CppCodeBlock::getNamedBlock(const std::string & name)
{
	NamedBlockMapType::iterator it = this->namedBlocks.find(name);
	if(it == this->namedBlocks.end()) {
		CppCodeBlock * block = new CppCodeBlock();
		this->codeList.push_back(block);
		this->namedBlocks.insert(make_pair(name, block));
		block->setUseBracket(false);
		block->setIndentBlock(false);
		return block;
	}
	else {
		return it->second;
	}
}

void CppCodeBlock::setUseBracket(bool useBracket)
{
	this->withBracket = useBracket;
}

void CppCodeBlock::setIndentBlock(bool indentBlock)
{
	this->indentBlock = indentBlock;
}


CppWriter::CppWriter(const std::string & fileName)
	: fileName(fileName), wrapperBlock(), mainBlock()
{
	wrapperBlock.setUseBracket(false);
	mainBlock.setUseBracket(false);
	wrapperBlock.setIndentBlock(false);
	mainBlock.setIndentBlock(false);
}

CppWriter::~CppWriter()
{
}

void writeIncludeList(const std::vector<std::string> & includeList, CodeWriter * codeWriter)
{
	for(std::vector<std::string>::const_iterator it = includeList.begin(); it != includeList.end(); ++it) {
		codeWriter->writeLine("#include \"" + *it + "\"");
	}
	if(! includeList.empty()) {
		codeWriter->writeBlankLine();
	}
}

void CppWriter::write(CodeWriter * codeWriter)
{
	codeWriter->writeLine("// Auto generated file, don't modify.");
	codeWriter->writeBlankLine();

	string headerGuard;
	Poco::Path path(this->fileName);
	string extension = path.getExtension();
	if(! extension.empty() && extension != "cpp") {
		headerGuard = "__" + path.getBaseName() + "_" + extension;
		Poco::toUpperInPlace(headerGuard);
	}

	if(! headerGuard.empty()) {
		codeWriter->writeLine("#ifndef " + headerGuard);
		codeWriter->writeLine("#define " + headerGuard);
		codeWriter->writeBlankLine();
	}

	writeIncludeList(this->includeList, codeWriter);

	for(StringListType::iterator it = this->usedNamespaceList.begin(); it != this->usedNamespaceList.end(); ++it) {
		codeWriter->writeLine("using namespace " + *it + ";");
	}
	if(! this->usedNamespaceList.empty()) {
		codeWriter->writeBlankLine();
	}

	if(! this->fileNamespace.empty()) {
		codeWriter->writeLine("namespace " + this->fileNamespace + "{");
		codeWriter->writeBlankLine();
	}

	this->wrapperBlock.write(codeWriter);
	this->mainBlock.write(codeWriter);

	if(! this->fileNamespace.empty()) {
		codeWriter->writeBlankLine();
		codeWriter->writeLine("} // namespace " + this->fileNamespace);
	}

	writeIncludeList(this->tailIncludeList, codeWriter);

	if(! headerGuard.empty()) {
		codeWriter->writeBlankLine();
		codeWriter->writeLine("#endif");
	}
}

void CppWriter::setNamespace(const std::string & ns)
{
	this->fileNamespace = ns;
}

void CppWriter::useNamespace(const std::string & ns)
{
	this->addToStringList(&this->usedNamespaceList, ns);
}

void CppWriter::include(const std::string & fileName)
{
	this->addToStringList(&this->includeList, fileName);
}

void CppWriter::tailIncldue(const std::string & fileName)
{
	this->addToStringList(&this->tailIncludeList, fileName);
}

CppCodeBlock * CppWriter::getWrapperBlock()
{
	return &this->wrapperBlock;
}

CppCodeBlock * CppWriter::getMainBlock()
{
	return &this->mainBlock;
}

void CppWriter::addToStringList(StringListType * stringList, const std::string & s)
{
	if(std::find(stringList->begin(), stringList->end(), s) == stringList->end()) {
		stringList->push_back(s);
	}
}
