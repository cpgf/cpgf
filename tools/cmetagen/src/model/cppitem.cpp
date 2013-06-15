#include "cppitem.h"
#include "cppcontainer.h"

const char * const ItemNames[icCount] = {
	"File", "Namespace", "Class", "Enum",
	"Constructor", "Destructor",
	"Field", "Method", "Operator",
};

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
		<< "category=" << ItemNames[this->getCategory()]
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
