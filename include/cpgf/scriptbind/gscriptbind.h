#ifndef CPGF_GSCRIPTBIND_H
#define CPGF_GSCRIPTBIND_H


#include "cpgf/scriptbind/gscriptbindapi.h"
#include "cpgf/scriptbind/gscriptvalue.h"
#include "cpgf/scriptbind/gscriptuserconverter.h"

#include "cpgf/gmetaapiutil.h"
#include "cpgf/gsharedinterface.h"
#include "cpgf/gclassutil.h"

#include <string>
#include <algorithm>
#include <vector>


namespace cpgf {

struct IScriptLibraryLoader;
struct IScriptUserConverter;

enum class GScriptInstanceCv {
	sicvNone,
	sicvConst,
	sicvVolatile,
	sicvConstVolatile,

	sicvCount
};

struct IScriptContext : public IObject
{
	virtual void G_API_CC addScriptUserConverter(IScriptUserConverter * converter) = 0;
	virtual void G_API_CC removeScriptUserConverter(IScriptUserConverter * converter) = 0;
	virtual uint32_t G_API_CC getScriptUserConverterCount() = 0;
	virtual IScriptUserConverter * G_API_CC getScriptUserConverterAt(uint32_t index) = 0;
	virtual void G_API_CC setAllowGC(const GVariantData * instance, gapi_bool allowGC) = 0;
	virtual void G_API_CC bindExternalObjectToClass(void * address, IMetaClass * metaClass) = 0;
};

class GScriptObject
{
protected:
	GScriptObject();
	GScriptObject(const GScriptObject & other);

public:
	virtual ~GScriptObject();

	GScriptObject * getOwner() const;

	virtual bool isGlobal() const;

public:
	GScriptValue getValue(const char * name);
	void setValue(const char * name, const GScriptValue & value);

	virtual GScriptValue createScriptObject(const char * name);

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
	GScriptObject * owner;
	std::string name;
	std::vector<GSharedInterface<IObject> > objectHolder;
};

GMetaType getVariantRealMetaType(const GVariant & value);


} // namespace cpgf



#endif
