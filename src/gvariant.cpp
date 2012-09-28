#include "cpgf/gvariant.h"


namespace cpgf {

namespace variant_internal {

unsigned int getVariantTypeSize(GVariantType type)
{
	switch(static_cast<int>(type)) {
		case vtEmpty:
		case vtVoid:
			return 0;

		case vtBool:
			return sizeof(bool);

		case vtChar:
			return sizeof(char);

		case vtWchar:
			return sizeof(wchar_t);

		case vtSignedChar:
			return sizeof(signed char);

		case vtUnsignedChar:
			return sizeof(unsigned char);

		case vtSignedShort:
			return sizeof(signed short);

		case vtUnsignedShort:
			return sizeof(unsigned short);

		case vtSignedInt:
			return sizeof(signed int);

		case vtUnsignedInt:
			return sizeof(unsigned int);

		case vtSignedLong:
			return sizeof(signed long);

		case vtUnsignedLong:
			return sizeof(unsigned long);

		case vtSignedLongLong:
			return sizeof(signed long long);

		case vtUnsignedLongLong:
			return sizeof(unsigned long long);

		case vtFloat:
			return sizeof(float);

		case vtDouble:
			return sizeof(double);

		case vtLongDouble:
			return 0; // sizeof(long double); on iOS, long double is 16 bytes which is too big.

		case vtShadow:
		case vtObject:
		case vtPointer:
		case vtString:
		case vtWideString:
		case vtInterface:
			return sizeof(void *);

		default:
			if(vtIsByPointer(type) || vtIsByReference(type)) {
				return sizeof(void *);
			}

	}

	GASSERT_MSG(false, "Can't detect variant type size.");

	return 0;
}

void adjustVariantType(GVariant * var)
{
	if(! vtIsInteger(vtGetType(var->data.typeData)) || vtGetSize(var->data.typeData) > sizeof(unsigned long long)) {
		raiseCoreException(Error_Variant_FailAdjustTypeSize);
	}

	unsigned long long value = var->data.valueUnsignedLongLong;
	switch(vtGetSize(var->data.typeData)) {
		case 1:
			value &= 0xff;
			break;

		case 2:
			value &= 0xffff;
			break;

		case 4:
			value &= 0xffffffffu;
			break;

		case 8:
			value &= 0xffffffffffffffffull;
			break;

		default:
			break;

	}

	InitVariant<true, unsigned long long>(*var, var->getTypeData(), value);
}


} // namespace variant_internal


GVariant::GVariant()
{
	vtInit(data.typeData);
	data.holder.a = 0;
	data.holder.b = 0;
	data.holder.c = 0;
}

GVariant::GVariant(const GVariantData & data) : data(data)
{
	retainVarData(&this->data);

	if(vtGetSize(this->data.typeData) != variant_internal::getVariantTypeSize(static_cast<GVariantType>(vtGetType(this->data.typeData)))) {
		variant_internal::adjustVariantType(this);
	}
}

GVariant::GVariant(const GVariant & other) : data(other.data)
{
	retainVarData(&this->data);
}

GVariant::~GVariant()
{
	freeVarData(&this->data);
}

GVariant & GVariant::operator = (GVariant other)
{
	this->swap(other);

	return *this;
}

void GVariant::swap(GVariant & other)
{
	using std::swap;

	swap(this->data.typeData, other.data.typeData);
	swap(this->data.holder, other.data.holder);
}

GVariantData GVariant::takeData()
{
	GVariantData result = this->data;

	vtInit(this->data.typeData);

	return result;
}


void failedCast()
{
	raiseCoreException(Error_Variant_FailCast);
}

void checkFailCast(bool success)
{
	if(!success) {
		failedCast();
	}
}

GVariant pointerToRefVariant(const GVariant & p)
{
	GVariant v(p);

	if(vtIsByPointer(v.getType())) {
		vtSetType(v.data.typeData, (vtGetType(v.data.typeData) & ~byPointer) | byReference);
	}

	return v;
}

GVariant pointerToObjectVariant(void * p)
{
	GVariant result;

	vtSetType(result.data.typeData, vtObject | byPointer);
	result.data.ptrObject = p;
	vtSetSize(result.data.typeData, sizeof(void *));

	return result;
}

void * objectAddressFromVariant(const GVariant & v)
{
	if(v.getType() == vtShadow) {
		return v.data.shadowObject->getObject();
	}

	return fromVariant<void *>(v);
}

void * referenceAddressFromVariant(const GVariant & v)
{
	if(vtIsByReference(v.getType())) {
		return const_cast<void *>(v.data.ptrObject);
	}
	else {
		return objectAddressFromVariant(v);
	}
}

void initializeVarData(GVariantData * data)
{
	vtInit(data->typeData);
}

GVariantData copyVarData(GVariantData * data)
{
	retainVarData(data);
	return *data;
}

void retainVarData(GVariantData * data)
{
	if(variant_internal::isVtUsingShadow(vtGetType(data->typeData))) {
		data->shadowObject->addReference();
	}
	else if(vtIsInterface(vtGetType(data->typeData)) && data->valueInterface != NULL) {
		data->valueInterface->addReference();
	}
}

void freeVarData(GVariantData * data)
{
	if(variant_internal::isVtUsingShadow(vtGetType(data->typeData))) {
		data->shadowObject->releaseReference();
	}
	else if(vtIsInterface(vtGetType(data->typeData)) && data->valueInterface != NULL) {
		data->valueInterface->releaseReference();
	}
}

void initializeVarString(GVariantData * data, const char * s)
{
	vtInit(data->typeData);
	vtSetType(data->typeData, vtString);
	vtSetSize(data->typeData, variant_internal::getVariantTypeSize(vtString));
	vtSetPointers(data->typeData, 0);
	variant_internal::IVariantShadowObject * shadow = new variant_internal::GVariantShadowObject<std::string>(std::string(s));
	data->shadowObject = shadow;
}

GVariant createStringVariant(const char * s)
{
	GVariant v;

	initializeVarString(&v.data, s);

	return v;
}

bool variantDataIsString(const GVariantData & v)
{
	return vtGetType(v.typeData) == vtString || (vtGetPointers(v.typeData) == 1 && vtGetBaseType(v.typeData) == vtChar);
}

bool variantIsString(const GVariant & v)
{
	return variantDataIsString(v.data);
}

void initializeVarWideString(GVariantData * data, const wchar_t * s)
{
	vtInit(data->typeData);
	vtSetType(data->typeData, vtWideString);
	vtSetSize(data->typeData, variant_internal::getVariantTypeSize(vtWideString));
	vtSetPointers(data->typeData, 0);
	variant_internal::IVariantShadowObject * shadow = new variant_internal::GVariantShadowObject<std::wstring>(std::wstring(s));
	data->shadowObject = shadow;
}

GVariant createWideStringVariant(const wchar_t * s)
{
	GVariant v;

	initializeVarWideString(&v.data, s);

	return v;
}

bool variantDataIsWideString(const GVariantData & v)
{
	return vtGetType(v.typeData) == vtWideString || (vtGetPointers(v.typeData) == 1 && vtGetBaseType(v.typeData) == vtWchar);
}

bool variantIsWideString(const GVariant & v)
{
	return variantDataIsWideString(v.data);
}


} // namespace cpgf

