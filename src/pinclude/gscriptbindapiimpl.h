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


class ImplScriptName : public ImplObject, public IScriptName
{
public:
	ImplScriptName(GScriptName * scriptName);
	
	GScriptName & getScriptName() {
		return *this->scriptName.get();
	}

protected:
	IMPL_OBJECT

	virtual const char * G_API_CC getName();

private:
	GScopedPointer<GScriptName> scriptName;

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

	virtual IScriptName * G_API_CC createName(const char * name);
	virtual gapi_bool G_API_CC cacheName(IScriptName * name);

	virtual uint32_t G_API_CC getType(IScriptName * name, IMetaTypedItem ** outMetaTypeItem);

	virtual void G_API_CC bindClass(IScriptName * name, IMetaClass * metaClass);
	virtual void G_API_CC bindEnum(IScriptName * name, IMetaEnum * metaEnum);

	virtual void G_API_CC bindFundamental(IScriptName * name, const GVariantData * value);
	virtual void G_API_CC bindString(IScriptName * stringName, const char * s);
	virtual void G_API_CC bindObject(IScriptName * objectName, void * instance, IMetaClass * type, gapi_bool transferOwnership);
	virtual void G_API_CC bindMethod(IScriptName * name, void * instance, IMetaMethod * method);
	virtual void G_API_CC bindMethodList(IScriptName * name, IMetaList * methodList);

	virtual IMetaClass * G_API_CC getClass(IScriptName * className);
	virtual IMetaEnum * G_API_CC getEnum(IScriptName * enumName);

	virtual void G_API_CC getFundamental(GVariantData * outResult, IScriptName * name);
	virtual char * G_API_CC getString(IScriptName * stringName, IMemoryAllocator * allocator);
	virtual void * G_API_CC getObject(IScriptName * objectName);
	virtual IMetaMethod * G_API_CC getMethod(IScriptName * methodName, void ** outInstance);
	virtual IMetaList * G_API_CC getMethodList(IScriptName * methodName);

	virtual IScriptObject * G_API_CC createScriptObject(IScriptName * name);
	virtual IScriptObject * G_API_CC getScriptObject(IScriptName * name);

	virtual void G_API_CC invoke(GMetaVarData * outResult, IScriptName * name, const GMetaVarData * params, uint32_t paramCount);
	virtual void G_API_CC invokeIndirectly(GMetaVarData * outResult, IScriptName * name, GMetaVarData const * const * params, uint32_t paramCount);

	virtual void G_API_CC assignValue(IScriptName * fromName, IScriptName * toName);
	virtual gapi_bool G_API_CC valueIsNull(IScriptName * name);
	virtual void G_API_CC nullifyValue(IScriptName * name);

private:
	GScopedPointer<GScriptObject> scriptObject;
};



} // namespace cpgf



#endif

