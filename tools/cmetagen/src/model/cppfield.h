#ifndef __CPPFIELD_H
#define __CPPFIELD_H

#include "Cppitem.h"
#include "cpptype.h"

class CppField : public CppNamedItem
{
private:
	typedef CppNamedItem super;

public:
	explicit CppField(const clang::Decl * decl);
	
	bool isStatic() const;
	bool isBitField() const;

	CppType getType() const;

protected:
	virtual ItemCategory getCategory() const { return icField; }
	
};


#endif
