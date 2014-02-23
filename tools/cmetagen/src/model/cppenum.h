#ifndef CPGF_CPPENUM_H
#define CPGF_CPPENUM_H

#include "cppitem.h"

#include <vector>

namespace metagen {


class CppEnumValue
{
public:
	explicit CppEnumValue(const std::string & name, const std::string & qualifiedName) : name(name), qualifiedName(qualifiedName) {}
	
	const std::string & getName() const { return this->name; }
	const std::string & getQualifiedName() const { return this->qualifiedName; }

private:
	std::string name;
	std::string qualifiedName;
};

class CppEnum : public CppNamedItem
{
private:
	typedef CppNamedItem super;
	
public:
	typedef std::vector<CppEnumValue> ValueListType;

public:
	explicit CppEnum(const clang::Decl * decl) : super(decl) {}

	const ValueListType * getValueList() const;
	bool isAnonymous() const;
		
protected:
	virtual ItemCategory getCategory() const { return icEnum; }
	void addValue(const std::string & name, const std::string & qualifiedName);
	
private:
	ValueListType valueList;
	
private:
	friend class ClangParserImplement;
};


} // namespace metagen


#endif
