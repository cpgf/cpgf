#ifndef CPGF_CPPITEM_H
#define CPGF_CPPITEM_H

#include "cpgf/gflags.h"

#include <string>
#include <ostream>

namespace clang { class Decl; class ASTContext; }

namespace metagen {

enum ItemVisibility
{
	ivFirst = 0,
	ivPublic = ivFirst, ivProtected, ivPrivate,
	ivCount
};

enum ItemCategory
{
	icFirst = 0,
	icFile = icFirst, icNamespace, icClass, icEnum,
	icConstructor, icDestructor,
	icField, icMethod, icOperator,
	icCount
};

enum ItemTextOption
{
	itoWithArgType = 1 << 0,
	itoWithArgName = 1 << 1,
	itoWithDefaultValue = 1 << 2,
	itoIncludeArg = itoWithArgType | itoWithArgName | itoWithDefaultValue,
	
	itoWithResult = 1 << 3,
	itoWithName = 1 << 4,
	itoWithQualifiers = 1 << 5,
	itoAsPointer = 1 << 6,
	itoWithParentName = 1 << 7
};

typedef cpgf::GFlags<ItemTextOption> ItemTextOptionFlags;

extern const char * const ItemNames[icCount];


class CppContainer;
class CppContext;
class Project;

class CppItem
{
private:
	enum ItemFlags {
		ifInMainFile = 1 << 0
	};
	
protected:
	explicit CppItem(const clang::Decl * decl);

public:
	virtual ~CppItem();

	virtual ItemCategory getCategory() const = 0;

	virtual void dump(std::ostream & os, int level = 0) const;

	const CppContainer * getParent() const { return this->parent; }
	const CppContainer * getNamedParent() const; // return non-anonymous parent

	bool isInMainFile() const { return this->flags.has(ifInMainFile); }
	
	int getIndexInCategory() const { return this->indexInCategory; }
	
	ItemVisibility getVisibility() const;

	const Project * getProject() const;

	bool isFile() const { return this->getCategory() == icFile; }
	bool isNamespace() const { return this->getCategory() == icNamespace; }
	bool isClass() const { return this->getCategory() == icClass; }
	bool isConstructor() const { return this->getCategory() == icConstructor; }
	bool isDestructor() const { return this->getCategory() == icDestructor; }
	bool isEnum() const { return this->getCategory() == icEnum; }
	bool isField() const { return this->getCategory() == icField; }
	bool isMethod() const { return this->getCategory() == icMethod; }
	bool isOperator() const { return this->getCategory() == icOperator; }
	virtual bool isInvokable() const { return false; }
	
	bool isGlobal() const;
	bool isNestedClass() const;

	virtual bool isContainer() const { return false; }
	virtual bool isNamed() const { return false; }
	
protected:
	const clang::Decl * getDecl() const { return this->declaration; }
	const clang::ASTContext * getASTContext() const;
	
	void setCppContext(const CppContext * cppContext) { this->cppContext = cppContext; }	
	const CppContext * getCppContext() const { return this->cppContext; }
	
	void setParent(CppContainer * parent) { this->parent = parent; }
	
	void setIndexInCategory(int indexInCategory) { this->indexInCategory = indexInCategory; }

	void setInMainFile(bool inMainFile) { this->flags.setByBool(ifInMainFile, inMainFile); }
	
	void dumpIndent(std::ostream & os, int level) const;
	
private:
	const clang::Decl * declaration;
	ItemVisibility visibility;
	const CppContainer * parent; // file, namespace or class
	const CppContext * cppContext;
	cpgf::GFlags<ItemFlags> flags;
	int indexInCategory;

private:
	friend class CppContext;
	friend class CppContainer;
	friend class ClangParserImplement;
};

class CppNamedItem : public CppItem
{
private:
	typedef CppItem super;

public:
	const std::string & getName() const;
	const std::string & getQualifiedName() const;
	const std::string & getQualifiedNameWithoutNamespace() const;

	virtual bool isNamed() const { return true; }

protected:
	explicit CppNamedItem(const clang::Decl * decl);
	
private:
	void checkLoadNames() const;

private:
	mutable std::string name;
	mutable std::string qualifiedName;
	mutable std::string qualifiedNameWithoutNamespace;
};

bool isVisibilityAllowed(ItemVisibility visibility, const Project * project);


} // namespace metagen


#endif
