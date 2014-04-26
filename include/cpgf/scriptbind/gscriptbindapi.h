#ifndef CPGF_GSCRIPTBINDAPI_H
#define CPGF_GSCRIPTBINDAPI_H

#include "cpgf/gmetaapi.h"
#include "cpgf/gdeprecated.h"


namespace cpgf {

struct IScriptLibraryLoader;
struct GScriptValueData;
struct IScriptContext;

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
	virtual void G_API_CC invoke(GScriptValueData * outResult, const GVariantData * params, uint32_t paramCount) = 0;
	virtual void G_API_CC invokeIndirectly(GScriptValueData * outResult, GVariantData const * const * params, uint32_t paramCount) = 0;
	virtual void G_API_CC invokeOnObject(GScriptValueData * outResult, const GVariantData * params, uint32_t paramCount) = 0;
	virtual void G_API_CC invokeIndirectlyOnObject(GScriptValueData * outResult, GVariantData const * const * params, uint32_t paramCount) = 0;

	// Internal use only!!!
	virtual void G_API_CC weaken() = 0;
};


struct IScriptArray : public IExtendObject
{
public:
	virtual uint32_t G_API_CC getLength() = 0;
	virtual void G_API_CC getValue(GScriptValueData * outResult, uint32_t index) = 0;
	virtual void G_API_CC setValue(uint32_t index, const GScriptValueData * value) = 0;

	virtual gapi_bool G_API_CC maybeIsScriptArray(uint32_t index) = 0;
	virtual void G_API_CC getAsScriptArray(GScriptValueData * outResult, uint32_t index) = 0;
	virtual void G_API_CC createScriptArray(GScriptValueData * outResult, uint32_t index) = 0;
};


struct IScriptObject : public IExtendObject
{
	virtual IScriptContext * G_API_CC getContext() = 0;

	virtual IScriptConfig * G_API_CC getConfig() = 0;
	virtual IScriptObject * G_API_CC getOwner() = 0;
	virtual gapi_bool G_API_CC isGlobal() = 0;

	virtual void G_API_CC getValue(GScriptValueData * outResult, const char * name) = 0;
	virtual void G_API_CC setValue(const char * name, const GScriptValueData * value) = 0;

	virtual void G_API_CC createScriptObject(GScriptValueData * outResult, const char * name) = 0;

	virtual void G_API_CC getScriptFunction(GScriptValueData * outResult, const char * name) = 0;

	virtual void G_API_CC invoke(GScriptValueData * outResult, const char * name, const GVariantData * params, uint32_t paramCount) = 0;
	virtual void G_API_CC invokeIndirectly(GScriptValueData * outResult, const char * name, GVariantData const * const * params, uint32_t paramCount) = 0;

	virtual void G_API_CC assignValue(const char * fromName, const char * toName) = 0;

	virtual void G_API_CC bindCoreService(const char * name, IScriptLibraryLoader * libraryLoader) = 0;
	virtual void G_API_CC holdObject(IObject * object) = 0;

	virtual gapi_bool G_API_CC maybeIsScriptArray(const char * name) = 0;
	virtual void G_API_CC getAsScriptArray(GScriptValueData * outResult, const char * name) = 0;
	virtual void G_API_CC createScriptArray(GScriptValueData * outResult, const char * name) = 0;

	G_DEPRECATED(
		virtual void G_API_CC bindClass(const char * name, IMetaClass * metaClass),
		"bindClass is deprecated. Use scriptSetValue(scriptObject, name, GScriptValue::fromClass(metaClass)) instead."
	) = 0;
	G_DEPRECATED(
		virtual void G_API_CC bindEnum(const char * name, IMetaEnum * metaEnum),
		"bindEnum is deprecated. Use scriptSetValue(scriptObject, name, GScriptValue::fromEnum(metaEnum)) instead."
	) = 0;

	G_DEPRECATED(
		virtual void G_API_CC bindFundamental(const char * name, const GVariantData * value),
		"bindFundamental is deprecated. Use scriptSetValue(scriptObject, name, GScriptValue::fromFundamental(value)) instead."
	) = 0;
	G_DEPRECATED(
		virtual void G_API_CC bindAccessible(const char * name, void * instance, IMetaAccessible * accessible),
		"bindAccessible is deprecated. Use scriptSetValue(scriptObject, name, GScriptValue::fromAccessible(instance, accessible)) instead."
	) = 0;
	G_DEPRECATED(
		virtual void G_API_CC bindString(const char * stringName, const char * s),
		"bindString is deprecated. Use scriptSetValue(scriptObject, name, GScriptValue::fromString(s)) instead."
	) = 0;
	G_DEPRECATED(
		virtual void G_API_CC bindObject(const char * objectName, void * instance, IMetaClass * type, gapi_bool transferOwnership),
		"bindObject is deprecated. Use scriptSetValue(scriptObject, name, GScriptValue::fromObject(instance, type, transferOwnership)) instead."
	) = 0;
	G_DEPRECATED(
		virtual void G_API_CC bindRaw(const char * name, const GVariantData * value),
		"bindRaw is deprecated. Use scriptSetValue(scriptObject, name, GScriptValue::fromRaw(GVariant(*value))) instead."
	) = 0;
	G_DEPRECATED(
		virtual void G_API_CC bindMethod(const char * name, void * instance, IMetaMethod * method),
		"bindMethod is deprecated. Use scriptSetValue(scriptObject, name, GScriptValue::fromMethod(instance, method)) instead."
	) = 0;
	G_DEPRECATED(
		virtual void G_API_CC bindMethodList(const char * name, IMetaList * methodList),
		"bindMethodList is deprecated. Use scriptSetValue(scriptObject, name, GScriptValue::fromOverloadedMethods(methodList)) instead."
	) = 0;

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
		"getMethodList is deprecated. Use scriptGetValue().toOverloadedMethods() instead."
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
