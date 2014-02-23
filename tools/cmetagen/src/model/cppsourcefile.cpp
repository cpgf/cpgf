#include "cppsourcefile.h"

#include "Poco/Path.h"


namespace metagen {


CppSourceFile::CppSourceFile(const std::string & fileName)
	: fileName(fileName),
		baseName(Poco::Path(fileName).getBaseName()),
		skipBind(false)
{
}

CppSourceFile::~CppSourceFile()
{
}

const char * CppSourceFile::getFileName() const
{
	return this->fileName.c_str();
}

const char * CppSourceFile::getBaseFileName() const
{
	return this->baseName.c_str();
}

void CppSourceFile::setSkipBind(bool skip)
{
	this->skipBind = skip;
}

bool CppSourceFile::shouldSkipBind() const
{
	return this->skipBind;
}

void CppSourceFile::addInclude(const char * include)
{
	this->includeList.push_back(std::string(include));
}

const std::vector<std::string> & CppSourceFile::getIncludeList() const
{
	return this->includeList;
}

void CppSourceFile::addMetaInclude(const char * include)
{
	this->metaIncludeList.push_back(std::string(include));
}

const std::vector<std::string> & CppSourceFile::getMetaIncludeList() const
{
	return this->metaIncludeList;
}


} // namespace metagen
