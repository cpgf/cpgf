#ifndef __CPPCLASS_H
#define __CPPCLASS_H

#include "cppcontainer.h"

class CppConstructor;
class CppDestructor;

class BaseClass
{
public:
	BaseClass() : visibility(ivPublic) {}
	
	ItemVisibility getVisibility() const { return this->visibility; }
	void setVisibility(ItemVisibility visibility) { this->visibility = visibility; }
	
	const std::string & getName() const { return this->name; }
	void setName(const std::string & name) { this->name = name; }
	const std::string & getQualifiedName() const { return this->qualifiedName; }
	void setQualifiedName(const std::string & qualifiedName) { this->qualifiedName = qualifiedName; }

private:
	ItemVisibility visibility;
	std::string name;
	std::string qualifiedName;
};

class CppClass : public CppContainer
{
private:
	typedef CppContainer super;

public:
	typedef std::vector<CppConstructor *> ConstructorListType;
	typedef std::vector<BaseClass *> BaseClassListType;
	
public:
	explicit CppClass(clang::Decl * decl);
	~CppClass();
	
	bool isTemplate() const;
	
	BaseClassListType * getBaseClassList() { return &this->baseClassList; }
	ConstructorListType * getConstructorList() { return &this->constructorList; }
	CppDestructor * getDestructor() { return this->destructor; }

protected:
	virtual ItemCategory getCategory() const { return icClass; }
	virtual void doAddItem(CppItem * item);

private:
	BaseClassListType baseClassList;
	ConstructorListType constructorList;
	CppDestructor * destructor;
};


#endif
