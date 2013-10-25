#ifndef CPGF_CODEWRITER_H
#define CPGF_CODEWRITER_H

#include <string>

namespace metagen {


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
	void ensureBlankLine();
	
	const std::string & getText() const;

private:
	std::string getIndentText() const;
	
private:
	int indent;
	std::string text;
};


} // namespace metagen


#endif
