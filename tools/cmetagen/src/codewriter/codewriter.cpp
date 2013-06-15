#include "codewriter.h"


CodeWriter::CodeWriter()
	: indent(0)
{
}

CodeWriter::~CodeWriter()
{
}

std::string CodeWriter::getIndentText() const
{
	std::string indentText;
	for(int i = 0; i < this->indent; ++i) {
		indentText = indentText + "    "; 
	}
	
	return indentText;
}

void CodeWriter::setIndent(int indent)
{
	this->indent = indent;
}

void CodeWriter::incIndent()
{
	++this->indent;
}

void CodeWriter::decIndent()
{
	if(this->indent > 0) {
		--this->indent;
	}
}

void CodeWriter::incIndent(int level)
{
	this->indent = level;
}

void CodeWriter::decIndent(int level)
{
	this->indent -= level;
	if(this->indent < 0) {
		this->indent = 0;
	}
}

void CodeWriter::write(const std::string & s)
{
	if(s.empty()) {
		return;
	}

	std::string indentText = this->getIndentText();

	if(! this->text.empty() && this->text[this->text.size() - 1] == '\n') {
		this->text = this->text + indentText;
	}

	this->text = this->text + s;
}

void CodeWriter::writeLine(const std::string & s)
{
	this->write(s + "\n");
}

void CodeWriter::writeBlankLine()
{
	this->write("\n");
}

const std::string & CodeWriter::getText() const
{
	return this->text;
}

