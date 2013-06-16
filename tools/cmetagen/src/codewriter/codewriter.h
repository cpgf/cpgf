#ifndef __CODEWRITER_H
#define __CODEWRITER_H

#include <string>


class CodeWriter
{
public:
	CodeWriter();
	~CodeWriter();

	void incIndent();
	void decIndent();
	void incIndent(int level);
	void decIndent(int level);
	
	void write(const std::string & text);
	void writeLine(const std::string & text);
	void writeBlankLine();
	
	const std::string & getText() const;

private:
	std::string getIndentText() const;
	
private:
	int indent;
	std::string text;
};


#endif
