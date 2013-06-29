#ifndef __CPPITEM_H
#define __CPPITEM_H

#include "cpgf/gflags.h"

#include <string>
#include <ostream>

namespace clang { class Decl; }


enum ItemVisibility
{
	ivPublic, ivProtected, ivPrivate
};

enum ItemCategory
{
	icFile = 0, icNamespace, icClass, icEnum,
	icConstructor, icDestructor,
	icField, icMethod, icOperator,
	icCount
};

extern const char * const ItemNames[icCount];


class CppContainer;

class CppItem
{
protected:
	explicit CppItem(clang::Decl * decl);

public:
	virtual ~CppItem();

	virtual ItemCategory getCategory() const = 0;
	
	virtual void dump(std::ostream & os, int level = 0);

	CppContainer * getParent() const { return this->parent; }
	void setParent(CppContainer * parent) { this->parent = parent; }
	
	ItemVisibility getVisibility() const;

	bool isFile() const { return this->getCategory() == icFile; }
	bool isNamespace() const { return this->getCategory() == icNamespace; }
	bool isClass() const { return this->getCategory() == icClass; }
	bool isConstructor() const { return this->getCategory() == icConstructor; }
	bool isDestructor() const { return this->getCategory() == icDestructor; }
	bool isEnum() const { return this->getCategory() == icEnum; }
	bool isField() const { return this->getCategory() == icField; }
	bool isMethod() const { return this->getCategory() == icMethod; }
	bool isOperator() const { return this->getCategory() == icOperator; }
	bool isContainer() const { return this->isFile() || this->isNamespace() || this->isClass(); }
	
protected:
	clang::Decl * getDecl() const { return this->declaration; }
	
	void dumpIndent(std::ostream & os, int level);
	
private:
	clang::Decl * declaration;
	ItemVisibility visibility;
	CppContainer * parent; // file, namespace or class
};

class CppNamedItem : public CppItem
{
private:
	typedef CppItem super;

public:
	const std::string & getName() const;
	const std::string & getQualifiedName() const;
	const std::string & getQualifiedNameWithoutNamespace() const;

	const std::string & getOutputName() const;

protected:
	explicit CppNamedItem(clang::Decl * decl);
	
private:
	void checkLoadNames() const;

private:
	mutable std::string name;
	mutable std::string qualifiedName;
	mutable std::string qualifiedNameWithoutNamespace;
};


#endif
