#ifndef __CPPWRITER_H
#define __CPPWRITER_H

#include "codeblock.h"

#include <string>
#include <set>


/********************************
A CppWriter represents a C++ source/header file.
CppWriter is structured.
The file structure:

Header guard begin, it this is a header file

List of #include

using namespace list, if any

namespace begin {, if any

code

} // namespace end, if any

Tail #include list, if any

Header guard end, it this is a header file
********************************/

namespace metagen {


class CodeWriter;

class CppWriter
{
private:
	typedef std::set<std::string> StringSetType;
	
public:
	explicit CppWriter();
	~CppWriter();
	
	void write(CodeWriter * codeWriter);
	
	void setHeaderGuard(const std::string & headerGuard);
	void setNamespace(const std::string & ns);
	void useNamespace(const std::string & ns);
	void include(const std::string & fileName);
	void tailIncldue(const std::string & fileName);
	
	CodeBlock * getCodeBlock();

private:
	std::string headerGuard;
	std::string fileNamespace;
	StringSetType includeList;
	StringSetType usedNamespaceList;
	StringSetType tailIncludeList;
	CodeBlock codeBlock;
};


} // namespace metagen


#endif
