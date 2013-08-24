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

void CppSourceFile::setSkipBind(bool skip)
{
	this->skipBind = skip;
}

bool CppSourceFile::shouldSkipBind() const
{
	return this->skipBind;
}

const char * CppSourceFile::getFileName() const
{
	return this->fileName.c_str();
}

const char * CppSourceFile::getBaseFileName() const
{
	return this->baseName.c_str();
}


} // namespace metagen
