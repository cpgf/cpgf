#ifndef CPGF_CPPINVOKABLE_H
#define CPGF_CPPINVOKABLE_H

#include "cppitem.h"
#include "cpptype.h"

#include <string>

namespace metagen {

class CppPolicy;

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

	bool hasTemplateDependentParam() const;

	size_t getArity() const;
	size_t getNonDefaultParamCount() const;
	CppType getParamType(size_t index) const;
	bool paramHasDefaultValue(size_t index) const;
	std::string getTextOfParamDeafultValue(size_t index) const;

	std::string getParamName(size_t index) const;
	std::string getTextOfUnusedParamsPlaceholder() const;
	std::string getTextOfPointeredType(bool includeClassName) const;
	std::string getTextOfParamList(const ItemTextOptionFlags & options) const;
	std::string getText(const ItemTextOptionFlags & options) const;
	std::string getTextWithReplacedName(const ItemTextOptionFlags & options, const std::string & replacedName) const;

	CppType getFunctionType() const;
	CppType getResultType() const;

	void getPolicy(CppPolicy * outPolicy) const;

private:
	void doGetPolicy(CppPolicy * outPolicy, const CppType & type, int index) const;
};


} // namespace metagen


#endif
