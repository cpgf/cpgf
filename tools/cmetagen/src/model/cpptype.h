#ifndef __CPPTYPE_H
#define __CPPTYPE_H

#include <string>

class CppType
{
public:
	const std::string & getLiteralName() const { return this->literalName; }
	void setLiteralName(const std::string & literalName) { this->literalName = literalName; }

	const std::string & getQualifiedName() const { return this->qualifiedName; }
	void setQualifiedName(const std::string & qualifiedName) { this->qualifiedName = qualifiedName; }
	
private:
	std::string literalName;
	std::string qualifiedName;
};


#endif
