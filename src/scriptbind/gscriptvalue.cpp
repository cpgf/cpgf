#include "cpgf/scriptbind/gscriptvalue.h"
#include "cpgf/scriptbind/gscriptbindapi.h"
#include "cpgf/scriptbind/gscriptbind.h"
#include "cpgf/gmetaapiutil.h"
#include "cpgf/gmetaapi.h"
#include "cpgf/gglobal.h"
#include "cpgf/gscopedinterface.h"

namespace cpgf {


GScriptValue createScriptValueFromData(const GScriptValueData & data)
{
	return GScriptValue(data);
}

GScriptValue::GScriptValue(Type type, const GVariant & value, IMetaItem * metaItem, IScriptValueObjectFlags * objectFlags)
	: type(type), value(value), metaItem(metaItem), objectFlags(objectFlags)
{
}

GScriptValue::GScriptValue(Type type, const GVariant & value, IMetaItem * metaItem)
	: type(type), value(value), metaItem(metaItem), objectFlags()
{
}

GScriptValue::GScriptValue(Type type, const GVariant & value)
	: type(type), value(value), metaItem(), objectFlags()
{
}

GScriptValue::GScriptValue()
	: type(typeNull), value(), metaItem(), objectFlags()
{
}

// This function is defined in gvariant.cpp, for internal use.
GVariant createVariantFromData(const GVariantData & data);

GScriptValue::GScriptValue(const GScriptValueData & data)
{
	this->type = (GScriptValue::Type)(data.type);

//	this->value = createVariantFromData(data.value);
	// must release it since GVariant(data.value) retains the data
//	releaseVariantData(&this->value.refData());
	
	this->value = 0;
	this->value.refData() = data.value;
	
	this->metaItem.reset(data.metaItem);
	if(data.metaItem != nullptr) {
		data.metaItem->releaseReference();
	}
	this->objectFlags.reset(data.objectFlags);
	if(data.objectFlags != nullptr) {
		data.objectFlags->releaseReference();
	}
}

GScriptValue::GScriptValue(const GScriptValue & other)
	: type(other.type), value(other.value), metaItem(other.metaItem), objectFlags(other.objectFlags)
{
}

GScriptValue & GScriptValue::operator = (const GScriptValue & other)
{
	if(this != &other) {
		this->type = other.type;
		this->value = other.value;
		this->metaItem = other.metaItem;
		this->objectFlags = other.objectFlags;
	}

	return *this;
}

GScriptValueData GScriptValue::takeData()
{
	GScriptValueData data;
	data.type = this->type;
	data.value = this->value.takeData();
	data.metaItem = this->metaItem.take();
	data.objectFlags = this->objectFlags.take();
	return data;
}

GScriptValueData GScriptValue::getData() const
{
	GScriptValueData data;
	data.type = this->type;
	data.value = this->value.refData();
	data.metaItem = this->metaItem.get();
	if(data.metaItem != nullptr) {
		data.metaItem->addReference();
	}
	data.objectFlags = this->objectFlags.get();
	return data;
}

GScriptValue GScriptValue::fromNull()
{
	return GScriptValue(typeNull, (void *)0);
}

GScriptValue GScriptValue::fromPrimary(const GVariant & primary)
{
	return GScriptValue(typePrimary, primary);
}

GScriptValue GScriptValue::fromClass(IMetaClass * metaClass)
{
	return GScriptValue(typeClass, metaClass);
}

GScriptValue GScriptValue::fromObject(const GVariant & instance, IMetaClass * metaClass, IScriptValueObjectFlags * objectFlags)
{
	GMetaType metaType(metaGetTypedItemMetaType(metaClass));
	metaType.addPointer();
	return GScriptValue(typeObject, createTypedVariant(instance, metaType), metaClass, objectFlags);
}

struct GScriptValueDefaultBindApi : public IScriptValueObjectFlags
{
	G_INTERFACE_IMPL_OBJECT

public:
	GScriptValueDefaultBindApi(const bool transferOwnership, const GScriptInstanceCv cv)
		:
			transferOwnership(transferOwnership),
			cv(cv)
	{
	}

	virtual ~GScriptValueDefaultBindApi() {}

	virtual void G_API_CC discardOwnership() override {
		transferOwnership = false;
	}

	virtual gapi_bool G_API_CC isOwnershipTransferred() override {
		return transferOwnership;
	}

	virtual int32_t getObjectCv() override {
		return (int32_t)this->cv;
	}

private:
	bool transferOwnership;
	GScriptInstanceCv cv;
};

GScriptValue GScriptValue::fromObject(const GVariant & instance, IMetaClass * metaClass, const bool transferOwnership, const GScriptInstanceCv cv)
{
	GScopedInterface<IScriptValueObjectFlags> objectFlags(new GScriptValueDefaultBindApi(transferOwnership, cv));
	return GScriptValue::fromObject(instance, metaClass, objectFlags.get());
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

GScriptValue GScriptValue::fromScriptArray(IScriptArray * scriptArray)
{
	return GScriptValue(typeScriptArray, scriptArray);
}

void * GScriptValue::toNull() const
{
	return nullptr;
}

GVariant GScriptValue::toPrimary() const
{
	if(this->isPrimary()) {
		return this->value;
	}
	else {
		return GVariant();
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
		return nullptr;
	}
}

GVariant GScriptValue::toObject(IMetaClass ** outMetaClass, bool * outTransferOwnership, GScriptInstanceCv * outCv) const
{
	if(this->isObject()) {
		if(outMetaClass != nullptr) {
			*outMetaClass = gdynamic_cast<IMetaClass *>(this->metaItem.get());
			this->metaItem->addReference();
		}
		if(outTransferOwnership != nullptr) {
			*outTransferOwnership = this->objectFlags ? !! this->objectFlags->isOwnershipTransferred() : false;
		}
		if(outCv != nullptr) {
			*outCv = this->objectFlags ? (GScriptInstanceCv)this->objectFlags->getObjectCv() : GScriptInstanceCv::sicvNone;
		}
		return this->value;
	}
	else {
		if(outMetaClass != nullptr) {
			*outMetaClass = nullptr;
		}
		if(outTransferOwnership != nullptr) {
			*outTransferOwnership = false;
		}
		if(outCv != nullptr) {
			*outCv = GScriptInstanceCv::sicvNone;
		}

		return GVariant();
	}
}

void * GScriptValue::toObjectAddress(IMetaClass ** outMetaClass, bool * outTransferOwnership, GScriptInstanceCv * outCv) const
{
	GVariant instance(this->toObject(outMetaClass, outTransferOwnership, outCv));
	if(! instance.isEmpty()) {
		return fromVariant<void *>(instance);
	}
	else {
		return nullptr;
	}
}

IMetaMethod * GScriptValue::toMethod(void ** outInstance) const
{
	if(outInstance != nullptr) {
		*outInstance = nullptr;
	}

	if(this->isMethod()) {
		IMetaMethod * metaMethod = gdynamic_cast<IMetaMethod *>(this->metaItem.get());
		metaMethod->addReference();
		if(outInstance != nullptr) {
			*outInstance = fromVariant<void *>(this->value);
		}
		return metaMethod;
	}
	else {
		return nullptr;
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
		return nullptr;
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
		return nullptr;
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
	if(outInstance != nullptr) {
		*outInstance = nullptr;
	}

	if(this->isAccessible()) {
		IMetaAccessible * accessible = gdynamic_cast<IMetaAccessible *>(this->metaItem.get());
		accessible->addReference();
		if(outInstance != nullptr) {
			*outInstance = fromVariant<void *>(this->value);
		}
		return accessible;
	}
	else {
		return nullptr;
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
		return nullptr;
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
		return nullptr;
	}
}

IScriptArray * GScriptValue::toScriptArray() const
{
	if(this->isScriptArray()) {
		IScriptArray * scriptArray = fromVariant<IScriptArray *>(this->value);
		scriptArray->addReference();
		return scriptArray;
	}
	else {
		return nullptr;
	}
}

void GScriptValue::discardOwnership()
{
	if (objectFlags) {
		objectFlags->discardOwnership();
	}

}



GScriptValueDataScopedGuard::GScriptValueDataScopedGuard(const GScriptValueData & data)
	: data(data)
{
}

GScriptValueDataScopedGuard::~GScriptValueDataScopedGuard()
{
	if(data.metaItem != nullptr) {
		data.metaItem->releaseReference();
	}
}


IMetaTypedItem * getTypedItemFromScriptValue(const GScriptValue & value)
{
	if(value.isObject()) {
		IMetaClass * metaClass;
		value.toObject(&metaClass, nullptr, nullptr);
		return metaClass;
	}
	else if(value.isClass()) {
		return value.toClass();
	}
	else if(value.isEnum()) {
		return value.toEnum();
	}
	else {
		return nullptr;
	}
}



} // namespace cpgf

