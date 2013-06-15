#ifndef __CODEBLOCK_H
#define __CODEBLOCK_H

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
	virtual void write(CodeWriter * codeWriter);

	void append(const std::string & code);

private:
	std::string code;

	friend class CodeBlock;
};


class CodeBlock : public CodeItem
{
private:
	typedef CodeItem super;
	
	typedef std::vector<CodeItem * > CodeListType;
	typedef std::map<std::string, CodeBlock * > NamedBlockMapType;
	
protected:
	CodeBlock();
	~CodeBlock();
	
public:
	virtual void write(CodeWriter * codeWriter);

	CodeLine * addLine();
	CodeLine * addLine(const std::string & code);
	CodeBlock * addBlockWithoutBracket();
	CodeBlock * addBlock();
	CodeBlock * getNamedBlock(const std::string & name);
	
	void setUseBracket(bool useBracket);
	void setIndentBlock(bool indentBlock);

private:
	bool withBracket;
	bool indentBlock;
	CodeListType codeList;
	NamedBlockMapType namedBlocks;
	
	friend class CppWriter;
};



#endif
