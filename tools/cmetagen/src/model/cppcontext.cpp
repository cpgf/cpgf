#include "cppcontext.h"
#include "cppnamespace.h"
#include "cppfile.h"

#include "util.h"

#include "cpgf/gassert.h"

namespace metagen {


CppContext::CppContext(const Project * project)
	: project(project)
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
		this->itemNameMap[item->getCategory()].insert(make_pair(namedItem->getQualifiedName(), namedItem));
	}
}

const CppNamedItem * CppContext::findNamedItem(ItemCategory category, const std::string & qualifiedName) const
{
	ItemNameMapType::const_iterator it = this->itemNameMap[category].find(qualifiedName);
	if(it == this->itemNameMap[category].end()) {
		return NULL;
	}
	else {
		return it->second;
	}
}


} // namespace metagen
