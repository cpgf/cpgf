#ifndef CPGF_CODEBLOCK_H
#define CPGF_CODEBLOCK_H

#include "cpgf/gflags.h"

#include <string>
#include <vector>
#include <map>

namespace metagen {


class CodeWriter;

class CodeItem
{
protected:
	CodeItem();

	int getIndent() const;

public:	
	virtual ~CodeItem();
	
	virtual void write(CodeWriter * codeWriter) = 0;

	virtual bool isLine() const { return false; }
	bool isBlock() const { return ! this->isLine(); }

	void incIndent();
	void decIndent();
	
	void setIndent(int indent);
	
private:
	int indent;
};


class CodeLine : public CodeItem
{
private:
	typedef CodeItem super;
	
protected:
	explicit CodeLine(const std::string & code);
	
public:
	const std::string & getCode() const { return this->code; }

	virtual bool isLine() const { return true; }

	virtual void write(CodeWriter * codeWriter);

	void append(const std::string & code);

private:
	std::string code;

	friend class CodeBlock;
};

enum CodeBlockStyle {
	cbsBracket = 1 << 0,
	cbsIndent = 1 << 1,
	cbsTailEmptyLine = 1 << 2,
	cbsBracketWithSemicolon = 1 << 3,
	cbsBracketAndIndent = cbsBracket | cbsIndent
};

class CodeBlock : public CodeItem
{
private:
	typedef CodeItem super;
	
	typedef std::vector<CodeItem * > CodeListType;
	typedef std::map<std::string, CodeBlock * > NamedBlockMapType;
	
public:
	CodeBlock();
	virtual ~CodeBlock();
	
public:
	virtual void write(CodeWriter * codeWriter);

	CodeLine * appendLine(const std::string & code);
	CodeLine * appendUniqueLine(const std::string & code);
	CodeLine * appendBlankLine();
	CodeBlock * appendBlock(const cpgf::GFlags<CodeBlockStyle> & flags = 0);
	CodeBlock * getNamedBlock(const std::string & name, const cpgf::GFlags<CodeBlockStyle> & flags = 0);
	void ensureNamedBlocks(const std::string & name1, const std::string & name2);
	
	void setFlags(const cpgf::GFlags<CodeBlockStyle> & flags);

private:
	cpgf::GFlags<CodeBlockStyle> flags;
	CodeListType codeList;
	NamedBlockMapType namedBlocks;
};


} // namespace metagen


#endif
