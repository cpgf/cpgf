#ifndef __CPPINVOKABLE_H
#define __CPPINVOKABLE_H

#include "cppitem.h"
#include "cpptype.h"

#include <string>

namespace metagen {


class CppInvokable : public CppNamedItem
{
private:
	typedef CppNamedItem super;

public:
	explicit CppInvokable(const clang::Decl * decl);
	
	virtual bool isInvokable() const { return true; }
	bool isStatic() const;
	bool isConst() const;
	bool isVirtual() const;
	bool isPureVirtual() const;
	bool isVariadic() const;
	bool isTemplate() const;
	bool isOverloaded() const;
	bool hasResult() const;

	size_t getArity() const;
	CppType getParamType(size_t index) const;
	bool paramHasDefaultValue(size_t index) const;
	std::string getTextOfParamDeafultValue(size_t index) const;

	std::string getTextOfPointeredType() const;
	std::string getTextOfParamList(const ItemTextOptionFlags & options) const;
	std::string getText(const ItemTextOptionFlags & options) const;
	std::string getTextWithReplacedName(const ItemTextOptionFlags & options, const std::string & replacedName) const;

	CppType getFunctionType() const;
	CppType getResultType() const;
};


} // namespace metagen


#endif
