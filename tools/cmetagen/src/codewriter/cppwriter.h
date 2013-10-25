#ifndef CPGF_CPPWRITER_H
#define CPGF_CPPWRITER_H

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
	std::set<std::string> usedNamespaceList;
	std::vector<std::string> includeList;
	std::vector<std::string> tailIncludeList;
	CodeBlock codeBlock;
};


} // namespace metagen


#endif
