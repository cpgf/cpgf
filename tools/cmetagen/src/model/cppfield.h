#ifndef __CPPFIELD_H
#define __CPPFIELD_H

#include "Cppitem.h"

class CppField : public CppNamedItem
{
private:
	typedef CppNamedItem super;

public:
	explicit CppField(const clang::Decl * decl);
	
	bool isStatic() const;

	int getBitFields() const { return this->bitFields; }
	void setBitFields(int bitFields) { this->bitFields = bitFields; }
	bool hasBitFields() const { return this->bitFields > 0; }

protected:
	virtual ItemCategory getCategory() const { return icField; }
	
private:
	int bitFields;
};


#endif
