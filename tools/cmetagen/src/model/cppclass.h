#ifndef CPGF_CPPCLASS_H
#define CPGF_CPPCLASS_H

#include "cppcontainer.h"
#include "cppclasstraits.h"

#include <string>

namespace clang { class CXXBaseSpecifier; class CXXRecordDecl; }

namespace metagen {

class CppConstructor;
class CppDestructor;
class CppContext;
class CppClass;

class BaseClass
{
public:
	BaseClass(const clang::CXXBaseSpecifier * baseSpecifier, const CppContext * cppContext,
		const CppClass * masterCppClass);
	
	ItemVisibility getVisibility() const;
	
	std::string getQualifiedName() const;

	const CppClass * getCppClass() const;

private:
	const clang::CXXBaseSpecifier * baseSpecifier;
	const CppContext * cppContext;
	const CppClass * masterCppClass;
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
	
	const BaseClassListType * getBaseClassList() const { return &this->baseClassList; }
	const ConstructorListType * getConstructorList() const { return &this->constructorList; }
	const CppDestructor * getDestructor() const { return this->destructor; }

	bool isTemplate() const;
	bool isChainedTemplate() const;
	bool isAnonymous() const;
	bool isAbstract() const;

	int getTemplateDepth() const;
	int getTemplateParamCount() const;
	std::string getTemplateParamName(int paramIndex) const;

	std::string getTextOfTemplateParamList(const ItemTextOptionFlags & options) const;
	std::string getTextOfChainedTemplateParamList(const ItemTextOptionFlags & options) const;
	std::string getTextOfQualifedInstantiationName() const;
	
	void getPolicy(CppPolicy * outPolicy) const;
	
private:
	BaseClassListType * getBaseClassList() { return &this->baseClassList; }
	ConstructorListType * getConstructorList() { return &this->constructorList; }
	
	virtual ItemCategory getCategory() const { return icClass; }
	virtual void doAddItem(CppItem * item);

	bool isDefaultConstructorHidden() const;

	CppClassTraits getClassTraits() const;
	
private:
	BaseClassListType baseClassList;
	ConstructorListType constructorList;
	const CppDestructor * destructor;
	
private:
	friend class ClangParserImplement;
	friend class CppContext;
	friend class BaseClass;
};


} // namespace metagen


#endif
