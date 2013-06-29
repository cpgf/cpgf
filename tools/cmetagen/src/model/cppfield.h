#ifndef __CPPFIELD_H
#define __CPPFIELD_H

#include "Cppitem.h"

class CppType;

class CppField : public CppNamedItem
{
private:
	typedef CppNamedItem super;

public:
	explicit CppField(clang::Decl * decl);
	
	bool isStatic() const;

	CppType * getType() const { return this->type; }
	void setType(CppType * type) { this->type = type; }
	
	int getBitFields() const { return this->bitFields; }
	void setBitFields(int bitFields) { this->bitFields = bitFields; }
	bool hasBitFields() const { return this->bitFields > 0; }

protected:
	virtual ItemCategory getCategory() const { return icField; }
	
private:
	CppType * type;
	int bitFields;
};


#endif
