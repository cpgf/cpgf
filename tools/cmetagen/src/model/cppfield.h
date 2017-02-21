#ifndef CPGF_CPPFIELD_H
#define CPGF_CPPFIELD_H

#include "Cppitem.h"
#include "cpptype.h"

namespace metagen {


class CppField : public CppNamedItem
{
private:
	typedef CppNamedItem super;

public:
	explicit CppField(const clang::Decl * decl);
	
	bool isStatic() const;
	bool isBitField() const;
	bool isAnonymousStructOrUnion() const;

	CppType getType() const;

protected:
	virtual ItemCategory getCategory() const { return icField; }
	
};


} // namespace metagen


#endif
