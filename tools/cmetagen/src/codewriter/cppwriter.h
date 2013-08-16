#ifndef __CPPWRITER_H
#define __CPPWRITER_H

#include "codeblock.h"
#include "cpgf/gcallback.h"

#include <string>
#include <set>


namespace metagen {


class CodeWriter;

enum CppWriterStage
{
	cwsBeginning,
	cwsMainCodeBlock
};

typedef cpgf::GCallback<void (CodeWriter * codeWriter, CppWriterStage stage)> CppWriterCallback;

class CppWriter
{
private:
	typedef std::set<std::string> StringSetType;
	
public:
	explicit CppWriter();
	~CppWriter();
	
	void write(CodeWriter * codeWriter, const CppWriterCallback & callback = CppWriterCallback());
	
	void setHeaderGuard(const std::string & headerGuard);
	void setNamespace(const std::string & ns);
	void useNamespace(const std::string & ns);
	void include(const std::string & fileName);
	void tailInclude(const std::string & fileName);

	CodeBlock * getCodeBlock() { return &this->codeBlock; };
	
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
