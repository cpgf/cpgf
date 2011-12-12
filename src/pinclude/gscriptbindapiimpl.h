#ifndef __GSCRIPTBINDAPIIMPL_H
#define __GSCRIPTBINDAPIIMPL_H


#include "cpgf/scriptbind/gscriptbindapi.h"
#include "cpgf/gexception.h"

#include "gapiimpl.h"
#include "gbindcommon.h"


#define ENTER_BINDING_API() \
	this->clearError(); \
	try {

#define LEAVE_BINDING_API(...) \
	} \
	catch(const GException & e) { this->handleError(e.getCode(), e.getMessage()); __VA_ARGS__; }


namespace cpgf {


class ImplScriptConfig : public ImplObject, public IScriptConfig
{
public:
	ImplScriptConfig();
	explicit ImplScriptConfig(GScriptConfig config);

protected:
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


class ImplScriptObject : public ImplExtendObject, public IScriptObject
{
public:
	ImplScriptObject(GScriptObject * scriptObject);
	
protected:
	IMPL_OBJECT
	IMPL_EXTENDOBJECT

	virtual IScriptConfig * G_API_CC getConfig();
	virtual IScriptObject * G_API_CC getOwner();
	virtual gapi_bool G_API_CC isGlobal();

	virtual uint32_t G_API_CC getType(const char * name, IMetaTypedItem ** outMetaTypeItem);

	virtual void G_API_CC bindClass(const char * name, IMetaClass * metaClass);
	virtual void G_API_CC bindEnum(const char * name, IMetaEnum * metaEnum);

	virtual void G_API_CC bindFundamental(const char * name, const GVariantData * value);
	virtual void G_API_CC bindAccessible(const char * name, void * instance, IMetaAccessible * accessible);
	virtual void G_API_CC bindString(const char * stringName, const char * s);
	virtual void G_API_CC bindObject(const char * objectName, void * instance, IMetaClass * type, gapi_bool transferOwnership);
	virtual void G_API_CC bindRaw(const char * name, const GVariantData * value);
	virtual void G_API_CC bindMethod(const char * name, void * instance, IMetaMethod * method);
	virtual void G_API_CC bindMethodList(const char * name, IMetaList * methodList);

	virtual IMetaClass * G_API_CC getClass(const char * className);
	virtual IMetaEnum * G_API_CC getEnum(const char * enumName);

	virtual void G_API_CC getFundamental(GVariantData * outResult, const char * name);
	virtual char * G_API_CC getString(const char * stringName, IMemoryAllocator * allocator);
	virtual void * G_API_CC getObject(const char * objectName);
	virtual void G_API_CC getRaw(GVariantData * outResult, const char * name);
	virtual IMetaMethod * G_API_CC getMethod(const char * methodName, void ** outInstance);
	virtual IMetaList * G_API_CC getMethodList(const char * methodName);

	virtual IScriptObject * G_API_CC createScriptObject(const char * name);
	virtual IScriptObject * G_API_CC gainScriptObject(const char * name);

	virtual void G_API_CC invoke(GMetaVarData * outResult, const char * name, const GMetaVarData * params, uint32_t paramCount);
	virtual void G_API_CC invokeIndirectly(GMetaVarData * outResult, const char * name, GMetaVarData const * const * params, uint32_t paramCount);

	virtual void G_API_CC assignValue(const char * fromName, const char * toName);
	virtual gapi_bool G_API_CC valueIsNull(const char * name);
	virtual void G_API_CC nullifyValue(const char * name);

private:
	GScopedPointer<GScriptObject> scriptObject;
};



} // namespace cpgf



#endif

