#include "cpptype.h"
#include "cpputil.h"

#if defined(_MSC_VER)
#pragma warning(push, 0)
#endif

#include "llvm/Support/raw_ostream.h"
#include "clang/AST/DeclTemplate.h"
#include "clang/AST/PrettyPrinter.h"

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#include <vector>

using namespace llvm;
using namespace clang;

namespace metagen {


std::string qualTypeToText(const clang::QualType & qualType, const std::string &name)
{
	std::string text;

	llvm::raw_string_ostream stream(text);
	LangOptions langOptions;
	langOptions.CPlusPlus = 1;
	langOptions.Bool = 1;
	PrintingPolicy policy(langOptions);
	policy.SuppressSpecifiers = 0;
//	policy.SuppressScope = 0;
	QualType::print(qualType.split(), stream, policy, name);

	return stream.str();

//	return cppPrintQualType(qualType, name);

//	string qualifiedName;
//
////qualifiedName = type->getTypeClassName();
//
//	if(qualType->getAsCXXRecordDecl() != NULL) {
//		qualifiedName = getNamedDeclOutputName(qualType->getAsCXXRecordDecl());
//	}
//	else if(qualType->getAs<TemplateSpecializationType>() != NULL){
//		const TemplateSpecializationType * t = qualType->getAs<TemplateSpecializationType>();
//		qualifiedName = getTemplateSpecializationName(t);
//	}
//	else if(qualType->getAs<TemplateTypeParmType>() != NULL){
////		const TemplateTypeParmType * t = qualType->getAs<TemplateTypeParmType>();
//		qualifiedName = qualType.getAsString();
//	}
//	else {
//		qualifiedName = qualType.getAsString();
//	}
//
//	qualifiedName = removeRecordWords(qualifiedName);
//
//	return qualifiedName;
}


CppType::CppType(const clang::QualType & qualType)
	: qualType(qualType)
{
}

CppType::~CppType()
{
}

CppType::CppType(const CppType & other)
	: qualType(other.qualType)
{
}

CppType & CppType::operator = (const CppType & other)
{
	this->qualType = other.qualType;

	return *this;
}

std::string CppType::getQualifiedName(const std::string & placeHolder) const
{
	return qualTypeToText(this->qualType.getCanonicalType(), placeHolder);
}

std::string CppType::getQualifiedName() const
{
	return this->getQualifiedName("");
}

bool CppType::isConst() const
{
	return this->qualType.isConstQualified();
}

bool CppType::isVolatile() const
{
	return this->qualType.isVolatileQualified();
}

bool CppType::isArray() const
{
	return this->qualType->isArrayType();
}

bool CppType::isPointer() const
{
	return this->qualType->isPointerType();
}

bool CppType::isMultiplePointer() const
{
	if(this->isPointer()) {
		return CppType(this->qualType->getPointeeType()).isPointer();
	}
	else {
		return false;
	}
}

bool CppType::isPointerToConst() const
{
	if(this->isPointer()) {
		return CppType(this->qualType->getPointeeType()).isConst();
	}
	else {
		return false;
	}
}

bool CppType::isPointerToVolatile() const
{
	if(this->isPointer()) {
		return CppType(this->qualType->getPointeeType()).isVolatile();
	}
	else {
		return false;
	}
}

bool CppType::isReference() const
{
	return this->qualType->isReferenceType();
}

bool CppType::isReferenceToConst() const
{
	if(this->isReference()) {
		return CppType(this->qualType->getPointeeType()).isConst();
	}
	else {
		return false;
	}
}

bool CppType::isReferenceToVolatile() const
{
	if(this->isReference()) {
		return CppType(this->qualType->getPointeeType()).isVolatile();
	}
	else {
		return false;
	}
}

bool CppType::isReferenceToValue() const
{
	if(this->isReference()) {
		return ! CppType(this->qualType->getPointeeType()).isPointer();
	}
	else {
		return false;
	}
}

bool CppType::isFunction() const
{
	return this->qualType->isFunctionType();
}

bool CppType::isFunctionPointer() const
{
	return this->qualType->isFunctionPointerType();
}

bool CppType::isVoid() const
{
	return this->qualType->isVoidType();
}

bool CppType::isFundamental() const
{
	return this->qualType->isFundamentalType();
}

bool CppType::isTemplateDependent() const
{
	return this->qualType->isDependentType();
}

CppType CppType::getNonReferenceType() const
{
	clang::QualType type = this->qualType.getNonReferenceType();
	return CppType(type);
}

CppType CppType::getBaseType() const
{
	return CppType(stripType(this->qualType));
}

const CXXRecordDecl * CppType::getCXXRecordDecl() const
{
	return this->qualType->getAsCXXRecordDecl();
}


} // namespace metagen
