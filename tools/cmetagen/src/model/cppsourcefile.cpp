#include "cppsourcefile.h"

#include "Poco/Path.h"


namespace metagen {


CppSourceFile::CppSourceFile(const std::string & fileName)
	: fileName(fileName), skipBind(false)
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

const std::string & CppSourceFile::getFileName() const
{
	return this->fileName;
}

std::string CppSourceFile::getBaseFileName() const
{
	return Poco::Path(this->fileName).getBaseName();
}


} // namespace metagen
