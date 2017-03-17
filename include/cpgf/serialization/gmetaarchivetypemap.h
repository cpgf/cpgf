#ifndef CPGF_GMETAARCHIVETYPEMAP_H
#define CPGF_GMETAARCHIVETYPEMAP_H


#include "cpgf/gtypetraits.h"
#include "cpgf/gifelse.h"
#include "cpgf/gvariant.h"


namespace cpgf {


template <typename T>
struct PermanentTypeMap
{
	typedef T Result;
};

template <>
struct PermanentTypeMap <wchar_t>
{
	typedef short Result;
};


template <typename T>
struct PromotedPermenentTypeMap
{
	typedef typename GIfElse<
		IsUnknownSign<T>::Result,
			T,
			typename GIfElse<
				IsFloat<T>::Result,
					long double,
					typename GIfElse<
						IsSigned<T>::Result,
						long long,
						unsigned long long
					>::Result
			>::Result
	>::Result
	Result
	;
};

template <>
struct PromotedPermenentTypeMap <wchar_t>
{
	typedef short Result;
};


enum PermanentType {
	ptBool = 1,
	ptChar = 2,
	ptWchar = 3,
	ptSignedChar = 4,
	ptUnsignedChar = 5,
	ptSignedShort = 6,
	ptUnsignedShort = 7,
	ptSignedInt = 8,
	ptUnsignedInt = 9,
	ptSignedLong = 10,
	ptUnsignedLong = 11,
	ptSignedLongLong = 12,
	ptUnsignedLongLong = 13,
	ptFloat = 14,
	ptDouble = 15,
	ptLongDouble = 16,
	ptObject = 17,
	ptString = 18,
	ptWideString = 19,

	ptNull = 50, // for null pointer
	ptReferenceID = 51, // for one pointer reference to another
	ptClassType = 52, // for class type definition
	
	ptArray = 53,

};

extern int defaultVariantTypeMap[];

GVariantType getVariantTypeFromMap(int * variantTypeMap, int mappedType);
int getMappedTypeFromMap(int * variantTypeMap, GVariantType vt);

bool permanentTypeIsFundamental(PermanentType type);


} // namespace cpgf


#endif

