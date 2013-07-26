#include "cpgf/scriptbind/gscriptvalue.h"
#include "cpgf/scriptbind/gscriptbindapi.h"
#include "cpgf/gmetaapiutil.h"
#include "cpgf/gmetaapi.h"
#include "cpgf/gglobal.h"


namespace cpgf {


GScriptValue createScriptValueFromData(const GScriptValueData & data)
{
	return GScriptValue(data);
}

GScriptValue::GScriptValue(Type type, const GVariant & value, IMetaItem * metaItem, bool transferOwnership)
	: type(type), value(value), metaItem(metaItem), flags()
{
	this->flags.setByBool(vfTransferOwnership, transferOwnership);
}

GScriptValue::GScriptValue(Type type, const GVariant & value, IMetaItem * metaItem)
	: type(type), value(value), metaItem(metaItem), flags()
{
}

GScriptValue::GScriptValue(Type type, const GVariant & value)
	: type(type), value(value), metaItem(), flags()
{
}

GScriptValue::GScriptValue()
	: type(typeNull), value(), metaItem(), flags()
{
}

// This function is defined in gvariant.cpp, for internal use.
GVariant createVariantFromData(const GVariantData & data);

GScriptValue::GScriptValue(const GScriptValueData & data)
{
	this->type = (GScriptValue::Type)(data.type);
	this->value = createVariantFromData(data.value);
	// must release it since GVariant(data.value) retains the data
	releaseVariantData(&this->value.refData());
	this->metaItem.reset(data.metaItem);
	if(data.metaItem != NULL) {
		data.metaItem->releaseReference();
	}
	this->flags = data.flags;
}

GScriptValue::GScriptValue(const GScriptValue & other)
	: type(other.type), value(other.value), metaItem(other.metaItem), flags(other.flags)
{
}

GScriptValue & GScriptValue::operator = (const GScriptValue & other)
{
	if(this != &other) {
		this->type = other.type;
		this->value = other.value;
		this->metaItem = other.metaItem;
		this->flags = other.flags;
	}
	
	return *this;
}

GScriptValueData GScriptValue::takeData()
{
	GScriptValueData data;
	data.type = this->type;
	data.value = this->value.takeData();
	data.metaItem = this->metaItem.take();
	data.flags = this->flags;
	return data;
}

GScriptValue GScriptValue::fromNull()
{
	return GScriptValue(typeNull, (void *)0);
}

GScriptValue GScriptValue::fromFundamental(const GVariant & fundamental)
{
	return GScriptValue(typeFundamental, fundamental);
}

GScriptValue GScriptValue::fromString(const char * s)
{
	return GScriptValue(typeString, s);
}

GScriptValue GScriptValue::fromAndCopyString(const char * s)
{
	return GScriptValue(typeString, createStringVariant(s));
}

GScriptValue GScriptValue::fromClass(IMetaClass * metaClass)
{
	return GScriptValue(typeClass, metaClass);
}

GScriptValue GScriptValue::fromObject(const GVariant & instance, IMetaClass * metaClass, bool transferOwnership)
{
	GMetaType metaType(metaGetTypedItemMetaType(metaClass));
	metaType.addPointer();
	return GScriptValue(typeObject, createTypedVariant(instance, metaType), metaClass, transferOwnership);
}

GScriptValue GScriptValue::fromMethod(void * instance, IMetaMethod * method)
{
	return GScriptValue(typeMethod, instance, method);
}

GScriptValue GScriptValue::fromOverloadedMethods(IMetaList * methods)
{
	return GScriptValue(typeOverloadedMethods, methods);
}

GScriptValue GScriptValue::fromEnum(IMetaEnum * metaEnum)
{
	return GScriptValue(typeEnum, metaEnum);
}

GScriptValue GScriptValue::fromRaw(const GVariant & raw)
{
	return GScriptValue(typeRaw, raw);
}

GScriptValue GScriptValue::fromAccessible(void * instance, IMetaAccessible * accessible)
{
	return GScriptValue(typeAccessible, instance, accessible);
}

GScriptValue GScriptValue::fromScriptObject(IScriptObject * scriptObject)
{
	return GScriptValue(typeScriptObject, scriptObject);
}

GScriptValue GScriptValue::fromScriptFunction(IScriptFunction * scriptFunction)
{
	return GScriptValue(typeScriptFunction, scriptFunction);
}

void * GScriptValue::toNull() const
{
	return NULL;
}

GVariant GScriptValue::toFundamental() const
{
	if(this->isFundamental()) {
		return this->value;
	}
	else {
		return GVariant();
	}
}

std::string GScriptValue::toString() const
{
	if(this->isString()) {
		return fromVariant<char *>(this->value);
	}
	else {
		return "";
	}
}

IMetaClass * GScriptValue::toClass() const
{
	if(this->isClass()) {
		IMetaClass * metaClass = fromVariant<IMetaClass *>(this->value);
		metaClass->addReference();
		return metaClass;
	}
	else {
		return NULL;
	}
}

GVariant GScriptValue::toObject(IMetaClass ** outMetaClass, bool * outTransferOwnership) const
{
	if(outMetaClass != NULL) {
		*outMetaClass = NULL;
	}
	if(outTransferOwnership != NULL) {
		*outTransferOwnership = NULL;
	}

	if(this->isObject()) {
		if(outMetaClass != NULL) {
			*outMetaClass = gdynamic_cast<IMetaClass *>(this->metaItem.get());
			this->metaItem->addReference();
		}
		if(outTransferOwnership != NULL) {
			*outTransferOwnership = this->flags.has(vfTransferOwnership);
		}
		return this->value;
	}
	else {
		return GVariant();
	}
}

void * GScriptValue::toObjectAddress(IMetaClass ** outMetaClass, bool * outTransferOwnership) const
{
	GVariant instance(this->toObject(outMetaClass, outTransferOwnership));
	if(! instance.isEmpty()) {
		return fromVariant<void *>(instance);
	}
	else {
		return NULL;
	}
}

IMetaMethod * GScriptValue::toMethod(void ** outInstance) const
{
	if(outInstance != NULL) {
		*outInstance = NULL;
	}

	if(this->isMethod()) {
		IMetaMethod * metaMethod = gdynamic_cast<IMetaMethod *>(this->metaItem.get());
		metaMethod->addReference();
		if(outInstance != NULL) {
			*outInstance = fromVariant<void *>(this->value);
		}
		return metaMethod;
	}
	else {
		return NULL;
	}
}

IMetaList * GScriptValue::toOverloadedMethods() const
{
	if(this->isOverloadedMethods()) {
		IMetaList * metaList = fromVariant<IMetaList *>(this->value);
		metaList->addReference();
		return metaList;
	}
	else {
		return NULL;
	}
}

IMetaEnum * GScriptValue::toEnum() const
{
	if(this->isEnum()) {
		IMetaEnum * metaEnum = fromVariant<IMetaEnum *>(this->value);
		metaEnum->addReference();
		return metaEnum;
	}
	else {
		return NULL;
	}
}

GVariant GScriptValue::toRaw() const
{
	if(this->isRaw()) {
		return this->value;
	}
	else {
		return GVariant();
	}
}

IMetaAccessible * GScriptValue::toAccessible(void ** outInstance) const
{
	if(outInstance != NULL) {
		*outInstance = NULL;
	}

	if(this->isAccessible()) {
		IMetaAccessible * accessible = gdynamic_cast<IMetaAccessible *>(this->metaItem.get());
		accessible->addReference();
		if(outInstance != NULL) {
			*outInstance = fromVariant<void *>(this->value);
		}
		return accessible;
	}
	else {
		return NULL;
	}
}

IScriptObject * GScriptValue::toScriptObject() const
{
	if(this->isScriptObject()) {
		IScriptObject * scriptObject = fromVariant<IScriptObject *>(this->value);
		scriptObject->addReference();
		return scriptObject;
	}
	else {
		return NULL;
	}
}

IScriptFunction * GScriptValue::toScriptFunction() const
{
	if(this->isScriptFunction()) {
		IScriptFunction * scriptFunction = fromVariant<IScriptFunction *>(this->value);
		scriptFunction->addReference();
		return scriptFunction;
	}
	else {
		return NULL;
	}
}

IMetaTypedItem * getTypedItemFromScriptValue(const GScriptValue & value)
{
	if(value.isObject()) {
		IMetaClass * metaClass;
		value.toObject(&metaClass, NULL);
		return metaClass;
	}
	else if(value.isClass()) {
		return value.toClass();
	}
	else if(value.isEnum()) {
		return value.toEnum();
	}
	else {
		return NULL;
	}
}


} // namespace cpgf

