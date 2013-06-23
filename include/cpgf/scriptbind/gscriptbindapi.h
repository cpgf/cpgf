#ifndef __GSCRIPTBINDAPI_H
#define __GSCRIPTBINDAPI_H

#include "cpgf/gmetaapi.h"
#include "cpgf/gdeprecated.h"


namespace cpgf {

struct IScriptLibraryLoader;
struct GScriptValueData;

struct IScriptConfig : public IObject
{
	virtual void G_API_CC setAccessStaticMethodViaInstance(gapi_bool set) = 0;
	virtual gapi_bool G_API_CC allowAccessStaticMethodViaInstance() = 0;

	virtual void G_API_CC setAccessEnumTypeViaInstance(gapi_bool set) = 0;
	virtual gapi_bool G_API_CC allowAccessEnumTypeViaInstance() = 0;

	virtual void G_API_CC setAccessEnumValueViaInstance(gapi_bool set) = 0;
	virtual gapi_bool G_API_CC allowAccessEnumValueViaInstance() = 0;

	virtual void G_API_CC setAccessClassViaInstance(gapi_bool set) = 0;
	virtual gapi_bool G_API_CC allowAccessClassViaInstance() = 0;
};


struct IScriptFunction : public IExtendObject
{
public:
	virtual void G_API_CC invoke(GVariantData * outResult, const GVariantData * params, uint32_t paramCount) = 0;
	virtual void G_API_CC invokeIndirectly(GVariantData * outResult, GVariantData const * const * params, uint32_t paramCount) = 0;
};


struct IScriptObject : public IExtendObject
{
	virtual IScriptConfig * G_API_CC getConfig() = 0;
	virtual IScriptObject * G_API_CC getOwner() = 0;
	virtual gapi_bool G_API_CC isGlobal() = 0;

	virtual void G_API_CC getValue(GScriptValueData * outResult, const char * name) = 0;
	virtual void G_API_CC setValue(const char * name, const GScriptValueData * value) = 0;

	virtual void G_API_CC bindClass(const char * name, IMetaClass * metaClass) = 0;
	virtual void G_API_CC bindEnum(const char * name, IMetaEnum * metaEnum) = 0;

	virtual void G_API_CC bindFundamental(const char * name, const GVariantData * value) = 0;
	virtual void G_API_CC bindAccessible(const char * name, void * instance, IMetaAccessible * accessible) = 0;
	virtual void G_API_CC bindString(const char * stringName, const char * s) = 0;
	virtual void G_API_CC bindObject(const char * objectName, void * instance, IMetaClass * type, gapi_bool transferOwnership) = 0;
	virtual void G_API_CC bindRaw(const char * name, const GVariantData * value) = 0;
	virtual void G_API_CC bindMethod(const char * name, void * instance, IMetaMethod * method) = 0;
	virtual void G_API_CC bindMethodList(const char * name, IMetaList * methodList) = 0;
	
	virtual IScriptObject * G_API_CC createScriptObject(const char * name) = 0;
	
	virtual IScriptFunction * G_API_CC gainScriptFunction(const char * name) = 0;
	
	virtual void G_API_CC invoke(GVariantData * outResult, const char * name, const GVariantData * params, uint32_t paramCount) = 0;
	virtual void G_API_CC invokeIndirectly(GVariantData * outResult, const char * name, GVariantData const * const * params, uint32_t paramCount) = 0;

	virtual void G_API_CC assignValue(const char * fromName, const char * toName) = 0;

	virtual void G_API_CC bindCoreService(const char * name, IScriptLibraryLoader * libraryLoader) = 0;
	virtual void G_API_CC holdObject(IObject * object) = 0;
	
	G_DEPRECATED(
		virtual IMetaClass * G_API_CC getClass(const char * className),
		"getClass is deprecated. Use scriptGetValue().toClass() instead."
	) = 0;
	G_DEPRECATED(
		virtual IMetaEnum * G_API_CC getEnum(const char * enumName),
		"getEnum is deprecated. Use scriptGetValue().toEnum() instead."
	) = 0;
	
	G_DEPRECATED(
		virtual void G_API_CC getFundamental(GVariantData * outResult, const char * name),
		"getFundamental is deprecated. Use scriptGetValue().toFundamental() instead."
	) = 0;
	G_DEPRECATED(
		virtual char * G_API_CC getString(const char * stringName, IMemoryAllocator * allocator),
		"getString is deprecated. Use scriptGetValue().toString() instead."
	) = 0;
	G_DEPRECATED(
		virtual void * G_API_CC getObject(const char * objectName),
		"getObject is deprecated. Use scriptGetValue().toObject() instead."
	) = 0;
	G_DEPRECATED(
		virtual void G_API_CC getRaw(GVariantData * outResult, const char * name),
		"getRaw is deprecated. Use scriptGetValue().toRaw() instead."
	) = 0;
	G_DEPRECATED(
		virtual IMetaMethod * G_API_CC getMethod(const char * methodName, void ** outInstance),
		"getMethod is deprecated. Use scriptGetValue().toMethod() instead."
	) = 0;
	G_DEPRECATED(
		virtual IMetaList * G_API_CC getMethodList(const char * methodName),
		"getMethodList is deprecated. Use scriptGetValue().toOverridedMethods() instead."
	) = 0;

	G_DEPRECATED(
		virtual uint32_t G_API_CC getType(const char * name, IMetaTypedItem ** outMetaTypeItem),
		"getType is deprecated. Use scriptGetValue().getType() instead."
	) = 0;

	G_DEPRECATED(
		virtual gapi_bool G_API_CC valueIsNull(const char * name),
		"valueIsNull is deprecated. Use scriptGetValue().isNull() instead."
	) = 0;
	G_DEPRECATED(
		virtual void G_API_CC nullifyValue(const char * name),
		"nullifyValue is deprecated. Use scriptSetValue(name, GScriptValue::fromNull()) instead."
	) = 0;
	
};



} // namespace cpgf




#endif
