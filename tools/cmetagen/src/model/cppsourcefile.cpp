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

const std::string & CppSourceFile::getFileName() const
{
	return this->fileName;
}

const std::string & CppSourceFile::getBaseFileName() const
{
	return this->baseName;
}


} // namespace metagen
