#include "cppitem.h"

CppItem::CppItem()
	: visibility(ivPublic), parent(NULL)
{
}

CppItem::~CppItem()
{
}

void CppItem::dump(std::ostream & os, int level)
{
	this->dumpIndent(os, level);
	os << "name=" << this->name
		<< " qualifiedName=" << this->qualifiedName
		<< " visibility=" << this->visibility
		<< " static=" << this->isStatic()
		<< " parent=" << (this->parent == NULL ? "-NONE-" : this->parent->getQualifiedName())
		<< std::endl;
}

void CppItem::dumpIndent(std::ostream & os, int level)
{
	while(level > 0) {
		--level;
		os << "    ";
	}
}
