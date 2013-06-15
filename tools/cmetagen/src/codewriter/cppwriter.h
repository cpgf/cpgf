#ifndef __CPPWRITER_H
#define __CPPWRITER_H

#include "codeblock.h"

#include <string>
#include <vector>


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
	
	CodeBlock * getWrapperBlock();
	CodeBlock * getMainBlock();

private:
	void addToStringList(StringListType * stringList, const std::string & s);

private:
	std::string fileName;
	std::string fileNamespace;
	StringListType includeList;
	StringListType usedNamespaceList;
	StringListType tailIncludeList;
	CodeBlock wrapperBlock;
	CodeBlock mainBlock;
};

struct CppPairWriter
{
	CppPairWriter(CppWriter * headerWriter, CppWriter * sourceWriter) : headerWriter(headerWriter), sourceWriter(sourceWriter) {}

	CppWriter * headerWriter;
	CppWriter * sourceWriter;
};


#endif
