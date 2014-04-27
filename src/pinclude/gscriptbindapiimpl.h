#ifndef CPGF_GSCRIPTBINDAPIIMPL_H
#define CPGF_GSCRIPTBINDAPIIMPL_H


#include "cpgf/gmetaapi.h"
#include "cpgf/scriptbind/gscriptbindapi.h"
#include "cpgf/gexception.h"

#include "gbindcommon.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4996)
#endif


#define ENTER_BINDING_API() \
	this->ginterface_implExtendObject.clearError(); \
	try {

#define LEAVE_BINDING_API(...) \
	} \
	catch(const GException & e) { this->ginterface_implExtendObject.handleError(e.getCode(), e.getMessage()); __VA_ARGS__; }


namespace cpgf {


class ImplScriptConfig : public IScriptConfig
{
public:
	ImplScriptConfig();
	explicit ImplScriptConfig(GScriptConfig config);
	virtual ~ImplScriptConfig();

protected:
	G_INTERFACE_IMPL_OBJECT

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


class ImplScriptFunction : public IScriptFunction
{
public:
	ImplScriptFunction(GScriptFunction * scriptFunction, bool freeFunction);
	virtual ~ImplScriptFunction();

protected:
	G_INTERFACE_IMPL_OBJECT
	G_INTERFACE_IMPL_EXTENDOBJECT

	virtual void G_API_CC invoke(GScriptValueData * outResult, const GVariantData * params, uint32_t paramCount);
	virtual void G_API_CC invokeIndirectly(GScriptValueData * outResult, GVariantData const * const * params, uint32_t paramCount);
	virtual void G_API_CC invokeOnObject(GScriptValueData * outResult, const GVariantData * params, uint32_t paramCount);
	virtual void G_API_CC invokeIndirectlyOnObject(GScriptValueData * outResult, GVariantData const * const * params, uint32_t paramCount);
	// Internal use only!!!
	virtual void G_API_CC weaken();

private:
	GScriptFunction * scriptFunction;
	bool freeFunction;
};


class ImplScriptArray : public IScriptArray
{
public:
	ImplScriptArray(GScriptArray * scriptArray, bool freeArray);
	virtual ~ImplScriptArray();

protected:
	G_INTERFACE_IMPL_OBJECT
	G_INTERFACE_IMPL_EXTENDOBJECT

	virtual uint32_t G_API_CC getLength();
	virtual void G_API_CC getValue(GScriptValueData * outResult, uint32_t index);
	virtual void G_API_CC setValue(uint32_t index, const GScriptValueData * value);

	virtual gapi_bool G_API_CC maybeIsScriptArray(uint32_t index);
	virtual void G_API_CC getAsScriptArray(GScriptValueData * outResult, uint32_t index);
	virtual void G_API_CC createScriptArray(GScriptValueData * outResult, uint32_t index);
private:
	GScriptArray * scriptArray;
	bool freeArray;
};


class ImplScriptObject : public IScriptObject
{
public:
	ImplScriptObject(GScriptObject * scriptObject, bool freeObject);
	virtual ~ImplScriptObject();

protected:
	G_INTERFACE_IMPL_OBJECT
	G_INTERFACE_IMPL_EXTENDOBJECT

	virtual IScriptContext * G_API_CC getContext();

	virtual IScriptConfig * G_API_CC getConfig();
	virtual IScriptObject * G_API_CC getOwner();
	virtual gapi_bool G_API_CC isGlobal();

	virtual void G_API_CC getValue(GScriptValueData * outResult, const char * name);
	virtual void G_API_CC setValue(const char * name, const GScriptValueData * value);

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

	virtual void G_API_CC createScriptObject(GScriptValueData * outResult, const char * name);

	virtual void G_API_CC getScriptFunction(GScriptValueData * outResult, const char * name);

	virtual void G_API_CC invoke(GScriptValueData * outResult, const char * name, const GVariantData * params, uint32_t paramCount);
	virtual void G_API_CC invokeIndirectly(GScriptValueData * outResult, const char * name, GVariantData const * const * params, uint32_t paramCount);

	virtual void G_API_CC assignValue(const char * fromName, const char * toName);
	virtual gapi_bool G_API_CC valueIsNull(const char * name);
	virtual void G_API_CC nullifyValue(const char * name);

	virtual void G_API_CC bindCoreService(const char * name, IScriptLibraryLoader * libraryLoader);
	virtual void G_API_CC holdObject(IObject * object);

	virtual gapi_bool G_API_CC maybeIsScriptArray(const char * name);
	virtual void G_API_CC getAsScriptArray(GScriptValueData * outResult, const char * name);
	virtual void G_API_CC createScriptArray(GScriptValueData * outResult, const char * name);

private:
	GScriptObject * scriptObject;
	bool freeObject;
};



} // namespace cpgf


#if defined(_MSC_VER)
#pragma warning(pop)
#endif


#endif

