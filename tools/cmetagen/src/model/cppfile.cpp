#include "cppfile.h"
#include "cppnamespace.h"

#include "util.h"

namespace metagen {


CppFile::CppFile(const char * fullFileName, const clang::Decl * decl)
	: super(decl), fullFileName(fullFileName)
{
	this->fullFileName = this->fullFileName.absolute();
//	this->setQualifiedName(normalizePath(this->fullFileName.toString(Poco::Path::PATH_GUESS)));
}

CppFile::~CppFile()
{
}

void CppFile::prepare() const
{
}


} // namespace metagen
