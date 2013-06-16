#include "codeblock.h"
#include "codewriter.h"
#include "util.h"


using namespace std;

CodeItem::CodeItem()
	: indent(0)
{
}

CodeItem::~CodeItem()
{
}

int CodeItem::getIndent() const
{
	return this->indent;
}

void CodeItem::setIndent(int indent)
{
	this->indent = indent;
}

void CodeItem::incIndent()
{
	++this->indent;
}

void CodeItem::decIndent()
{
	if(this->indent > 0) {
		--this->indent;
	}
}


CodeLine::CodeLine(const std::string & code)
	: super(), code(code)
{
}

void CodeLine::write(CodeWriter * codeWriter)
{
	codeWriter->incIndent(this->getIndent());

	codeWriter->writeLine(this->code);
	
	codeWriter->decIndent(this->getIndent());
}

void CodeLine::append(const std::string & code)
{
	this->code.append(code);
}


CodeBlock::CodeBlock()
	: super(), withBracket(true), indentBlock(true)
{
}

CodeBlock::~CodeBlock()
{
	clearPointerContainer(this->codeList);
}

void CodeBlock::write(CodeWriter * codeWriter)
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

	if(this->withBracket) {
		codeWriter->writeBlankLine();
	}
}

CodeLine * CodeBlock::addLine()
{
	return this->addLine("");
}

CodeLine * CodeBlock::addLine(const std::string & code)
{
	CodeLine * line = new CodeLine(code);
	this->codeList.push_back(line);
	line->setIndent(this->getIndent());
	return line;
}

CodeBlock * CodeBlock::addBlock(CodeBlockBracket bracket, CodeBlockIndent indent)
{
	CodeBlock * block = new CodeBlock();
	block->setIndent(this->getIndent());
	this->codeList.push_back(block);
	block->setUseBracket(bracket == cbbBracket);
	block->setIndentBlock(indent == cbiIndent);
	return block;
}

CodeBlock * CodeBlock::getNamedBlock(const std::string & name, CodeBlockBracket bracket, CodeBlockIndent indent)
{
	NamedBlockMapType::iterator it = this->namedBlocks.find(name);
	if(it == this->namedBlocks.end()) {
		CodeBlock * block = new CodeBlock();
		block->setIndent(this->getIndent());
		this->codeList.push_back(block);
		this->namedBlocks.insert(make_pair(name, block));
		block->setUseBracket(bracket == cbbBracket);
		block->setIndentBlock(indent == cbiIndent);
		return block;
	}
	else {
		return it->second;
	}
}

void CodeBlock::setUseBracket(bool useBracket)
{
	this->withBracket = useBracket;
}

void CodeBlock::setIndentBlock(bool indentBlock)
{
	this->indentBlock = indentBlock;
}

