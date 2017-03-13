#include "cpgf/serialization/gmetaarchivetypemap.h"


namespace cpgf {


int defaultVariantTypeMap[] = {
	(int)GVariantType::vtBool, ptBool,
	(int)GVariantType::vtChar, ptChar,
	(int)GVariantType::vtWchar, ptWchar,
	(int)GVariantType::vtSignedChar, ptSignedChar,
	(int)GVariantType::vtUnsignedChar, ptUnsignedChar,
	(int)GVariantType::vtSignedShort, ptSignedShort,
	(int)GVariantType::vtUnsignedShort, ptUnsignedShort,
	(int)GVariantType::vtSignedInt, ptSignedInt,
	(int)GVariantType::vtUnsignedInt, ptUnsignedInt,
	(int)GVariantType::vtSignedLong, ptSignedLong,
	(int)GVariantType::vtUnsignedLong, ptUnsignedLong,
	(int)GVariantType::vtSignedLongLong, ptSignedLongLong,
	(int)GVariantType::vtUnsignedLongLong, ptUnsignedLongLong,
	(int)GVariantType::vtFloat, ptFloat,
	(int)GVariantType::vtDouble, ptDouble,
	(int)GVariantType::vtLongDouble, ptLongDouble,
	(int)GVariantType::vtObject, ptObject,
	(int)GVariantType::vtString, ptString,
	(int)GVariantType::vtWideString, ptWideString,

	(int)GVariantType::vtEmpty,
};


GVariantType getVariantTypeFromMap(int * variantTypeMap, int mappedType)
{
	while(*variantTypeMap != (int)GVariantType::vtEmpty) {
		if(*(variantTypeMap + 1) == mappedType) {
			return static_cast<GVariantType>(*variantTypeMap);
		}
		variantTypeMap += 2;
	}

	return GVariantType::vtEmpty;
}

int getMappedTypeFromMap(int * variantTypeMap, GVariantType vt)
{
	while(*variantTypeMap != (int)GVariantType::vtEmpty) {
		if(*variantTypeMap == (int)vt) {
			return *(variantTypeMap + 1);
		}
		variantTypeMap += 2;
	}

	return -1;
}

bool permanentTypeIsFundamental(PermanentType type)
{
	return type >= ptBool && type <= ptLongDouble;
}


} // namespace cpgf



