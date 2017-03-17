#include "cpgf/scriptbind/gscriptvalue.h"
#include "cpgf/scriptbind/gscriptbindapi.h"
#include "cpgf/gmetaapiutil.h"
#include "cpgf/gmetaapi.h"
#include "cpgf/gglobal.h"
#include "cpgf/gscopedinterface.h"

namespace cpgf {


GScriptValue createScriptValueFromData(const GScriptValueData & data)
{
	return GScriptValue(data);
}

GScriptValue::GScriptValue(Type type, const GVariant & value, IMetaItem * metaItem, IScriptValueBindApi * bindApi)
	: type(type), value(value), metaItem(metaItem), bindApi(bindApi)
{
}

GScriptValue::GScriptValue(Type type, const GVariant & value, IMetaItem * metaItem)
	: type(type), value(value), metaItem(metaItem), bindApi()
{
}

GScriptValue::GScriptValue(Type type, const GVariant & value)
	: type(type), value(value), metaItem(), bindApi()
{
}

GScriptValue::GScriptValue()
	: type(typeNull), value(), metaItem(), bindApi()
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
	this->bindApi.reset(data.bindApi);
	if(data.bindApi != nullptr) {
		data.bindApi->releaseReference();
	}
}

GScriptValue::GScriptValue(const GScriptValue & other)
	: type(other.type), value(other.value), metaItem(other.metaItem), bindApi(other.bindApi)
{
}

GScriptValue & GScriptValue::operator = (const GScriptValue & other)
{
	if(this != &other) {
		this->type = other.type;
		this->value = other.value;
		this->metaItem = other.metaItem;
		this->bindApi = other.bindApi;
	}

	return *this;
}

GScriptValueData GScriptValue::takeData()
{
	GScriptValueData data;
	data.type = this->type;
	data.value = this->value.takeData();
	data.metaItem = this->metaItem.take();
	data.bindApi = this->bindApi.take();
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
	data.bindApi = this->bindApi.get();
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

GScriptValue GScriptValue::fromObject(const GVariant & instance, IMetaClass * metaClass, IScriptValueBindApi * bindApi)
{
	GMetaType metaType(metaGetTypedItemMetaType(metaClass));
	metaType.addPointer();
	return GScriptValue(typeObject, createTypedVariant(instance, metaType), metaClass, bindApi);
}

struct GScriptValueDefaultBindApi : public IScriptValueBindApi
{
	G_INTERFACE_IMPL_OBJECT
public:
	GScriptValueDefaultBindApi(bool transferOwnership) : transferOwnership(transferOwnership) {}

	virtual ~GScriptValueDefaultBindApi() {}

	virtual void G_API_CC discardOwnership() {
		transferOwnership = false;
	}

	virtual bool G_API_CC isOwnershipTransferred() {
		return transferOwnership;
	}


private:
	bool transferOwnership;
};

GScriptValue GScriptValue::fromObject(const GVariant & instance, IMetaClass * metaClass, bool transferOwnership)
{
	GScopedInterface<IScriptValueBindApi> bindApi(new GScriptValueDefaultBindApi(transferOwnership));
	return GScriptValue::fromObject(instance, metaClass, bindApi.get());
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
		return nullptr;
	}
}

GVariant GScriptValue::toObject(IMetaClass ** outMetaClass, bool * outTransferOwnership) const
{
	if(outMetaClass != nullptr) {
		*outMetaClass = nullptr;
	}
	if(outTransferOwnership != nullptr) {
		*outTransferOwnership = false;
	}

	if(this->isObject()) {
		if(outMetaClass != nullptr) {
			*outMetaClass = gdynamic_cast<IMetaClass *>(this->metaItem.get());
			this->metaItem->addReference();
		}
		if(outTransferOwnership != nullptr) {
			*outTransferOwnership = bindApi ? bindApi->isOwnershipTransferred() : false;
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
	if (bindApi) {
		bindApi->discardOwnership();
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
		value.toObject(&metaClass, nullptr);
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

