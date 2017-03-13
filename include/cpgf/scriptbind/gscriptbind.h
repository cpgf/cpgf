#ifndef CPGF_GSCRIPTBIND_H
#define CPGF_GSCRIPTBIND_H


#include "cpgf/scriptbind/gscriptbindapi.h"
#include "cpgf/scriptbind/gscriptvalue.h"
#include "cpgf/scriptbind/gscriptconfig.h"

#include "cpgf/gmetaapiutil.h"
#include "cpgf/gsharedinterface.h"
#include "cpgf/gclassutil.h"
#include "cpgf/gdeprecated.h"

#include <string>
#include <algorithm>
#include <vector>


namespace cpgf {

struct IScriptLibraryLoader;
struct IScriptUserConverter;

struct IScriptContext : public IObject
{
	virtual void G_API_CC addScriptUserConverter(IScriptUserConverter * converter) = 0;
	virtual void G_API_CC removeScriptUserConverter(IScriptUserConverter * converter) = 0;
	virtual uint32_t G_API_CC getScriptUserConverterCount() = 0;
	virtual IScriptUserConverter * G_API_CC getScriptUserConverterAt(uint32_t index) = 0;
	virtual void G_API_CC setAllowGC(const GVariant & instance, bool allowGC) = 0;
	virtual void G_API_CC bindExternalObjectToClass(void * address, IMetaClass * metaClass) = 0;
};

class GScriptObject
{
protected:
	explicit GScriptObject(const GScriptConfig & config);
	GScriptObject(const GScriptObject & other);

public:
	virtual ~GScriptObject();

	const GScriptConfig & getConfig() const;

	GScriptObject * getOwner() const;

	virtual bool isGlobal() const;

public:
	GScriptValue getValue(const char * name);
	void setValue(const char * name, const GScriptValue & value);

	virtual GScriptValue createScriptObject(const char * name);

	virtual GScriptValue getScriptFunction(const char * name) = 0;

	virtual GScriptValue invoke(const char * name, const GVariant * params, size_t paramCount) = 0;
	virtual GScriptValue invokeIndirectly(const char * name, GVariant const * const * params, size_t paramCount) = 0;

	virtual void assignValue(const char * fromName, const char * toName) = 0;

	void bindCoreService(const char * name, IScriptLibraryLoader * libraryLoader);

	virtual IMetaService * getMetaService() = 0;

	virtual void holdObject(IObject * object);

	virtual IMetaClass * cloneMetaClass(IMetaClass * metaClass) = 0;

	virtual bool maybeIsScriptArray(const char * name) = 0;
	virtual GScriptValue getAsScriptArray(const char * name) = 0;
	virtual GScriptValue createScriptArray(const char * name) = 0;

	virtual IScriptContext * getContext() const = 0;


	G_DEPRECATED(
		void bindClass(const char * name, IMetaClass * metaClass),
		"bindClass is deprecated. Use setValue(name, GScriptValue::fromClass(metaClass)) instead."
	);
	G_DEPRECATED(
		void bindEnum(const char * name, IMetaEnum * metaEnum),
		"bindEnum is deprecated. Use setValue(name, GScriptValue::fromEnum(metaEnum)) instead."
	);

	G_DEPRECATED(
		void bindFundamental(const char * name, const GVariant & value),
		"bindFundamental is deprecated. Use setValue(name, GScriptValue::fromFundamental(value)) instead."
	);
	G_DEPRECATED(
		void bindAccessible(const char * name, void * instance, IMetaAccessible * accessible),
		"bindAccessible is deprecated. Use setValue(name, GScriptValue::fromAccessible(instance, accessible)) instead."
	);
	G_DEPRECATED(
		void bindString(const char * stringName, const char * s),
		"bindString is deprecated. Use setValue(name, GScriptValue::fromString(s)) instead."
	);
	G_DEPRECATED(
		void bindObject(const char * objectName, void * instance, IMetaClass * type, bool transferOwnership),
		"bindObject is deprecated. Use setValue(name, GScriptValue::fromObject(instance, type, transferOwnership)) instead."
	);
	G_DEPRECATED(
		void bindRaw(const char * name, const GVariant & value),
		"bindRaw is deprecated. Use setValue(name, GScriptValue::fromRaw(value)) instead."
	);
	G_DEPRECATED(
		void bindMethod(const char * name, void * instance, IMetaMethod * method),
		"bindMethod is deprecated. Use setValue(name, GScriptValue::fromMethod(instance, method)) instead."
	);
	G_DEPRECATED(
		void bindMethodList(const char * name, IMetaList * methodList),
		"bindMethodList is deprecated. Use setValue(name, GScriptValue::fromOverloadedMethods(methodList)) instead."
	);

	G_DEPRECATED(
		IMetaClass * getClass(const char * className),
		"getClass is deprecated. Use getValue().toClass() instead."
	);
	G_DEPRECATED(
		IMetaEnum * getEnum(const char * enumName),
		"getEnum is deprecated. Use getValue().toEnum() instead."
	);

	G_DEPRECATED(
		GVariant getFundamental(const char * name),
		"getFundamental is deprecated. Use getValue().toFundamental() instead."
	);
	G_DEPRECATED(
		std::string getString(const char * stringName),
		"getString is deprecated. Use getValue().toString() instead."
	);
	G_DEPRECATED(
		void * getObject(const char * objectName),
		"getObject is deprecated. Use getValue().toObject() instead."
	);
	G_DEPRECATED(
		GVariant getRaw(const char * name),
		"getRaw is deprecated. Use getValue().toRaw() instead."
	);
	G_DEPRECATED(
		IMetaMethod * getMethod(const char * methodName, void ** outInstance),
		"getMethod is deprecated. Use getValue().toMethod() instead."
	);
	G_DEPRECATED(
		IMetaList * getMethodList(const char * methodName),
		"getMethodList is deprecated. Use getValue().toOverloadedMethods() instead."
	);

	G_DEPRECATED(
		GScriptValue::Type getType(const char * name, IMetaTypedItem ** outMetaTypeItem),
		"getType is deprecated. Use getValue().getType() instead."
	);

	G_DEPRECATED(
		bool valueIsNull(const char * name),
		"valueIsNull is deprecated. Use getValue().isNull() instead."
	);
	G_DEPRECATED(
		void nullifyValue(const char * name),
		"nullifyValue is deprecated. Use setValue(name, GScriptValue::fromNull()) instead."
	);

protected:
	virtual GScriptValue doGetValue(const char * name) = 0;
	virtual void doSetValue(const char * name, const GScriptValue & value) = 0;

	virtual void doBindCoreService(const char * name, IScriptLibraryLoader * libraryLoader) = 0;

	virtual GScriptObject * doCreateScriptObject(const char * name) = 0;

protected:
	const char * getName() const;
	void setName(const std::string & newName);
	void setOwner(GScriptObject * newOwner);

private:
	GScriptObject & operator = (const GScriptObject &);

private:
	GScriptConfig config;
	GScriptObject * owner;
	std::string name;
	std::vector<GSharedInterface<IObject> > objectHolder;
};

GMetaType getVariantRealMetaType(const GVariant & value);


} // namespace cpgf



#endif
