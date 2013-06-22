#ifndef __GSCRIPTBIND_H
#define __GSCRIPTBIND_H


#include "cpgf/scriptbind/gscriptbindapi.h"
#include "cpgf/scriptbind/gscriptconfig.h"

#include "cpgf/gmetaapiutil.h"
#include "cpgf/gsharedinterface.h"
#include "cpgf/gclassutil.h"

#include <string>
#include <algorithm>
#include <vector>


namespace cpgf {

struct IScriptLibraryLoader;

enum GScriptDataType {
	sdtUnknown = 0,
	sdtNull = 1,
	sdtFundamental = 2, sdtString = 3,
	sdtClass = 4, sdtObject = 5,
	sdtMethod = 6,
	sdtEnum = 7,
	sdtRaw = 8,

	sdtScriptObject = 9, sdtScriptMethod = 10,
};


class GScriptFunction : public GNoncopyable
{
public:
	GScriptFunction();
	virtual ~GScriptFunction();
	
	virtual GVariant invoke(const GVariant * params, size_t paramCount) = 0;
	virtual GVariant invokeIndirectly(GVariant const * const * params, size_t paramCount) = 0;
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
	virtual void bindClass(const char * name, IMetaClass * metaClass) = 0;
	virtual void bindEnum(const char * name, IMetaEnum * metaEnum) = 0;

	virtual void bindFundamental(const char * name, const GVariant & value) = 0;
	virtual void bindAccessible(const char * name, void * instance, IMetaAccessible * accessible) = 0;
	virtual void bindString(const char * stringName, const char * s) = 0;
	virtual void bindObject(const char * objectName, void * instance, IMetaClass * type, bool transferOwnership) = 0;
	virtual void bindRaw(const char * name, const GVariant & value) = 0;
	virtual void bindMethod(const char * name, void * instance, IMetaMethod * method) = 0;
	virtual void bindMethodList(const char * name, IMetaList * methodList) = 0;

	virtual IMetaClass * getClass(const char * className) = 0;
	virtual IMetaEnum * getEnum(const char * enumName) = 0;
	
	virtual GVariant getFundamental(const char * name) = 0;
	virtual std::string getString(const char * stringName) = 0;
	virtual void * getObject(const char * objectName) = 0;
	virtual GVariant getRaw(const char * name) = 0;
	virtual IMetaMethod * getMethod(const char * methodName, void ** outInstance) = 0;
	virtual IMetaList * getMethodList(const char * methodName) = 0;

	virtual GScriptDataType getType(const char * name, IMetaTypedItem ** outMetaTypeItem) = 0;

	virtual GScriptObject * createScriptObject(const char * name);

	virtual GScriptFunction * gainScriptFunction(const char * name) = 0;
	
	virtual GVariant invoke(const char * name, const GVariant * params, size_t paramCount) = 0;
	virtual GVariant invokeIndirectly(const char * name, GVariant const * const * params, size_t paramCount) = 0;

	virtual void assignValue(const char * fromName, const char * toName) = 0;
	virtual bool valueIsNull(const char * name) = 0;
	virtual void nullifyValue(const char * name) = 0;

	virtual void bindCoreService(const char * name, IScriptLibraryLoader * libraryLoader) = 0;

	virtual IMetaService * getMetaService() = 0;
	
	virtual void holdObject(IObject * object);

	virtual IMetaClass * cloneMetaClass(IMetaClass * metaClass) = 0;

protected:
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



} // namespace cpgf





#endif
