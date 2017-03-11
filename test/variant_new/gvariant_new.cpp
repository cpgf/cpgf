#include "gvariant.h"

namespace xcpgf {

VariantTypeInfo variantTypeInfo[] = {
	{ 0 }, // vtEmpty
	{ 0 }, // vtVoid

	{ sizeof(bool) }, // vtBool
	{ sizeof(char) },
	{ sizeof(wchar_t) },
	{ sizeof(signed char) },
	{ sizeof(unsigned char) },
	{ sizeof(signed short) },
	{ sizeof(unsigned short) },
	{ sizeof(signed int) },
	{ sizeof(unsigned int) },
	{ sizeof(signed long) },
	{ sizeof(unsigned long) },
	{ sizeof(signed long long) },
	{ sizeof(unsigned long long) },
	{ sizeof(float) },
	{ sizeof(double) },
	{ sizeof(long double) },

	{ sizeof(void *) },
	{ sizeof(void *) },
	{ sizeof(void *) },
	{ sizeof(void *) },
	{ sizeof(void *) },
	{ sizeof(void *) },
	{ sizeof(void *) },
};

void retainVariantData(GVariantData * data)
{
	if(data->typeData.vt >= (uint16_t)GVariantType::vtInterfaceBegin && data->typeData.vt <= (uint16_t)GVariantType::vtInterfaceEnd) {
		data->valueInterface->addReference();
	}
}

void releaseVariantData(GVariantData * data)
{
	if(data->typeData.vt >= (uint16_t)GVariantType::vtInterfaceBegin && data->typeData.vt <= (uint16_t)GVariantType::vtInterfaceEnd) {
		data->valueInterface->releaseReference();
	}
}

GVariant createStringVariant(const char * s)
{
	GVariant v;
	GVariantData & data = v.refData();

	data.typeData.vt = (uint16_t)GVariantType::vtString;
	vtSetSizeAndPointers(data.typeData, sizeof(void *), 0);
	data.valueInterface = new variant_internal::GVariantShadowObject<std::string>(std::string(s));

	return v;
}

GVariant createWideStringVariant(const wchar_t * s)
{
	GVariant v;
	GVariantData & data = v.refData();

	data.typeData.vt = (uint16_t)GVariantType::vtWideString;
	vtSetSizeAndPointers(data.typeData, sizeof(void *), 0);
	data.valueInterface = new variant_internal::GVariantShadowObject<std::wstring>(std::wstring(s));

	return v;
}


} //namespace cpgf

