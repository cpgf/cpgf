#include "cppcontext.h"
#include "cppnamespace.h"
#include "cppfile.h"

#include "util.h"

#include "cpgf/gassert.h"


CppContext::CppContext()
	: currentFileInfo(NULL)
{
}

CppContext::~CppContext()
{
	clearPointerContainer(this->itemList);
	clearPointerContainer(this->fileList);
}

void CppContext::beginFile(const char * fileName, clang::Decl * decl)
{
	GASSERT(this->currentFileInfo == NULL);

	this->currentFileInfo = new CppFile(fileName, decl);
	this->fileList.push_back(this->currentFileInfo);
}

void CppContext::endFile(const char * /*fileName*/)
{
	this->currentFileInfo->prepare();
	this->currentFileInfo = NULL;
}
