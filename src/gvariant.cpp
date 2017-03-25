#include "cpgf/gvariant.h"
#include "cpgf/gmemorypool.h"
#include "cpgf/gmetatype.h"

namespace cpgf {

namespace variant_internal {

struct IVariantTypedVar : public cpgf::IObject
{
public:
	virtual void G_API_CC getValue(GVariantData * outValue) = 0;
	virtual void G_API_CC getType(cpgf::GMetaTypeData * outType) = 0;
};

class GVariantTypedVar : public IVariantTypedVar
{
public:
	void * operator new (const size_t size);
	void operator delete(void * p, size_t size);

	GVariantTypedVar(const GVariant & value, const cpgf::GMetaType & type);
	virtual ~GVariantTypedVar();

	G_INTERFACE_IMPL_OBJECT

protected:
	virtual void G_API_CC getValue(GVariantData * outValue);
	virtual void G_API_CC getType(cpgf::GMetaTypeData * outType);

private:
	GVariant value;
	cpgf::GMetaType type;
};

void * GVariantTypedVar::operator new (const size_t size)
{
	return cpgf::GMemoryPool::getInstance()->allocate(size);
}

void GVariantTypedVar::operator delete(void * p, size_t size)
{
	cpgf::GMemoryPool::getInstance()->free(p, size);
}

GVariantTypedVar::GVariantTypedVar(const GVariant & value, const cpgf::GMetaType & type)
	: value(value), type(type)
{
	cpgf::fixupMetaType(&this->type);
}

GVariantTypedVar::~GVariantTypedVar()
{
}

void G_API_CC GVariantTypedVar::getValue(GVariantData * outValue)
{
	*outValue = this->value.refData();
}

void G_API_CC GVariantTypedVar::getType(cpgf::GMetaTypeData * outType)
{
	*outType = this->type.refData();
}


void retainVariantData(GVariantData & data)
{
	const GVariantType baseType = vtGetBaseType((GVariantType)data.typeData.vt);
	if(baseType >= GVariantType::vtInterfaceBegin
		&& baseType <= GVariantType::vtInterfaceEnd
		&& data.valueInterface != nullptr) {
		data.valueInterface->addReference();
	}
}

void releaseVariantData(GVariantData & data)
{
	const GVariantType baseType = vtGetBaseType((GVariantType)data.typeData.vt);
	if(baseType >= GVariantType::vtInterfaceBegin
		&& baseType <= GVariantType::vtInterfaceEnd
		&& data.valueInterface != nullptr) {
		data.valueInterface->releaseReference();
	}
}

VariantTypeInfo variantTypeInfo[] = {
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

} //namespace variant_internal


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

GVariant createTypedVariant(const GVariant & value, const cpgf::GMetaType & type)
{
	GVariant v;
	GVariantData & data = v.refData();

	data.typeData.vt = (uint16_t)GVariantType::vtTypedVar;
	vtSetSizeAndPointers(data.typeData, sizeof(void *), 0);
	data.valueInterface = new variant_internal::GVariantTypedVar(getVariantRealValue(value), type);

	return v;
}

GVariant getVariantRealValue(const GVariant & value)
{
	if(vtIsTypedVar(value.getType())) {
		GVariantData data;
		static_cast<variant_internal::IVariantTypedVar *>(value.refData().valueInterface)->getValue(&data);
		// IVariantTypedVar::getValue doesn't add reference on interface,
		// so we must call createVariantFromData to do so.
		return createVariantFromData(data);
	}
	else {
		return value;
	}
}

cpgf::GMetaType getVariantRealMetaType(const GVariant & value)
{
	if(vtIsTypedVar(value.getType())) {
		cpgf::GMetaType type;
		static_cast<variant_internal::IVariantTypedVar *>(value.refData().valueInterface)->getType(&type.refData());
		return type;
	}
	else {
		return cpgf::GMetaType();
	}
}

GVariant createObjectVariantFromPointer(void * p)
{
	GVariant result;
	GVariantData & data = result.refData();

	data.typeData.vt = (uint16_t)GVariantType::vtObject | (uint16_t)GVariantType::byPointer;
	data.pointer = p;
	vtSetSizeAndPointers(data.typeData, sizeof(void *), 1);

	return result;
}

GVariant createObjectVariant(void * object)
{
	GVariant result;
	GVariantData & data = result.refData();

	data.typeData.vt = (uint16_t)GVariantType::vtObject;
	data.pointer = object;
	vtSetSizeAndPointers(data.typeData, sizeof(void *), 0);

	return result;
}

GVariant variantPointerToLvalueReference(const GVariant & p)
{
	GVariant v(p);

	if(vtIsByPointer(v.getType())) {
		vtSetType(v.refData().typeData, (v.getType() & ~GVariantType::byPointer) | GVariantType::byLvalueReference);
	}

	return v;
}



} //namespace cpgf

