#include "cppitem.h"
#include "cppcontainer.h"
#include "cpputil.h"

#include "util.h"

#if defined(_MSC_VER)
#pragma warning(push, 0)
#endif

#include "clang/AST/Decl.h"
#include "clang/Basic/Specifiers.h"

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

using namespace clang;

const char * const ItemNames[icCount] = {
	"File", "Namespace", "Class", "Enum",
	"Constructor", "Destructor",
	"Field", "Method", "Operator",
};

ItemVisibility accessToVisibility(AccessSpecifier access)
{
	switch(access) {
		case AS_public:
			return ivPublic;

		case AS_protected:
			return ivProtected;

		case AS_private:
			return ivPrivate;
	}

	return ivPublic;
}


CppItem::CppItem(clang::Decl * decl)
	: declaration(decl), visibility(ivPublic), parent(NULL)
{
}

CppItem::~CppItem()
{
}

ItemVisibility CppItem::getVisibility() const
{
	return accessToVisibility(this->getDecl()->getAccess());
}

void CppItem::dump(std::ostream & os, int level)
{
	this->dumpIndent(os, level);
	os	<< "category=" << ItemNames[this->getCategory()]
		<< " visibility=" << this->getVisibility()
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


CppNamedItem::CppNamedItem(clang::Decl * decl)
	: super(decl)
{
}

void CppNamedItem::checkLoadNames() const
{
	if(this->qualifiedName.empty() && isa<NamedDecl>(this->getDecl())) {
		getNamedDeclNames(dyn_cast<NamedDecl>(this->getDecl()),
			this->name,
			this->qualifiedName,
			this->qualifiedNameWithoutNamespace
		);
	}
}

const std::string & CppNamedItem::getName() const
{
	this->checkLoadNames();
	return this->name;
}

const std::string & CppNamedItem::getQualifiedName() const
{
	this->checkLoadNames();
	return this->qualifiedName;
}

const std::string & CppNamedItem::getQualifiedNameWithoutNamespace() const
{
	this->checkLoadNames();
	return this->qualifiedNameWithoutNamespace;
}

const std::string & CppNamedItem::getOutputName() const
{
	return this->getQualifiedNameWithoutNamespace();
}

