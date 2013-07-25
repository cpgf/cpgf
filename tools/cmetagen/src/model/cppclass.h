#ifndef __CPPCLASS_H
#define __CPPCLASS_H

#include "cppcontainer.h"

#include <string>

namespace clang { class CXXBaseSpecifier; }

namespace metagen {

class CppConstructor;
class CppDestructor;
class CppContext;
class CppClass;

class BaseClass
{
public:
	explicit BaseClass(const clang::CXXBaseSpecifier * baseSpecifier, const CppContext * cppContext);
	
	ItemVisibility getVisibility() const;
	
	std::string getQualifiedName() const;

	const CppClass * getCppClass() const;

private:
	const clang::CXXBaseSpecifier * baseSpecifier;
	const CppContext * cppContext;
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


} // namespace metagen


#endif
