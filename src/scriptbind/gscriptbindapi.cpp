#include "cpgf/gapiutil.h"
#include "cpgf/scriptbind/gscriptbindutil.h"
#include "../pinclude/gscriptbindapiimpl.h"


namespace cpgf {


// This function is defined in gvariant.cpp, for internal use.
GVariant createVariantFromData(const GVariantData & data);

ImplScriptConfig::ImplScriptConfig()
{
}

ImplScriptConfig::ImplScriptConfig(GScriptConfig config) : config(config)
{
}

ImplScriptConfig::~ImplScriptConfig()
{
}

void G_API_CC ImplScriptConfig::setAccessStaticMethodViaInstance(gapi_bool set)
{
	this->config.setAccessStaticMethodViaInstance(set != 0);
}

gapi_bool G_API_CC ImplScriptConfig::allowAccessStaticMethodViaInstance()
{
	return this->config.allowAccessStaticMethodViaInstance();
}

void G_API_CC ImplScriptConfig::setAccessEnumTypeViaInstance(gapi_bool set)
{
	this->config.setAccessEnumTypeViaInstance(set != 0);
}

gapi_bool G_API_CC ImplScriptConfig::allowAccessEnumTypeViaInstance()
{
	return this->config.allowAccessEnumTypeViaInstance();
}

void G_API_CC ImplScriptConfig::setAccessEnumValueViaInstance(gapi_bool set)
{
	this->config.setAccessEnumValueViaInstance(set != 0);
}

gapi_bool G_API_CC ImplScriptConfig::allowAccessEnumValueViaInstance()
{
	return this->config.allowAccessEnumValueViaInstance();
}

void G_API_CC ImplScriptConfig::setAccessClassViaInstance(gapi_bool set)
{
	this->config.setAccessClassViaInstance(set != 0);
}

gapi_bool G_API_CC ImplScriptConfig::allowAccessClassViaInstance()
{
	return this->config.allowAccessClassViaInstance();
}



ImplScriptFunction::ImplScriptFunction(GScriptFunction * scriptFunction, bool freeFunction)
	: scriptFunction(scriptFunction), freeFunction(freeFunction)
{
}

ImplScriptFunction::~ImplScriptFunction()
{
	if(this->freeFunction) {
		delete this->scriptFunction;
	}
}

void G_API_CC ImplScriptFunction::invoke(GVariantData * outResult, const GVariantData * params, uint32_t paramCount)
{
	ENTER_BINDING_API()

	const GVariantData * paramIndirect[REF_MAX_ARITY];

	for(uint32_t i = 0; i < paramCount; ++i) {
		paramIndirect[i] = &params[i];
	}

	this->invokeIndirectly(outResult, paramIndirect, paramCount);

	LEAVE_BINDING_API()
}

void G_API_CC ImplScriptFunction::invokeIndirectly(GVariantData * outResult, GVariantData const * const * params, uint32_t paramCount)
{
	ENTER_BINDING_API()

	GVariant paramVariants[REF_MAX_ARITY];
	const GVariant * paramIndirect[REF_MAX_ARITY];

	for(uint32_t i = 0; i < paramCount; ++i) {
		paramVariants[i] = createVariantFromData(*params[i]);
		paramIndirect[i] = &paramVariants[i];
	}

	GVariant result = this->scriptFunction->invokeIndirectly(paramIndirect, paramCount);
	if(outResult) {
		*outResult = result.takeData();
	}

	LEAVE_BINDING_API()
}


ImplScriptObject::ImplScriptObject(GScriptObject * scriptObject, bool freeObject)
	: scriptObject(scriptObject), freeObject(freeObject)
{
}

ImplScriptObject::~ImplScriptObject()
{
	if(this->freeObject) {
		delete this->scriptObject;
	}
}

IScriptConfig * G_API_CC ImplScriptObject::getConfig()
{
	return new ImplScriptConfig(this->scriptObject->getConfig());
}

IScriptObject * G_API_CC ImplScriptObject::getOwner()
{
	ENTER_BINDING_API()

	GScriptObject * owner = this->scriptObject->getOwner();
	if(owner == NULL) {
		return NULL;
	}
	else {
		return new ImplScriptObject(owner, false);
	}

	LEAVE_BINDING_API(return NULL)
}

gapi_bool G_API_CC ImplScriptObject::isGlobal()
{
	ENTER_BINDING_API()

	return this->scriptObject->isGlobal();

	LEAVE_BINDING_API(return false)
}

void G_API_CC ImplScriptObject::getValue(GScriptValueData * outResult, const char * name)
{
	GScriptValue value(this->scriptObject->getValue(name));
	*outResult = value.takeData();
}

// This function is defined in gscriptvalue.cpp internally.
GScriptValue createScriptValueFromData(const GScriptValueData & data);

void G_API_CC ImplScriptObject::setValue(const char * name, const GScriptValueData * value)
{
	GScriptValue scriptValue(createScriptValueFromData(*value));
	this->scriptObject->setValue(name, scriptValue);
}

uint32_t G_API_CC ImplScriptObject::getType(const char * name, IMetaTypedItem ** outMetaTypeItem)
{
	ENTER_BINDING_API()

	GScriptValue value(this->scriptObject->getValue(name));
	if(outMetaTypeItem != NULL) {
		*outMetaTypeItem = getTypedItemFromScriptValue(value);
	}
	return value.getType();

	LEAVE_BINDING_API(return GScriptValue::typeNull)
}

void G_API_CC ImplScriptObject::bindClass(const char * name, IMetaClass * metaClass)
{
	ENTER_BINDING_API()

	scriptSetValue(this->scriptObject, name,
		GScriptValue::fromClass(metaClass));

	LEAVE_BINDING_API()
}

void G_API_CC ImplScriptObject::bindEnum(const char * name, IMetaEnum * metaEnum)
{
	ENTER_BINDING_API()

	scriptSetValue(this->scriptObject, name,
		GScriptValue::fromEnum(metaEnum));

	LEAVE_BINDING_API()
}

void G_API_CC ImplScriptObject::bindFundamental(const char * name, const GVariantData * value)
{
	ENTER_BINDING_API()

	scriptSetValue(this->scriptObject, name,
		GScriptValue::fromFundamental(createVariantFromData(*value)));

	LEAVE_BINDING_API()
}

void G_API_CC ImplScriptObject::bindAccessible(const char * name, void * instance, IMetaAccessible * accessible)
{
	ENTER_BINDING_API()

	scriptSetValue(this->scriptObject, name,
		GScriptValue::fromAccessible(instance, accessible));

	LEAVE_BINDING_API()
}

void G_API_CC ImplScriptObject::bindString(const char * stringName, const char * s)
{
	ENTER_BINDING_API()

	scriptSetValue(this->scriptObject, stringName,
		GScriptValue::fromString(s));

	LEAVE_BINDING_API()
}

void G_API_CC ImplScriptObject::bindObject(const char * objectName, void * instance, IMetaClass * type, gapi_bool transferOwnership)
{
	ENTER_BINDING_API()

	scriptSetValue(this->scriptObject, objectName,
		GScriptValue::fromObject(instance, type, !! transferOwnership));

	LEAVE_BINDING_API()
}

void G_API_CC ImplScriptObject::bindRaw(const char * name, const GVariantData * value)
{
	ENTER_BINDING_API()

	scriptSetValue(this->scriptObject, name,
		GScriptValue::fromRaw(createVariantFromData(*value)));

	LEAVE_BINDING_API()
}

void G_API_CC ImplScriptObject::bindMethod(const char * name, void * instance, IMetaMethod * method)
{
	ENTER_BINDING_API()

	scriptSetValue(this->scriptObject, name,
		GScriptValue::fromMethod(instance, method));

	LEAVE_BINDING_API()
}

void G_API_CC ImplScriptObject::bindMethodList(const char * name, IMetaList * methodList)
{
	ENTER_BINDING_API()

	scriptSetValue(this->scriptObject, name,
		GScriptValue::fromOverloadedMethods(methodList));

	LEAVE_BINDING_API()
}

IMetaClass * G_API_CC ImplScriptObject::getClass(const char * className)
{
	ENTER_BINDING_API()

	return this->scriptObject->getValue(className).toClass();

	LEAVE_BINDING_API(return NULL)
}

IMetaEnum * G_API_CC ImplScriptObject::getEnum(const char * enumName)
{
	ENTER_BINDING_API()

	return this->scriptObject->getValue(enumName).toEnum();

	LEAVE_BINDING_API(return NULL)
}

void G_API_CC ImplScriptObject::getFundamental(GVariantData * outResult, const char * name)
{
	ENTER_BINDING_API()

	*outResult = this->scriptObject->getValue(name).toFundamental().takeData();

	LEAVE_BINDING_API()
}

char * G_API_CC ImplScriptObject::getString(const char * stringName, IMemoryAllocator * allocator)
{
	ENTER_BINDING_API()

	std::string s = this->scriptObject->getValue(stringName).toString();

	void * cs = allocator->allocate(static_cast<uint32_t>(s.length() + 1));
	memmove(cs, s.c_str(), s.length() + 1);
	
	return static_cast<char *>(cs);

	LEAVE_BINDING_API(return NULL)
}

void * G_API_CC ImplScriptObject::getObject(const char * objectName)
{
	ENTER_BINDING_API()

	return objectAddressFromVariant(this->scriptObject->getValue(objectName).toObject(NULL, NULL));

	LEAVE_BINDING_API(return NULL)
}

void G_API_CC ImplScriptObject::getRaw(GVariantData * outResult, const char * name)
{
	ENTER_BINDING_API()

	*outResult = this->scriptObject->getValue(name).toRaw().takeData();

	LEAVE_BINDING_API()
}

IMetaMethod * G_API_CC ImplScriptObject::getMethod(const char * methodName, void ** outInstance)
{
	ENTER_BINDING_API()

	return this->scriptObject->getValue(methodName).toMethod(outInstance);

	LEAVE_BINDING_API(return NULL)
}

IMetaList * G_API_CC ImplScriptObject::getMethodList(const char * methodName)
{
	ENTER_BINDING_API()

	return this->scriptObject->getValue(methodName).toOverloadedMethods();

	LEAVE_BINDING_API(return NULL)
}

void G_API_CC ImplScriptObject::createScriptObject(GScriptValueData * outResult, const char * name)
{
	ENTER_BINDING_API()

	*outResult = this->scriptObject->createScriptObject(name).takeData();

	LEAVE_BINDING_API()
}

void G_API_CC ImplScriptObject::getScriptFunction(GScriptValueData * outResult, const char * name)
{
	ENTER_BINDING_API()

	*outResult = this->scriptObject->getScriptFunction(name).takeData();

	LEAVE_BINDING_API()
}

void G_API_CC ImplScriptObject::invoke(GVariantData * outResult, const char * name, const GVariantData * params, uint32_t paramCount)
{
	ENTER_BINDING_API()

	const GVariantData * paramIndirect[REF_MAX_ARITY];

	for(uint32_t i = 0; i < paramCount; ++i) {
		paramIndirect[i] = &params[i];
	}

	this->invokeIndirectly(outResult, name, paramIndirect, paramCount);

	LEAVE_BINDING_API()
}

void G_API_CC ImplScriptObject::invokeIndirectly(GVariantData * outResult, const char * name, GVariantData const * const * params, uint32_t paramCount)
{
	ENTER_BINDING_API()

	GVariant paramVariants[REF_MAX_ARITY];
	const GVariant * paramIndirect[REF_MAX_ARITY];

	for(uint32_t i = 0; i < paramCount; ++i) {
		paramVariants[i] = createVariantFromData(*params[i]);
		paramIndirect[i] = &paramVariants[i];
	}

	GVariant result = this->scriptObject->invokeIndirectly(name, paramIndirect, paramCount);
	if(outResult) {
		*outResult = result.takeData();
	}

	LEAVE_BINDING_API()
}

void G_API_CC ImplScriptObject::assignValue(const char * fromName, const char * toName)
{
	ENTER_BINDING_API()

	this->scriptObject->assignValue(fromName, toName);

	LEAVE_BINDING_API()
}

gapi_bool G_API_CC ImplScriptObject::valueIsNull(const char * name)
{
	ENTER_BINDING_API()

	return this->scriptObject->getValue(name).isNull();

	LEAVE_BINDING_API(return false)
}

void G_API_CC ImplScriptObject::nullifyValue(const char * name)
{
	ENTER_BINDING_API()

	this->scriptObject->setValue(name, GScriptValue::fromNull());

	LEAVE_BINDING_API()
}

void G_API_CC ImplScriptObject::bindCoreService(const char * name, IScriptLibraryLoader * libraryLoader)
{
	ENTER_BINDING_API()

	this->scriptObject->bindCoreService(name, libraryLoader);

	LEAVE_BINDING_API()
}

void G_API_CC ImplScriptObject::holdObject(IObject * object)
{
	ENTER_BINDING_API()

	this->scriptObject->holdObject(object);

	LEAVE_BINDING_API()
}


} // namespace cpfg

