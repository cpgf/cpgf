#include "cpgf/scriptbind/gscriptbindapi.h"
#include "cpgf/scriptbind/gluabind.h"

#include "../pinclude/gapiimpl.h"



#define ENTER_BINDING_API() \
	this->clearError(); \
	try {

#define LEAVE_BINDING_API(...) \
	} \
	catch(const GVariantException & e) { this->handleError(metaError_VariantCastFail, e.what()); __VA_ARGS__; } \
	catch(const GMetaException & e) { this->handleError(e.getErrorCode(), e.what()); __VA_ARGS__; } \
	catch(const GScriptException & e) { this->handleError(e.getErrorCode(), e.what()); __VA_ARGS__; }

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
	ImplScriptName(GScriptName * scriptName);
	
protected:
	IMPL_BASE
	IMPL_OBJECT
	
	virtual const char * G_API_CC getName();
	
private:
	GScopedPointer<GScriptName> scriptName;

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

	virtual IScriptName * G_API_CC createName(const char * name);
	virtual gapi_bool G_API_CC cacheName(IScriptName * name);

	virtual uint32_t G_API_CC getType(IScriptName * name);

	virtual void G_API_CC bindClass(IScriptName * name, IMetaClass * metaClass);
	virtual void G_API_CC bindEnum(IScriptName * name, IMetaEnum * metaEnum);

	virtual IScriptObject * G_API_CC createScriptObject(IScriptName * name);
	
	virtual void G_API_CC invokeIndirectly(IScriptName * name, GVarData * outResult, GVarData const * const * params, uint32_t paramCount);
	virtual void G_API_CC invoke(IScriptName * name, GVarData * outResult, const GVarData * params, uint32_t paramCount);

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
		return *static_cast<ImplScriptName *>(scriptName)->scriptName.get();
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

	return this->scriptObject->cacheName(&this->unwrapScriptName(name));

	LEAVE_BINDING_API(return false)
}

uint32_t G_API_CC ImplScriptObject::getType(IScriptName * name)
{
	ENTER_BINDING_API()

	return this->scriptObject->getType(this->unwrapScriptName(name));

	LEAVE_BINDING_API(return sdtUnknown)
}

void G_API_CC ImplScriptObject::bindClass(IScriptName * name, IMetaClass * metaClass)
{
	ENTER_BINDING_API()

	this->scriptObject->bindClass(this->unwrapScriptName(name), metaClass);

	LEAVE_BINDING_API()
}

void G_API_CC ImplScriptObject::bindEnum(IScriptName * name, IMetaEnum * metaEnum)
{
	ENTER_BINDING_API()

	this->scriptObject->bindEnum(this->unwrapScriptName(name), metaEnum);

	LEAVE_BINDING_API()
}

IScriptObject * G_API_CC ImplScriptObject::createScriptObject(IScriptName * name)
{
	ENTER_BINDING_API()

	GScriptObject * obj = this->scriptObject->createScriptObject(this->unwrapScriptName(name));
	if(obj == NULL) {
		return NULL;
	}
	else {
		return new ImplScriptObject(obj);
	}

	LEAVE_BINDING_API(return NULL)
}

void G_API_CC ImplScriptObject::invokeIndirectly(IScriptName * name, GVarData * outResult, GVarData const * const * params, uint32_t paramCount)
{
	ENTER_BINDING_API()

	GVariant paramVariants[REF_MAX_ARITY];
	const GVariant * paramIndirect[REF_MAX_ARITY];

	for(uint32_t i = 0; i < paramCount; ++i) {
		paramVariants[i] = GVariant(*params[i]);
		paramIndirect[i] = &paramVariants[i];
	}

	GVariant result = this->scriptObject->invokeIndirectly(this->unwrapScriptName(name), paramIndirect, paramCount);
	if(outResult) {
		*outResult = result.takeData();
	}

	LEAVE_BINDING_API()
}

void G_API_CC ImplScriptObject::invoke(IScriptName * name, GVarData * outResult, const GVarData * params, uint32_t paramCount)
{
	ENTER_BINDING_API()

	const GVarData * paramIndirect[REF_MAX_ARITY];

	for(uint32_t i = 0; i < paramCount; ++i) {
		paramIndirect[i] = &params[i];
	}

	this->invokeIndirectly(name, outResult, paramIndirect, paramCount);

	LEAVE_BINDING_API()
}

void G_API_CC ImplScriptObject::setFundamental(IScriptName * name, const GVarData * value)
{
	ENTER_BINDING_API()

	this->scriptObject->setFundamental(this->unwrapScriptName(name), GVariant(*value));

	LEAVE_BINDING_API()
}

void G_API_CC ImplScriptObject::setString(IScriptName * stringName, const char * s)
{
	ENTER_BINDING_API()

	this->scriptObject->setString(this->unwrapScriptName(stringName), s);

	LEAVE_BINDING_API()
}

void G_API_CC ImplScriptObject::setObject(IScriptName * objectName, void * instance, IMetaClass * type, gapi_bool transferOwnership)
{
	ENTER_BINDING_API()

	this->scriptObject->setObject(this->unwrapScriptName(objectName), instance, type, !! transferOwnership);

	LEAVE_BINDING_API()
}

void G_API_CC ImplScriptObject::setMethod(IScriptName * name, void * instance, IMetaMethod * method)
{
	ENTER_BINDING_API()

	this->scriptObject->setMethod(this->unwrapScriptName(name), instance, method);

	LEAVE_BINDING_API()
}

void G_API_CC ImplScriptObject::getFundamental(IScriptName * name, GVarData * outResult)
{
	ENTER_BINDING_API()

	*outResult = this->scriptObject->getFundamental(this->unwrapScriptName(name)).takeData();

	LEAVE_BINDING_API()
}

char * G_API_CC ImplScriptObject::getString(IScriptName * stringName)
{
	ENTER_BINDING_API()

	GApiScopedPointer<IMetaService> service(createMetaService());
	
	std::string s = this->scriptObject->getString(this->unwrapScriptName(stringName));
	void * cs = service->allocateMemory(s.length() + 1);
	memmove(cs, s.c_str(), s.length() + 1);
	
	return static_cast<char *>(cs);

	LEAVE_BINDING_API(return NULL)
}

void * G_API_CC ImplScriptObject::getObject(IScriptName * objectName)
{
	ENTER_BINDING_API()

	return this->scriptObject->getObject(this->unwrapScriptName(objectName));

	LEAVE_BINDING_API(return NULL)
}

IMetaClass * G_API_CC ImplScriptObject::getObjectType(IScriptName * objectName)
{
	ENTER_BINDING_API()

	return this->scriptObject->getObjectType(this->unwrapScriptName(objectName));

	LEAVE_BINDING_API(return NULL)
}

IMetaClass * G_API_CC ImplScriptObject::getClass(IScriptName * className)
{
	ENTER_BINDING_API()

	return this->scriptObject->getClass(this->unwrapScriptName(className));

	LEAVE_BINDING_API(return NULL)
}

IMetaMethod * G_API_CC ImplScriptObject::getMethod(IScriptName * methodName)
{
	ENTER_BINDING_API()

	return this->scriptObject->getMethod(this->unwrapScriptName(methodName));

	LEAVE_BINDING_API(return NULL)
}

IMetaEnum * G_API_CC ImplScriptObject::getEnum(IScriptName * enumName)
{
	ENTER_BINDING_API()

	return this->scriptObject->getEnum(this->unwrapScriptName(enumName));

	LEAVE_BINDING_API(return NULL)
}

void G_API_CC ImplScriptObject::assignValue(IScriptName * fromName, IScriptName * toName)
{
	ENTER_BINDING_API()

	this->scriptObject->assignValue(this->unwrapScriptName(fromName), this->unwrapScriptName(toName));

	LEAVE_BINDING_API()
}

gapi_bool G_API_CC ImplScriptObject::valueIsNull(IScriptName * name)
{
	ENTER_BINDING_API()

	return this->scriptObject->valueIsNull(this->unwrapScriptName(name));

	LEAVE_BINDING_API(return false)
}

void G_API_CC ImplScriptObject::nullifyValue(IScriptName * name)
{
	ENTER_BINDING_API()

	this->scriptObject->nullifyValue(this->unwrapScriptName(name));

	LEAVE_BINDING_API()
}

	

IScriptObject * createLuaScriptObject(IMetaService * service, lua_State * L, const GScriptConfig & config)
{
	return new ImplScriptObject(new GLuaScriptObject(service, L, config));
}


} // namespace cpfg

