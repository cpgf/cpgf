#include "../pinclude/gapiimpl.h"
#include "../pinclude/gscriptbindapiimpl.h"


namespace cpgf {


GScriptName & unwrapScriptName(IScriptName * scriptName)
{
	return static_cast<ImplScriptName *>(scriptName)->getScriptName();
}


ImplScriptConfig::ImplScriptConfig()
{
}

ImplScriptConfig::ImplScriptConfig(GScriptConfig config) : config(config)
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



ImplScriptName::ImplScriptName(GScriptName * scriptName) : scriptName(scriptName)
{
}
	
const char * G_API_CC ImplScriptName::getName()
{
	return this->scriptName->getName();
}



ImplScriptObject::ImplScriptObject(GScriptObject * scriptObject) : scriptObject(scriptObject)
{
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
		return new ImplScriptObject(owner);
	}

	LEAVE_BINDING_API(return NULL)
}

gapi_bool G_API_CC ImplScriptObject::isGlobal()
{
	ENTER_BINDING_API()

	return this->scriptObject->isGlobal();

	LEAVE_BINDING_API(return false)
}

IScriptName * G_API_CC ImplScriptObject::createName(const char * name)
{
	ENTER_BINDING_API()

	return new ImplScriptName(new GScriptName(name));

	LEAVE_BINDING_API(return NULL)
}

gapi_bool G_API_CC ImplScriptObject::cacheName(IScriptName * name)
{
	ENTER_BINDING_API()

	return this->scriptObject->cacheName(&unwrapScriptName(name));

	LEAVE_BINDING_API(return false)
}

uint32_t G_API_CC ImplScriptObject::getType(IScriptName * name, IMetaTypedItem ** outMetaTypeItem)
{
	ENTER_BINDING_API()

	return this->scriptObject->getType(unwrapScriptName(name), outMetaTypeItem);

	LEAVE_BINDING_API(return sdtUnknown)
}

void G_API_CC ImplScriptObject::bindClass(IScriptName * name, IMetaClass * metaClass)
{
	ENTER_BINDING_API()

	this->scriptObject->bindClass(unwrapScriptName(name), metaClass);

	LEAVE_BINDING_API()
}

void G_API_CC ImplScriptObject::bindEnum(IScriptName * name, IMetaEnum * metaEnum)
{
	ENTER_BINDING_API()

	this->scriptObject->bindEnum(unwrapScriptName(name), metaEnum);

	LEAVE_BINDING_API()
}

void G_API_CC ImplScriptObject::bindFundamental(IScriptName * name, const GVariantData * value)
{
	ENTER_BINDING_API()

	this->scriptObject->bindFundamental(unwrapScriptName(name), GVariant(*value));

	LEAVE_BINDING_API()
}

void G_API_CC ImplScriptObject::bindString(IScriptName * stringName, const char * s)
{
	ENTER_BINDING_API()

	this->scriptObject->bindString(unwrapScriptName(stringName), s);

	LEAVE_BINDING_API()
}

void G_API_CC ImplScriptObject::bindObject(IScriptName * objectName, void * instance, IMetaClass * type, gapi_bool transferOwnership)
{
	ENTER_BINDING_API()

	this->scriptObject->bindObject(unwrapScriptName(objectName), instance, type, !! transferOwnership);

	LEAVE_BINDING_API()
}

void G_API_CC ImplScriptObject::bindMethod(IScriptName * name, void * instance, IMetaMethod * method)
{
	ENTER_BINDING_API()

	this->scriptObject->bindMethod(unwrapScriptName(name), instance, method);

	LEAVE_BINDING_API()
}

void G_API_CC ImplScriptObject::bindMethodList(IScriptName * name, IMetaList * methodList)
{
	ENTER_BINDING_API()

	this->scriptObject->bindMethodList(unwrapScriptName(name), methodList);

	LEAVE_BINDING_API()
}

IMetaClass * G_API_CC ImplScriptObject::getClass(IScriptName * className)
{
	ENTER_BINDING_API()

	return this->scriptObject->getClass(unwrapScriptName(className));

	LEAVE_BINDING_API(return NULL)
}

IMetaEnum * G_API_CC ImplScriptObject::getEnum(IScriptName * enumName)
{
	ENTER_BINDING_API()

	return this->scriptObject->getEnum(unwrapScriptName(enumName));

	LEAVE_BINDING_API(return NULL)
}

void G_API_CC ImplScriptObject::getFundamental(GVariantData * outResult, IScriptName * name)
{
	ENTER_BINDING_API()

	*outResult = this->scriptObject->getFundamental(unwrapScriptName(name)).takeData();

	LEAVE_BINDING_API()
}

char * G_API_CC ImplScriptObject::getString(IScriptName * stringName, IMemoryAllocator * allocator)
{
	ENTER_BINDING_API()

	std::string s = this->scriptObject->getString(unwrapScriptName(stringName));

	void * cs = allocator->allocate(static_cast<uint32_t>(s.length() + 1));
	memmove(cs, s.c_str(), s.length() + 1);
	
	return static_cast<char *>(cs);

	LEAVE_BINDING_API(return NULL)
}

void * G_API_CC ImplScriptObject::getObject(IScriptName * objectName)
{
	ENTER_BINDING_API()

	return this->scriptObject->getObject(unwrapScriptName(objectName));

	LEAVE_BINDING_API(return NULL)
}

IMetaMethod * G_API_CC ImplScriptObject::getMethod(IScriptName * methodName)
{
	ENTER_BINDING_API()

	return this->scriptObject->getMethod(unwrapScriptName(methodName));

	LEAVE_BINDING_API(return NULL)
}

IMetaList * G_API_CC ImplScriptObject::getMethodList(IScriptName * methodName)
{
	ENTER_BINDING_API()

	return this->scriptObject->getMethodList(unwrapScriptName(methodName));

	LEAVE_BINDING_API(return NULL)
}

IScriptObject * G_API_CC ImplScriptObject::createScriptObject(IScriptName * name)
{
	ENTER_BINDING_API()

	GScriptObject * obj = this->scriptObject->createScriptObject(unwrapScriptName(name));
	if(obj == NULL) {
		return NULL;
	}
	else {
		return new ImplScriptObject(obj);
	}

	LEAVE_BINDING_API(return NULL)
}

void G_API_CC ImplScriptObject::invoke(GMetaVarData * outResult, IScriptName * name, const GMetaVarData * params, uint32_t paramCount)
{
	ENTER_BINDING_API()

	const GMetaVarData * paramIndirect[REF_MAX_ARITY];

	for(uint32_t i = 0; i < paramCount; ++i) {
		paramIndirect[i] = &params[i];
	}

	this->invokeIndirectly(outResult, name, paramIndirect, paramCount);

	LEAVE_BINDING_API()
}

void G_API_CC ImplScriptObject::invokeIndirectly(GMetaVarData * outResult, IScriptName * name, GMetaVarData const * const * params, uint32_t paramCount)
{
	ENTER_BINDING_API()

	GMetaVariant paramVariants[REF_MAX_ARITY];
	const GMetaVariant * paramIndirect[REF_MAX_ARITY];

	for(uint32_t i = 0; i < paramCount; ++i) {
		paramVariants[i] = GMetaVariant(*params[i]);
		paramIndirect[i] = &paramVariants[i];
	}

	GMetaVariant result = this->scriptObject->invokeIndirectly(unwrapScriptName(name), paramIndirect, paramCount);
	if(outResult) {
		*outResult = result.takeData();
	}

	LEAVE_BINDING_API()
}

void G_API_CC ImplScriptObject::assignValue(IScriptName * fromName, IScriptName * toName)
{
	ENTER_BINDING_API()

	this->scriptObject->assignValue(unwrapScriptName(fromName), unwrapScriptName(toName));

	LEAVE_BINDING_API()
}

gapi_bool G_API_CC ImplScriptObject::valueIsNull(IScriptName * name)
{
	ENTER_BINDING_API()

	return this->scriptObject->valueIsNull(unwrapScriptName(name));

	LEAVE_BINDING_API(return false)
}

void G_API_CC ImplScriptObject::nullifyValue(IScriptName * name)
{
	ENTER_BINDING_API()

	this->scriptObject->nullifyValue(unwrapScriptName(name));

	LEAVE_BINDING_API()
}



} // namespace cpfg

