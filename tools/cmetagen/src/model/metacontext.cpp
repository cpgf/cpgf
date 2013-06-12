#include "metacontext.h"
#include "cpptype.h"
#include "cppnamespace.h"
#include "fileinfo.h"

#include "../util.h"

#include "cpgf/gassert.h"


MetaContext::MetaContext()
	: currentFileInfo(NULL)
{
}

MetaContext::~MetaContext()
{
	clearPointerContainer(this->typeList);
	clearPointerContainer(this->itemList);
	clearPointerContainer(this->fileInfoList);
}

CppType * MetaContext::createType()
{
	CppType * type = new CppType();
	this->typeList.push_back(type);
	return type;
}

void MetaContext::beginFile(const char * fileName)
{
	GASSERT(this->currentFileInfo == NULL);
	
	this->currentFileInfo = new FileInfo(fileName);
	this->fileInfoList.push_back(this->currentFileInfo);
}

void MetaContext::endFile(const char * fileName)
{
	this->currentFileInfo = NULL;
}
