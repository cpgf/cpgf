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

const std::string & CodeWriter::getText() const
{
	return this->text;
}

