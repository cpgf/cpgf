#include "fileinfo.h"
#include "cppnamespace.h"


FileInfo::FileInfo(const char * fullFileName)
	: fullFileName(fullFileName), globalNamespace(new CppNamespace())
{
}

FileInfo::~FileInfo()
{
}
