#ifndef CPGF_CPPTYPE_H
#define CPGF_CPPTYPE_H

#if defined(_MSC_VER)
#pragma warning(push, 0)
#endif

#include "clang/AST/Type.h"

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#include <string>

namespace clang { class CXXRecordDecl; }

namespace metagen {

class CppType
{
public:
	explicit CppType(const clang::QualType & qualType);
	~CppType();
	
	CppType(const CppType & other);
	CppType & operator = (const CppType & other);
	
	std::string getQualifiedName(const std::string & placeHolder) const;
	std::string getQualifiedName() const;

	bool isConst() const;
	bool isVolatile() const;
	bool isArray() const;

	bool isPointer() const;
	bool isMultiplePointer() const;
	bool isPointerToConst() const;
	bool isPointerToVolatile() const;

	bool isReference() const;
	bool isReferenceToConst() const;
	bool isReferenceToVolatile() const;
	bool isReferenceToValue() const; // "const A &" returns true, "const A * &" returns false

	bool isFunction() const;
	bool isFunctionPointer() const;

	bool isVoid() const;
	bool isFundamental() const;

	bool isTemplateDependent() const;

	CppType getNonReferenceType() const;
	CppType getBaseType() const; // strip all pointers, reference, or arrays
	
	const clang::CXXRecordDecl * getCXXRecordDecl() const;
	
private:
	clang::QualType qualType;
};


} // namespace metagen


#endif
