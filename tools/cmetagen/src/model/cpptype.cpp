#include "cpptype.h"
#include "cpputil.h"


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
	return qualTypeToText(this->qualType, placeHolder);
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

