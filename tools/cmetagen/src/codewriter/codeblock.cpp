#include "codeblock.h"
#include "codewriter.h"
#include "util.h"

using namespace std;

namespace metagen {


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
	: super(), flags()
{
}

CodeBlock::~CodeBlock()
{
	clearPointerContainer(this->codeList);
}

void CodeBlock::write(CodeWriter * codeWriter)
{
	codeWriter->incIndent(this->getIndent());
	
	if(this->flags.hasAny(cbsBracket | cbsBracketWithSemicolon)) {
		codeWriter->writeLine("{");
	}
	
	if(this->flags.has(cbsIndent)) {
		codeWriter->incIndent();
	}

	for(CodeListType::iterator it = this->codeList.begin(); it != this->codeList.end(); ++it) {
		(*it)->write(codeWriter);
	}

	if(this->flags.has(cbsIndent)) {
		codeWriter->decIndent();
	}

	if(this->flags.has(cbsBracket)) {
		codeWriter->writeLine("}");
	}
	else if(this->flags.has(cbsBracketWithSemicolon)) {
		codeWriter->writeLine("};");
	}
	
	codeWriter->decIndent(this->getIndent());

	if(this->flags.hasAny(cbsBracket | cbsBracketWithSemicolon) || this->flags.has(cbsTailEmptyLine)) {
		codeWriter->ensureBlankLine();
	}
}

CodeLine * CodeBlock::appendLine(const std::string & code)
{
	CodeLine * line = new CodeLine(code);
	this->codeList.push_back(line);
	line->setIndent(this->getIndent());
	return line;
}

CodeLine * CodeBlock::appendUniqueLine(const std::string & code)
{
	for(CodeListType::iterator it = this->codeList.begin(); it != this->codeList.end(); ++it) {
		if((*it)->isLine() && static_cast<CodeLine *>(*it)->getCode() == code) {
			return static_cast<CodeLine *>(*it);
		}
	}

	return this->appendLine(code);
}

CodeLine * CodeBlock::appendBlankLine()
{
	return this->appendLine("");
}

CodeBlock * CodeBlock::appendBlock(const cpgf::GFlags<CodeBlockStyle> & flags)
{
	CodeBlock * block = new CodeBlock();
	this->codeList.push_back(block);
	block->setIndent(this->getIndent());
	block->setFlags(flags);
	return block;
}

CodeBlock * CodeBlock::getNamedBlock(const std::string & name, const cpgf::GFlags<CodeBlockStyle> & flags)
{
	CodeBlock * block = NULL;
	NamedBlockMapType::iterator it = this->namedBlocks.find(name);
	if(it == this->namedBlocks.end()) {
		block = new CodeBlock();
		this->codeList.push_back(block);
		this->namedBlocks.insert(make_pair(name, block));
	}
	else {
		block = it->second;
	}
	block->setIndent(this->getIndent());
	block->setFlags(flags);
	return block;
}

void CodeBlock::ensureNamedBlocks(const std::string & name1, const std::string & name2)
{
	this->getNamedBlock(name1);
	this->getNamedBlock(name2);
}

void CodeBlock::setFlags(const cpgf::GFlags<CodeBlockStyle> & flags)
{
	this->flags = flags;
}


} // namespace metagen
