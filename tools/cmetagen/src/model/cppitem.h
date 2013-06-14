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
	icFile, icNamespace, icClass, icEnum,
	icConstructor, icDestructor,
	icField, icMethod, icOperator,
};

enum ItemFlags
{
	ifStatic = 1 << 0,
	ifConst = 1 << 1,
};

class CppItem
{
protected:
	typedef cpgf::GFlags<ItemFlags> Flags;
	
protected:
	CppItem();

public:
	virtual ~CppItem();

	CppItem * getParent() const { return this->parent; }
	void setParent(CppItem * parent) { this->parent = parent; }
	
	const std::string & getName() const { return this->name; }
	void setName(const std::string & name) { this->name = name; }
	const std::string & getQualifiedName() const { return this->qualifiedName; }
	void setQualifiedName(const std::string & qualifiedName) { this->qualifiedName = qualifiedName; }
	
	bool isStatic() const { return this->flags.has(ifStatic); }
	void setStatic(bool value) { this->flags.setByBool(ifStatic, value); }
	
	ItemVisibility getVisibility() const { return this->visibility; }
	void setVisibility(ItemVisibility visibility) { this->visibility = visibility; }

	virtual ItemCategory getCategory() const = 0;
	
	virtual void dump(std::ostream & os, int level = 0);

protected:
	Flags & getFlags() { return this->flags; }
	const Flags & getFlags() const { return this->flags; }
	
	void dumpIndent(std::ostream & os, int level);
	
private:
	std::string name;
	std::string qualifiedName;
	ItemVisibility visibility;
	Flags flags;
	CppItem * parent; // either namespace of class
};



#endif
