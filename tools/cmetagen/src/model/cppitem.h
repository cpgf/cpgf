#ifndef __CPPITEM_H
#define __CPPITEM_H

#include "cpgf/gflags.h"

#include <string>
#include <ostream>


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

enum ItemFlags
{
	ifStatic = 1 << 0,
	ifConst = 1 << 1,
};

class CppContainer;

class CppItem
{
protected:
	typedef cpgf::GFlags<ItemFlags> Flags;
	
protected:
	CppItem();

public:
	virtual ~CppItem();

	virtual ItemCategory getCategory() const = 0;
	
	virtual void dump(std::ostream & os, int level = 0);

	CppContainer * getParent() const { return this->parent; }
	void setParent(CppContainer * parent) { this->parent = parent; }
	
	const std::string & getName() const { return this->name; }
	const std::string & getQualifiedName() const { return this->qualifiedName; }
	void setQualifiedName(const std::string & qualifiedName);
	
	bool isStatic() const { return this->flags.has(ifStatic); }
	void setStatic(bool value) { this->flags.setByBool(ifStatic, value); }
	
	ItemVisibility getVisibility() const { return this->visibility; }
	void setVisibility(ItemVisibility visibility) { this->visibility = visibility; }

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
	virtual bool isTemplate() const { return false; }
	
protected:
	Flags & getFlags() { return this->flags; }
	const Flags & getFlags() const { return this->flags; }
	
	void dumpIndent(std::ostream & os, int level);
	
private:
	std::string name;
	std::string qualifiedName;
	ItemVisibility visibility;
	Flags flags;
	CppContainer * parent; // file, namespace or class
};



#endif
