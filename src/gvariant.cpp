#include "cpgf/gvariant.h"
#include "cpgf/gmetatype.h"
#include "cpgf/gerrorcode.h"


namespace cpgf {

namespace variant_internal {


struct IVariantTypedVar : public IObject
{
public:
	virtual void G_API_CC getValue(GVariantData * outValue) = 0;
	virtual void G_API_CC getType(GMetaTypeData * outType) = 0;
};

class GVariantTypedVar : public IVariantTypedVar
{
public:
	GVariantTypedVar(const GVariant & value, const GMetaType & type);
	virtual ~GVariantTypedVar();
	
	G_INTERFACE_IMPL_OBJECT

protected:
	virtual void G_API_CC getValue(GVariantData * outValue);
	virtual void G_API_CC getType(GMetaTypeData * outType);
	
private:
	GVariant value;
	GMetaType type;
};

GVariantTypedVar::GVariantTypedVar(const GVariant & value, const GMetaType & type)
	: value(value), type(type)
{
	fixupMetaType(&this->type);
}

GVariantTypedVar::~GVariantTypedVar()
{
}
	
void G_API_CC GVariantTypedVar::getValue(GVariantData * outValue)
{
	*outValue = this->value.refData();
}

void G_API_CC GVariantTypedVar::getType(GMetaTypeData * outType)
{
	*outType = this->type.refData();
}


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
		case vtTypedVar:
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
	if(! vtIsInteger(vtGetType(var->refData().typeData)) || vtGetSize(var->refData().typeData) > sizeof(unsigned long long)) {
		raiseCoreException(Error_Variant_FailAdjustTypeSize);
	}

	unsigned long long value = var->refData().valueUnsignedLongLong;
	switch(vtGetSize(var->refData().typeData)) {
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


GVariant createVariantFromData(const GVariantData & data)
{
	return GVariant(data);
}

GVariant::GVariant()
{
	vtInit(data.typeData);
	data.holder.a = 0;
	data.holder.b = 0;
	data.holder.c = 0;
}

GVariant::GVariant(const GVariantData & data) : data(data)
{
	retainVariantData(&this->data);

	if(vtGetSize(this->data.typeData) != variant_internal::getVariantTypeSize(static_cast<GVariantType>(vtGetType(this->data.typeData)))) {
		variant_internal::adjustVariantType(this);
	}
}

GVariant::GVariant(const GVariant & other) : data(other.data)
{
	retainVariantData(&this->data);
}

GVariant::~GVariant()
{
	releaseVariantData(&this->data);
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
		vtSetType(v.refData().typeData, (vtGetType(v.refData().typeData) & ~byPointer) | byReference);
	}

	return v;
}

GVariant pointerToObjectVariant(void * p)
{
	GVariant result;

	vtSetType(result.refData().typeData, vtObject | byPointer);
	result.refData().ptrObject = p;
	vtSetSize(result.refData().typeData, sizeof(void *));

	return result;
}

GVariant objectToVariant(void * object)
{
	GVariant result;

	vtSetType(result.refData().typeData, vtObject);
	result.refData().ptrObject = object;
	vtSetSize(result.refData().typeData, sizeof(void *));

	return result;
}

void * objectAddressFromVariant(const GVariant & v)
{
	return fromVariant<void *>(v);
}

void * referenceAddressFromVariant(const GVariant & v)
{
	if(vtIsByReference(v.getType())) {
		return const_cast<void *>(v.refData().ptrObject);
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
	retainVariantData(data);
	return *data;
}

void retainVariantData(GVariantData * data)
{
	if(variant_internal::isVtUsingShadow(vtGetType(data->typeData))) {
		data->shadowObject->addReference();
	}
	else if(vtIsInterface(vtGetType(data->typeData)) && data->valueInterface != NULL) {
		data->valueInterface->addReference();
	}
	else if(vtIsTypedVar(vtGetType(data->typeData)) && data->valueTypedVar != NULL) {
		data->valueTypedVar->addReference();
	}
}

void releaseVariantData(GVariantData * data)
{
	if(variant_internal::isVtUsingShadow(vtGetType(data->typeData))) {
		data->shadowObject->releaseReference();
	}
	else if(vtIsInterface(vtGetType(data->typeData)) && data->valueInterface != NULL) {
		data->valueInterface->releaseReference();
	}
	else if(vtIsTypedVar(vtGetType(data->typeData)) && data->valueTypedVar != NULL) {
		data->valueTypedVar->releaseReference();
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

	initializeVarString(&v.refData(), s);

	return v;
}

bool variantDataIsString(const GVariantData & v)
{
	return vtGetType(v.typeData) == vtString || (vtGetPointers(v.typeData) == 1 && vtGetBaseType(v.typeData) == vtChar);
}

bool variantIsString(const GVariant & v)
{
	return variantDataIsString(v.refData());
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

	initializeVarWideString(&v.refData(), s);

	return v;
}

bool variantDataIsWideString(const GVariantData & v)
{
	return vtGetType(v.typeData) == vtWideString || (vtGetPointers(v.typeData) == 1 && vtGetBaseType(v.typeData) == vtWchar);
}

bool variantIsWideString(const GVariant & v)
{
	return variantDataIsWideString(v.refData());
}


void initializeVarTypedVariant(GVariantData * data, const GVariant & value, const GMetaType & type)
{
	vtInit(data->typeData);
	vtSetType(data->typeData, vtTypedVar);
	vtSetSize(data->typeData, variant_internal::getVariantTypeSize(vtTypedVar));
	vtSetPointers(data->typeData, 0);
	data->valueTypedVar = new variant_internal::GVariantTypedVar(value, type);
}

GVariant createTypedVariant(const GVariant & value, const GMetaType & type)
{
	GVariant v;

	initializeVarTypedVariant(&v.refData(), getVariantRealValue(value), type);

	return v;
}

GVariant getVariantRealValue(const GVariant & value)
{
	if(vtIsTypedVar(value.getType())) {
		GVariantData data;
		value.refData().valueTypedVar->getValue(&data);
		return createVariantFromData(data);
	}
	else {
		return value;
	}
}

GMetaType getVariantRealMetaType(const GVariant & value)
{
	if(vtIsTypedVar(value.getType())) {
		GMetaTypeData type;
		value.refData().valueTypedVar->getType(&type);
		return GMetaType(type);
	}
	else {
		return GMetaType();
	}
}



} // namespace cpgf

