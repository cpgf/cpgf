#ifndef __CPPOPERATOR_H
#define __CPPOPERATOR_H

#include "cppitem.h"
#include "cppinvokable.h"


class CppOperator : public CppInvokable
{
private:
	typedef CppInvokable super;
	
public:
	explicit CppOperator(const clang::Decl * decl);
	
	const std::string & getOperatorName() const;
	bool isArray() const;
	bool isFunctor() const;
	bool isTypeConverter() const;

protected:
	virtual ItemCategory getCategory() const { return icOperator; }
	
private:
	mutable std::string operatorName;
};


#endif
