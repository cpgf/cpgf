#include "cppfile.h"
#include "cppnamespace.h"

#include "util.h"


CppFile::CppFile(const char * fullFileName)
	: fullFileName(fullFileName)
{
	this->fullFileName = this->fullFileName.absolute();
	this->setQualifiedName(normalizePath(this->fullFileName.toString(Poco::Path::PATH_GUESS)));
}

CppFile::~CppFile()
{
}
