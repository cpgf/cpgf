#ifndef __GSCRIPTBIND_H
#define __GSCRIPTBIND_H


#include "cpgf/scriptbind/gscriptbindapi.h"
#include "cpgf/scriptbind/gscriptvalue.h"
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
	sdtMethod = 6, sdtMethodList = 7,
	sdtEnum = 8,
	sdtRaw = 9,

	sdtScriptObject = 10, sdtScriptMethod = 11,
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
	GScriptValue getValue(const char * name);

	void bindClass(const char * name, IMetaClass * metaClass);
	void bindEnum(const char * name, IMetaEnum * metaEnum);

	void bindFundamental(const char * name, const GVariant & value);
	void bindAccessible(const char * name, void * instance, IMetaAccessible * accessible);
	void bindString(const char * stringName, const char * s);
	void bindObject(const char * objectName, void * instance, IMetaClass * type, bool transferOwnership);
	void bindRaw(const char * name, const GVariant & value);
	void bindMethod(const char * name, void * instance, IMetaMethod * method);
	void bindMethodList(const char * name, IMetaList * methodList);

	IMetaClass * getClass(const char * className);
	IMetaEnum * getEnum(const char * enumName);
	
	GVariant getFundamental(const char * name);
	std::string getString(const char * stringName);
	void * getObject(const char * objectName);
	GVariant getRaw(const char * name);
	IMetaMethod * getMethod(const char * methodName, void ** outInstance);
	IMetaList * getMethodList(const char * methodName);

	virtual GScriptDataType getType(const char * name, IMetaTypedItem ** outMetaTypeItem) = 0;

	virtual GScriptObject * createScriptObject(const char * name);

	virtual GScriptFunction * gainScriptFunction(const char * name) = 0;
	
	virtual GVariant invoke(const char * name, const GVariant * params, size_t paramCount) = 0;
	virtual GVariant invokeIndirectly(const char * name, GVariant const * const * params, size_t paramCount) = 0;

	virtual void assignValue(const char * fromName, const char * toName) = 0;
	virtual bool valueIsNull(const char * name) = 0;
	virtual void nullifyValue(const char * name) = 0;

	void bindCoreService(const char * name, IScriptLibraryLoader * libraryLoader);

	virtual IMetaService * getMetaService() = 0;
	
	virtual void holdObject(IObject * object);

	virtual IMetaClass * cloneMetaClass(IMetaClass * metaClass) = 0;

protected:
	virtual GScriptValue doGetValue(const char * name) = 0;

	virtual void doBindClass(const char * name, IMetaClass * metaClass) = 0;
	virtual void doBindEnum(const char * name, IMetaEnum * metaEnum) = 0;

	virtual void doBindFundamental(const char * name, const GVariant & value) = 0;
	virtual void doBindAccessible(const char * name, void * instance, IMetaAccessible * accessible) = 0;
	virtual void doBindString(const char * stringName, const char * s) = 0;
	virtual void doBindObject(const char * objectName, void * instance, IMetaClass * type, bool transferOwnership) = 0;
	virtual void doBindRaw(const char * name, const GVariant & value) = 0;
	virtual void doBindMethod(const char * name, void * instance, IMetaMethod * method) = 0;
	virtual void doBindMethodList(const char * name, IMetaList * methodList) = 0;
	
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



} // namespace cpgf





#endif
