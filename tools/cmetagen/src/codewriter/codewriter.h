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
	
	void write(const std::string & text);
	
	const std::string & getText() const;

private:
	std::string getIndentText() const;
	
private:
	int indent;
	std::string text;
};


#endif
