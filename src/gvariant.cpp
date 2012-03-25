#include "cpgf/gvariant.h"


namespace cpgf {

namespace variant_internal {

unsigned int getVariantTypeSize(GVariantType type)
{
	switch(static_cast<int>(type)) {
		case vtEmpty:
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
			return sizeof(long double);

		case vtShadow:
		case vtObject:
		case vtPointer:
		case vtString:
		case vtWideString:
		case vtInterface:
		case vtByteArray:
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
	this->init();

	if(vtGetSize(this->data.typeData) != variant_internal::getVariantTypeSize(static_cast<GVariantType>(vtGetType(this->data.typeData)))) {
		variant_internal::adjustVariantType(this);
	}
}

GVariant::GVariant(const GVariant & other) : data(other.data)
{
	this->init();
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

void GVariant::init()
{
	if(variant_internal::isVtUsingShadow(vtGetType(this->data.typeData))) {
		this->data.shadowObject->retain();
	}
	else if(vtIsInterface(vtGetType(this->data.typeData)) && this->data.valueInterface != NULL) {
		this->data.valueInterface->addReference();
	}
	else if(vtIsByteArray(vtGetType(this->data.typeData)) && this->data.valueByteArray != NULL) {
		this->data.valueByteArray->addReference();
	}
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

void freeVarData(GVariantData * data)
{
	if(variant_internal::isVtUsingShadow(vtGetType(data->typeData))) {
		data->shadowObject->release();
	}
	else if(vtIsInterface(vtGetType(data->typeData)) && data->valueInterface != NULL) {
		data->valueInterface->releaseReference();
	}
	else if(vtIsByteArray(vtGetType(data->typeData)) && data->valueByteArray != NULL) {
		data->valueByteArray->releaseReference();
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

bool variantIsString(const GVariant & v)
{
	return v.getType() == vtString || (v.getPointers() == 1 && v.getBaseType() == vtChar);
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

bool variantIsWideString(const GVariant & v)
{
	return v.getType() == vtWideString || (v.getPointers() == 1 && v.getBaseType() == vtWchar);
}


} // namespace cpgf

