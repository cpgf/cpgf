#include "cppfile.h"
#include "cppnamespace.h"

#include "util.h"

namespace metagen {


CppFile::CppFile(const clang::Decl * decl)
	: super(decl)
{
}

CppFile::~CppFile()
{
}


} // namespace metagen
