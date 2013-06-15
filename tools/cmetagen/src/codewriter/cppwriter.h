#ifndef __CPPWRITER_H
#define __CPPWRITER_H

#include <string>
#include <vector>
#include <map>


/********************************
A CppWriter represents a C++ source/header file.
CppWriter is structured.
The file structure:

Header guard begin, it this is a header file

List of #include

using namespace list, if any

namespace begin {, if any

code block wrapper, used for generate wrapper code for operators and bit fields
code block main, used for generate main reflection code

} // namespace end, if any

Tail #include list, if any

Header guard end, it this is a header file
********************************/

class CodeWriter;

class CppCodeItem
{
protected:
	CppCodeItem();

	int getIndent() const;

public:	
	virtual ~CppCodeItem();
	
	virtual void write(CodeWriter * codeWriter) = 0;

	void incIndent();
	void decIndent();
	
private:
	int indent;
};


class CppCodeLine : public CppCodeItem
{
private:
	typedef CppCodeItem super;
	
protected:
	explicit CppCodeLine(const std::string & code);
	
public:
	virtual void write(CodeWriter * codeWriter);

	void append(const std::string & code);

private:
	std::string code;

	friend class CppCodeBlock;
};


class CppCodeBlock : public CppCodeItem
{
private:
	typedef CppCodeItem super;
	
	typedef std::vector<CppCodeItem * > CodeListType;
	typedef std::map<std::string, CppCodeBlock * > NamedBlockMapType;
	
protected:
	CppCodeBlock();
	~CppCodeBlock();
	
public:
	virtual void write(CodeWriter * codeWriter);

	CppCodeLine * addLine();
	CppCodeLine * addLine(const std::string & code);
	CppCodeBlock * addBlockWithoutBracket();
	CppCodeBlock * addBlock();
	CppCodeBlock * getNamedBlock(const std::string & name);
	
	void setUseBracket(bool useBracket);
	void setIndentBlock(bool indentBlock);

private:
	bool withBracket;
	bool indentBlock;
	CodeListType codeList;
	NamedBlockMapType namedBlocks;
	
	friend class CppWriter;
};


class CppWriter
{
private:
	typedef std::vector<std::string> StringListType;
	
public:
	explicit CppWriter(const std::string & fileName);
	~CppWriter();
	
	void write(CodeWriter * codeWriter);
	
	void setNamespace(const std::string & ns);
	void useNamespace(const std::string & ns);
	void include(const std::string & fileName);
	void tailIncldue(const std::string & fileName);
	
	CppCodeBlock * getWrapperBlock();
	CppCodeBlock * getMainBlock();

private:
	void addToStringList(StringListType * stringList, const std::string & s);

private:
	std::string fileName;
	std::string fileNamespace;
	StringListType includeList;
	StringListType usedNamespaceList;
	StringListType tailIncludeList;
	CppCodeBlock wrapperBlock;
	CppCodeBlock mainBlock;
};

struct CppPairWriter
{
	CppPairWriter(CppWriter * headerWriter, CppWriter * sourceWriter) : headerWriter(headerWriter), sourceWriter(sourceWriter) {}

	CppWriter * headerWriter;
	CppWriter * sourceWriter;
};


#endif
