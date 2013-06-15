#include "cppcontext.h"
#include "cpptype.h"
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
	clearPointerContainer(this->typeList);
	clearPointerContainer(this->itemList);
	clearPointerContainer(this->fileList);
}

CppType * CppContext::createType()
{
	CppType * type = new CppType();
	this->typeList.push_back(type);
	return type;
}

void CppContext::beginFile(const char * fileName)
{
	GASSERT(this->currentFileInfo == NULL);
	
	this->currentFileInfo = new CppFile(fileName);
	this->fileList.push_back(this->currentFileInfo);
}

void CppContext::endFile(const char * /*fileName*/)
{
	this->currentFileInfo = NULL;
}
