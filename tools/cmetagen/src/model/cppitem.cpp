#include "cppitem.h"
#include "cppcontainer.h"
#include "cppclass.h"
#include "cppnamespace.h"
#include "cppcontext.h"
#include "cpputil.h"
#include "util.h"
#include "project.h"

#if defined(_MSC_VER)
#pragma warning(push, 0)
#endif

#include "clang/AST/Decl.h"
#include "clang/Basic/Specifiers.h"

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

using namespace clang;

namespace metagen {


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

bool isVisibilityAllowed(ItemVisibility visibility, const Project * project)
{
	if((visibility == ivPublic) != project->doesAllowPublic()
		|| (visibility == ivProtected) != project->doesAllowProtected()
		|| (visibility == ivPrivate) != project->doesAllowPrivate()
		) {
		return false;
	}

	return true;
}


CppItem::CppItem(const clang::Decl * decl)
	: declaration(decl), visibility(ivPublic), parent(NULL), cppContext(NULL), indexInCategory(0)
{
}

CppItem::~CppItem()
{
}

const CppContainer * CppItem::getNamedParent() const
{
	const CppContainer * container = this->getParent();
	for(;;) {
		if(container->isClass()) {
			if(! static_cast<const CppClass *>(container)->isAnonymous()) {
				break;
			}
		}
		else if(container->isNamespace()) {
			if(! static_cast<const CppNamespace *>(container)->isAnonymous()) {
				break;
			}
		}
		else {
			break;
		}
		container = container->getParent();
	}
	return container;
}

ItemVisibility CppItem::getVisibility() const
{
	return accessToVisibility(this->getDecl()->getAccess());
}

const clang::ASTContext * CppItem::getASTContext() const
{
	return &this->declaration->getASTContext();
}

const Project * CppItem::getProject() const
{
	return this->getCppContext()->getProject();
}

bool CppItem::isGlobal() const
{
	return ! this->getParent()->isClass();
}

bool CppItem::isNestedClass() const
{
	return this->isClass() && this->getParent() != NULL && this->getParent()->isClass();
}

void CppItem::dump(std::ostream & os, int level) const
{
	this->dumpIndent(os, level);
	os	<< "category=" << ItemNames[this->getCategory()]
		<< " visibility=" << this->getVisibility()
		<< " parent=" << (this->parent == NULL ? "-NONE-" : this->parent->getQualifiedName())
		<< std::endl;
}

void CppItem::dumpIndent(std::ostream & os, int level) const
{
	while(level > 0) {
		--level;
		os << "    ";
	}
}


CppNamedItem::CppNamedItem(const clang::Decl * decl)
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


} // namespace metagen
