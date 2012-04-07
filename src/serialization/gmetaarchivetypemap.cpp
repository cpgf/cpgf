#include "cpgf/serialization/gmetaarchivetypemap.h"


namespace cpgf {


int defaultVariantTypeMap[] = {
	vtBool, ptBool,
	vtChar, ptChar,
	vtWchar, ptWchar,
	vtSignedChar, ptSignedChar,
	vtUnsignedChar, ptUnsignedChar,
	vtSignedShort, ptSignedShort,
	vtUnsignedShort, ptUnsignedShort,
	vtSignedInt, ptSignedInt,
	vtUnsignedInt, ptUnsignedInt,
	vtSignedLong, ptSignedLong,
	vtUnsignedLong, ptUnsignedLong,
	vtSignedLongLong, ptSignedLongLong,
	vtUnsignedLongLong, ptUnsignedLongLong,
	vtFloat, ptFloat,
	vtDouble, ptDouble,
	vtLongDouble, ptLongDouble,
	vtObject, ptObject,
	vtString, ptString,
	vtWideString, ptWideString,

	vtEmpty,
};


GVariantType getVariantTypeFromMap(int * variantTypeMap, int mappedType)
{
	while(*variantTypeMap != vtEmpty) {
		if(*(variantTypeMap + 1) == mappedType) {
			return static_cast<GVariantType>(*variantTypeMap);
		}
		variantTypeMap += 2;
	}

	return vtEmpty;
}

int getMappedTypeFromMap(int * variantTypeMap, GVariantType vt)
{
	while(*variantTypeMap != vtEmpty) {
		if(*variantTypeMap == vt) {
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



