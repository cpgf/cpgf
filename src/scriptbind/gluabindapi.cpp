#include "cpgf/scriptbind/gscriptbindapi.h"
#include "cpgf/scriptbind/gluabind.h"

#include "../pinclude/gapiimpl.h"


#define ENTER_META_API() \
	this->clearError(); \
	try {

#define LEAVE_META_API(...) \
	} \
	catch(const GVariantException & e) { this->handleError(metaError_VariantCastFail, e.what()); __VA_ARGS__; } \
	catch(const GMetaException & e) { this->handleError(e.getErrorCode(), e.what()); __VA_ARGS__; }


#define IMPL_BASE \
	virtual uint32_t G_API_CC unused_queryInterface(void *, void *) { return 0; } \
	virtual uint32_t G_API_CC addReference() { return this->doAddReference(); } \
	virtual uint32_t G_API_CC releaseReference() { return this->doReleaseReference(); }

#define IMPL_OBJECT \
protected: \
	virtual int32_t G_API_CC getErrorCode() { return this->doGetErrorCode(); } \
	virtual const char * G_API_CC getErrorMessage() { return this->doGetErrorMessage(); }


namespace cpgf {



class ImplScriptConfig : public ImplApiObject, public IScriptConfig
{
public:
	ImplScriptConfig();
	explicit ImplScriptConfig(GScriptConfig config);

protected:
	IMPL_BASE
	IMPL_OBJECT

	virtual void G_API_CC setAccessStaticMethodViaInstance(gapi_bool set);
	virtual gapi_bool G_API_CC allowAccessStaticMethodViaInstance();

	virtual void G_API_CC setAccessEnumTypeViaInstance(gapi_bool set);
	virtual gapi_bool G_API_CC allowAccessEnumTypeViaInstance();

	virtual void G_API_CC setAccessEnumValueViaInstance(gapi_bool set);
	virtual gapi_bool G_API_CC allowAccessEnumValueViaInstance();

	virtual void G_API_CC setAccessClassViaInstance(gapi_bool set);
	virtual gapi_bool G_API_CC allowAccessClassViaInstance();
	
private:
	GScriptConfig config;
};


class ImplScriptName : public ImplApiObject, public IScriptName
{
public:
	ImplScriptName(const char * name);
	
protected:
	IMPL_BASE
	IMPL_OBJECT
	
	virtual const char * G_API_CC getName();
	
private:
	GScriptName scriptName;

private:
	friend class ImplScriptObject;
};

class ImplScriptObject : public ImplApiObject, public IScriptObject
{
public:
	ImplScriptObject(GScriptObject * scriptObject);
	
protected:
	IMPL_BASE
	IMPL_OBJECT
	
	virtual IScriptConfig * G_API_CC getConfig();
	virtual IScriptObject * G_API_CC getOwner();
	virtual gapi_bool G_API_CC isGlobal();

	virtual gapi_bool G_API_CC cacheName(IScriptName * name);

	virtual uint32_t G_API_CC getType(IScriptName * name);

	virtual void G_API_CC bindClass(IScriptName * name, IMetaClass * metaClass);
	virtual void G_API_CC bindEnum(IScriptName * name, IMetaEnum * metaEnum);

	virtual IScriptObject * G_API_CC createScriptObject(IScriptName * name);
	
	virtual void G_API_CC callIndirectly(IScriptName * name, GVarData * outResult, GVarData const * const * params, uint32_t paramCount);
	virtual void G_API_CC call(IScriptName * name, GVarData * outResult, const GVarData * params, uint32_t paramCount);

	virtual void G_API_CC setFundamental(IScriptName * name, const GVarData * value);
	virtual void G_API_CC setString(IScriptName * stringName, const char * s);
	virtual void G_API_CC setObject(IScriptName * objectName, void * instance, IMetaClass * type, gapi_bool transferOwnership);
	virtual void G_API_CC setMethod(IScriptName * name, void * instance, IMetaMethod * method);
	
	virtual void G_API_CC getFundamental(IScriptName * name, GVarData * outResult);
	virtual char * G_API_CC getString(IScriptName * stringName);
	
	virtual void * G_API_CC getObject(IScriptName * objectName);
	virtual IMetaClass * G_API_CC getObjectType(IScriptName * objectName);
	virtual IMetaClass * G_API_CC getClass(IScriptName * className);
	
	virtual IMetaMethod * G_API_CC getMethod(IScriptName * methodName);
	virtual IMetaEnum * G_API_CC getEnum(IScriptName * enumName);
	
	virtual void G_API_CC assignValue(IScriptName * fromName, IScriptName * toName);
	virtual gapi_bool G_API_CC valueIsNull(IScriptName * name);
	virtual void G_API_CC nullifyValue(IScriptName * name);

private:
	GScriptName & unwrapScriptName(IScriptName * scriptName) {
		return static_cast<ImplScriptName *>(scriptName)->scriptName;
	}
	
private:
	GScopedPointer<GScriptObject> scriptObject;
};



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



ImplScriptName::ImplScriptName(const char * name) : scriptName(name)
{
}
	
const char * G_API_CC ImplScriptName::getName()
{
	return this->scriptName.getName();
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
	GScriptObject * owner = this->scriptObject->getOwner();
	if(owner == NULL) {
		return NULL;
	}
	else {
		return new ImplScriptObject(owner);
	}
}

gapi_bool G_API_CC ImplScriptObject::isGlobal()
{
	return this->scriptObject->isGlobal();
}

gapi_bool G_API_CC ImplScriptObject::cacheName(IScriptName * name)
{
	return this->scriptObject->cacheName(&this->unwrapScriptName(name));
}

uint32_t G_API_CC ImplScriptObject::getType(IScriptName * name)
{
	return this->scriptObject->getType(this->unwrapScriptName(name));
}

void G_API_CC ImplScriptObject::bindClass(IScriptName * name, IMetaClass * metaClass)
{
	this->scriptObject->bindClass(this->unwrapScriptName(name), metaClass);
}

void G_API_CC ImplScriptObject::bindEnum(IScriptName * name, IMetaEnum * metaEnum)
{
	this->scriptObject->bindEnum(this->unwrapScriptName(name), metaEnum);
}

IScriptObject * G_API_CC ImplScriptObject::createScriptObject(IScriptName * name)
{
	GScriptObject * obj = this->scriptObject->createScriptObject(this->unwrapScriptName(name));
	if(obj == NULL) {
		return NULL;
	}
	else {
		return new ImplScriptObject(obj);
	}
}

void G_API_CC ImplScriptObject::callIndirectly(IScriptName * name, GVarData * outResult, GVarData const * const * params, uint32_t paramCount)
{
	GVariant paramVariants[REF_MAX_ARITY];
	const GVariant * paramIndirect[REF_MAX_ARITY];

	for(uint32_t i = 0; i < paramCount; ++i) {
		paramVariants[i] = GVariant(*params[i]);
		paramIndirect[i] = &paramVariants[i];
	}

	GVariant result = this->scriptObject->callIndirectly(this->unwrapScriptName(name), paramIndirect, paramCount);
	if(outResult) {
		*outResult = result.takeData();
	}
}

void G_API_CC ImplScriptObject::call(IScriptName * name, GVarData * outResult, const GVarData * params, uint32_t paramCount)
{
	const GVarData * paramIndirect[REF_MAX_ARITY];

	for(uint32_t i = 0; i < paramCount; ++i) {
		paramIndirect[i] = &params[i];
	}

	this->callIndirectly(name, outResult, paramIndirect, paramCount);
}

void G_API_CC ImplScriptObject::setFundamental(IScriptName * name, const GVarData * value)
{
	this->scriptObject->setFundamental(this->unwrapScriptName(name), GVariant(*value));
}

void G_API_CC ImplScriptObject::setString(IScriptName * stringName, const char * s)
{
	this->scriptObject->setString(this->unwrapScriptName(stringName), s);
}

void G_API_CC ImplScriptObject::setObject(IScriptName * objectName, void * instance, IMetaClass * type, gapi_bool transferOwnership)
{
	this->scriptObject->setObject(this->unwrapScriptName(objectName), instance, type, !! transferOwnership);
}

void G_API_CC ImplScriptObject::setMethod(IScriptName * name, void * instance, IMetaMethod * method)
{
	this->scriptObject->setMethod(this->unwrapScriptName(name), instance, method);
}

void G_API_CC ImplScriptObject::getFundamental(IScriptName * name, GVarData * outResult)
{
	*outResult = this->scriptObject->getFundamental(this->unwrapScriptName(name)).takeData();
}

char * G_API_CC ImplScriptObject::getString(IScriptName * stringName)
{
	GApiScopedPointer<IMetaService> service(createMetaService());
	
	std::string s = this->scriptObject->getString(this->unwrapScriptName(stringName));
	void * cs = service->allocateMemory(s.length() + 1);
	memmove(cs, s.c_str(), s.length() + 1);
	
	return static_cast<char *>(cs);
}

void * G_API_CC ImplScriptObject::getObject(IScriptName * objectName)
{
	return this->scriptObject->getObject(this->unwrapScriptName(objectName));
}

IMetaClass * G_API_CC ImplScriptObject::getObjectType(IScriptName * objectName)
{
	return this->scriptObject->getObjectType(this->unwrapScriptName(objectName));
}

IMetaClass * G_API_CC ImplScriptObject::getClass(IScriptName * className)
{
	return this->scriptObject->getClass(this->unwrapScriptName(className));
}

IMetaMethod * G_API_CC ImplScriptObject::getMethod(IScriptName * methodName)
{
	return this->scriptObject->getMethod(this->unwrapScriptName(methodName));
}

IMetaEnum * G_API_CC ImplScriptObject::getEnum(IScriptName * enumName)
{
	return this->scriptObject->getEnum(this->unwrapScriptName(enumName));
}

void G_API_CC ImplScriptObject::assignValue(IScriptName * fromName, IScriptName * toName)
{
	this->scriptObject->assignValue(this->unwrapScriptName(fromName), this->unwrapScriptName(toName));
}

gapi_bool G_API_CC ImplScriptObject::valueIsNull(IScriptName * name)
{
	return this->scriptObject->valueIsNull(this->unwrapScriptName(name));
}

void G_API_CC ImplScriptObject::nullifyValue(IScriptName * name)
{
	this->scriptObject->nullifyValue(this->unwrapScriptName(name));
}

	

IScriptObject * createLuaScriptObject(IMetaService * service, lua_State * L, const GScriptConfig & config)
{
	return new ImplScriptObject(new GLuaScriptObject(service, L, config));
}


} // namespace cpfg

