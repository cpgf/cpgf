#include "cpgf/scriptbind/gscriptvalue.h"
#include "cpgf/scriptbind/gscriptbindapi.h"
#include "cpgf/gmetaapi.h"


namespace cpgf {


GScriptValue::GScriptValue(Type type, const GVariant & value, IMetaClass * metaClass)
	: type(type), value(value), objectMetaClass(metaClass)
{
}

GScriptValue::GScriptValue(Type type, const GVariant & value)
	: type(type), value(value), objectMetaClass()
{
}

GScriptValue::GScriptValue()
	: type(typeUnknown), value(), objectMetaClass()
{
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
	return GScriptValue(typeString, createStringVariant(s));
}

GScriptValue GScriptValue::fromMetaClass(IMetaClass * metaClass)
{
	return GScriptValue(typeMetaClass, metaClass);
}

GScriptValue GScriptValue::fromObject(void * instance, IMetaClass * metaClass)
{
	return GScriptValue(typeObject, instance, metaClass);
}

GScriptValue GScriptValue::fromMethod(IMetaMethod * method)
{
	return GScriptValue(typeMethod, method);
}

GScriptValue GScriptValue::fromOverridedMethods(IMetaList * methods)
{
	return GScriptValue(typeOverridedMethods, methods);
}

GScriptValue GScriptValue::fromEnum(IMetaEnum * metaEnum)
{
	return GScriptValue(typeEnum, metaEnum);
}

GScriptValue GScriptValue::fromRaw(const GVariant & raw)
{
	return GScriptValue(typeRaw, raw);
}

GScriptValue GScriptValue::fromScriptObject(IScriptObject * scriptObject)
{
	return GScriptValue(typeScriptObject, scriptObject);
}

GScriptValue GScriptValue::fromScriptMethod(IScriptFunction * scriptFunction)
{
	return GScriptValue(typeScriptMethod, scriptFunction);
}

void checkConvertible(bool can)
{
}

void * GScriptValue::toNull() const
{
	checkConvertible(this->isNull());

	return NULL;
}

GVariant GScriptValue::toFundamental() const
{
	checkConvertible(this->isFundamental());

	return this->value;
}

std::string GScriptValue::toString() const
{
	checkConvertible(this->isString());

	return fromVariant<char *>(this->value);
}

IMetaClass * GScriptValue::toMetaClass() const
{
	checkConvertible(this->isMetaClass());

	IMetaClass * metaClass = fromVariant<IMetaClass *>(this->value);
	metaClass->addReference();
	return metaClass;
}

void * GScriptValue::toObject(IMetaClass ** metaClass) const
{
	checkConvertible(this->isObject());

	if(metaClass != NULL) {
		*metaClass = this->objectMetaClass.get();
		this->objectMetaClass->addReference();
	}

	return fromVariant<void *>(this->value);
}

IMetaMethod * GScriptValue::toMethod() const
{
	checkConvertible(this->isMethod());

	IMetaMethod * metaMethod = fromVariant<IMetaMethod *>(this->value);
	metaMethod->addReference();
	return metaMethod;
}

IMetaList * GScriptValue::toOverridedMethods() const
{
	checkConvertible(this->isOverridedMethods());

	IMetaList * metaList = fromVariant<IMetaList *>(this->value);
	metaList->addReference();
	return metaList;
}

IMetaEnum * GScriptValue::toEnum() const
{
	checkConvertible(this->isEnum());

	IMetaEnum * metaEnum = fromVariant<IMetaEnum *>(this->value);
	metaEnum->addReference();
	return metaEnum;
}

GVariant GScriptValue::toRaw() const
{
	checkConvertible(this->isRaw());

	return this->value;
}

IScriptObject * GScriptValue::toScriptObject() const
{
	checkConvertible(this->isScriptObject());

	IScriptObject * scriptObject = fromVariant<IScriptObject *>(this->value);
	scriptObject->addReference();
	return scriptObject;
}

IScriptFunction * GScriptValue::toScriptMethod() const
{
	checkConvertible(this->isScriptMethod());

	IScriptFunction * scriptFunction = fromVariant<IScriptFunction *>(this->value);
	scriptFunction->addReference();
	return scriptFunction;
}



} // namespace cpgf

