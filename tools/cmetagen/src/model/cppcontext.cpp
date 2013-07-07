#include "cppcontext.h"
#include "cppnamespace.h"
#include "cppfile.h"

#include "util.h"

#include "cpgf/gassert.h"


CppContext::CppContext(const Config * config)
	: config(config)
{
}

CppContext::~CppContext()
{
	clearPointerContainer(this->itemList);
}

void CppContext::beginFile(const char * fileName, clang::Decl * decl)
{
	GASSERT(! this->cppFile);

	this->cppFile.reset(new CppFile(fileName, decl));
}

void CppContext::endFile(const char * /*fileName*/)
{
	this->cppFile->prepare();
}

void CppContext::itemCreated(CppItem * item)
{
	this->itemList.push_back(item);
	item->setCppContext(this);
	
	if(item->isNamed()) {
		CppNamedItem * namedItem = static_cast<CppNamedItem *>(item);
		this->itemNameMap.insert(make_pair(namedItem->getQualifiedName(), namedItem));
	}
}

const CppNamedItem * CppContext::findNamedItem(const std::string & qualifiedName) const
{
	ItemNameMapType::const_iterator it = this->itemNameMap.find(qualifiedName);
	if(it == this->itemNameMap.end()) {
		return NULL;
	}
	else {
		return it->second;
	}
}

