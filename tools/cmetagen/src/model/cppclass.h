#ifndef __CPPCLASS_H
#define __CPPCLASS_H

#include "cppcontainer.h"

#include <string>


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
	typedef std::vector<const CppConstructor *> ConstructorListType;
	typedef std::vector<const BaseClass *> BaseClassListType;
	
public:
	explicit CppClass(const clang::Decl * decl);
	~CppClass();
	
	bool isTemplate() const;
	bool isChainedTemplate() const;
	bool isAnonymous() const;
	
	std::string getTextOfTemplateParamList(const ItemTextOptionFlags & options) const;
	std::string getTextOfChainedTemplateParamList(const ItemTextOptionFlags & options) const;
	
	const BaseClassListType * getBaseClassList() const { return &this->baseClassList; }
	const ConstructorListType * getConstructorList() const { return &this->constructorList; }
	const CppDestructor * getDestructor() { return this->destructor; }

protected:
	BaseClassListType * getBaseClassList() { return &this->baseClassList; }
	ConstructorListType * getConstructorList() { return &this->constructorList; }
	
	virtual ItemCategory getCategory() const { return icClass; }
	virtual void doAddItem(CppItem * item);

private:
	BaseClassListType baseClassList;
	ConstructorListType constructorList;
	const CppDestructor * destructor;
	
private:
	friend class ClangParserImplement;
};


#endif
