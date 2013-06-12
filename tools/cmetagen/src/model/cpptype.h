#ifndef __CPPTYPE_H
#define __CPPTYPE_H

#include "cpgf/gflags.h"

#include <string>

class CppType
{
private:
	enum TypeFlags {
		tfConst = 1 << 0,
		tfVolatile = 1 << 1,
		tfArray = 1 << 2,
		
		tfPointer = 1 << 3,
		tfMultiPointer = 1 << 4,
		tfPointerToConst = 1 << 5,
		tfPointerToVolatile = 1 << 6,
		
		tfReference = 1 << 7,
		tfReferenceToConst = 1 << 8,
		tfReferenceToVolatile = 1 << 9,
		
		tfFunction = 1 << 10,
		tfFunctionPointer = 1 << 11,
	};
	
public:
	const std::string & getBaseName() const { return this->baseName; }
	void setBaseName(const std::string & baseName) { this->baseName = baseName; }

	const std::string & getLiteralName() const { return this->literalName; }
	void setLiteralName(const std::string & literalName) { this->literalName = literalName; }

	const std::string & getQualifiedName() const { return this->qualifiedName; }
	void setQualifiedName(const std::string & qualifiedName) { this->qualifiedName = qualifiedName; }
	
	bool isConst() const { return this->flags.has(tfConst); }
	void setConst(bool value) { return this->flags.setByBool(tfConst, value); }
	bool isVolatile() const { return this->flags.has(tfVolatile); }
	void setVolatile(bool value) { return this->flags.setByBool(tfVolatile, value); }
	bool isArray() const { return this->flags.has(tfArray); }
	void setArray(bool value) { return this->flags.setByBool(tfArray, value); }

	bool isPointer() const { return this->flags.has(tfPointer); }
	void setPointer(bool value) { return this->flags.setByBool(tfPointer, value); }
	bool isMultiPointer() const { return this->flags.has(tfMultiPointer); }
	void setMultiPointer(bool value) { return this->flags.setByBool(tfMultiPointer, value); }
	bool isPointerToConst() const { return this->flags.has(tfPointerToConst); }
	void setPointerToConst(bool value) { return this->flags.setByBool(tfPointerToConst, value); }
	bool isPointerToVolatile() const { return this->flags.has(tfPointerToVolatile); }
	void setPointerToVolatile(bool value) { return this->flags.setByBool(tfPointerToVolatile, value); }

	bool isReference() const { return this->flags.has(tfReference); }
	void setReference(bool value) { return this->flags.setByBool(tfReference, value); }
	bool isReferenceToConst() const { return this->flags.has(tfReferenceToConst); }
	void setReferenceToConst(bool value) { return this->flags.setByBool(tfReferenceToConst, value); }
	bool isReferenceToVolatile() const { return this->flags.has(tfReferenceToVolatile); }
	void setReferenceToVolatile(bool value) { return this->flags.setByBool(tfReferenceToVolatile, value); }

	bool isFunction() const { return this->flags.has(tfFunction); }
	void setFunction(bool value) { return this->flags.setByBool(tfFunction, value); }
	bool isFunctionPointer() const { return this->flags.has(tfFunctionPointer); }
	void setFunctionPointer(bool value) { return this->flags.setByBool(tfFunctionPointer, value); }
	
private:
	std::string baseName;
	std::string literalName;
	std::string qualifiedName;
	cpgf::GFlags<TypeFlags> flags;
};


#endif
