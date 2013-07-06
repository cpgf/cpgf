#ifndef __CODEBLOCK_H
#define __CODEBLOCK_H

#include "cpgf/gflags.h"

#include <string>
#include <vector>
#include <map>


class CodeWriter;

class CodeItem
{
protected:
	CodeItem();

	int getIndent() const;

public:	
	virtual ~CodeItem();
	
	virtual void write(CodeWriter * codeWriter) = 0;

	void incIndent();
	void decIndent();
	
	void setIndent(int indent);
	
private:
	int indent;
};

class CodeIndent
{
public:
	explicit CodeIndent(CodeItem * codeItem) : codeItem(codeItem) { codeItem->incIndent(); }
	~CodeIndent() { codeItem->decIndent(); }
	
private:
	CodeItem * codeItem;
};


class CodeLine : public CodeItem
{
private:
	typedef CodeItem super;
	
protected:
	explicit CodeLine(const std::string & code);
	
public:
	virtual void write(CodeWriter * codeWriter);

	void append(const std::string & code);

private:
	std::string code;

	friend class CodeBlock;
};

enum CodeBlockStyle {
	cbsBracket = 1 << 0,
	cbsIndent = 1 << 1,
	cbsBracketAndIndent = cbsBracket | cbsIndent
};

class CodeBlock : public CodeItem
{
private:
	typedef CodeItem super;
	
	typedef std::vector<CodeItem * > CodeListType;
	typedef std::map<std::string, CodeBlock * > NamedBlockMapType;
	
protected:
	CodeBlock();
	virtual ~CodeBlock();
	
public:
	virtual void write(CodeWriter * codeWriter);

	CodeLine * addLine();
	CodeLine * addLine(const std::string & code);
	CodeBlock * addBlock(const cpgf::GFlags<CodeBlockStyle> & flags = 0);
	CodeBlock * getNamedBlock(const std::string & name, const cpgf::GFlags<CodeBlockStyle> & flags = 0);
	void ensureNamedBlocks(const std::string & name1, const std::string & name2);
	
	void setFlags(const cpgf::GFlags<CodeBlockStyle> & flags);

private:
	cpgf::GFlags<CodeBlockStyle> flags;
	CodeListType codeList;
	NamedBlockMapType namedBlocks;
	
	friend class CppWriter;
};



#endif
