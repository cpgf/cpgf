#ifndef GBINDCONTEXT_H
#define GBINDCONTEXT_H

#include "cpgf/scriptbind/gscriptbind.h"
#include "cpgf/scriptbind/gscriptservice.h"
#include "cpgf/gsharedinterface.h"
#include "cpgf/gstringmap.h"
#include "cpgf/glifecycle.h"

#include "gbindgluedata.h"

#include <vector>
#include <tuple>
#include <map>
#include <memory>
#include <string>

namespace cpgf {

namespace bind_internal {

class GBindingContext;

class GScriptContext : public IScriptContext
{
public:
	explicit GScriptContext(GBindingContext * bindingContext) : bindingContext(bindingContext) {}
	virtual ~GScriptContext() {}

private:
	typedef GSharedInterface<IScriptUserConverter> ScriptUserConverterType;
	typedef std::vector<ScriptUserConverterType> ScriptUserConverterListType;

	G_INTERFACE_IMPL_OBJECT_OVERRIDE

protected:
	virtual void G_API_CC addScriptUserConverter(IScriptUserConverter * converter) override;
	virtual void G_API_CC removeScriptUserConverter(IScriptUserConverter * converter) override;
	virtual uint32_t G_API_CC getScriptUserConverterCount() override;
	virtual IScriptUserConverter * G_API_CC getScriptUserConverterAt(uint32_t index) override;
	virtual void G_API_CC setAllowGC(const GVariantData * instance, gapi_bool allowGC) override;
	virtual void G_API_CC bindExternalObjectToClass(void * address, IMetaClass * metaClass) override;

private:
	ScriptUserConverterListType::iterator findConverter(IScriptUserConverter * converter);

private:
	std::unique_ptr<ScriptUserConverterListType> scriptUserConverterList;
	GBindingContext * bindingContext;
	std::vector<GObjectGlueDataPointer> externalObjects;
};

class GBindingPool
{
private:
	typedef std::tuple<void *, void *> MethodKey; // <method, instance>
	typedef std::tuple<void *, void *, GScriptInstanceCv> ObjectKey; // <class address, object address, constness>
	typedef std::tuple<GObjectGlueData *, GMethodGlueData *> ObjectAndMethodKey;
	typedef std::tuple<void *, IMetaAccessible *> AccessibleKey;
	typedef std::tuple<GObjectGlueData *, IMetaClass *, GMetaOpType> OperatorKey;

public:
	explicit GBindingPool(const std::shared_ptr<GBindingContext> & context);
	~GBindingPool();

	template <typename T>
	void glueDataAdded(const T & /*glueData*/) {}
	template <typename T>
	void glueDataRemoved(const T & /*glueData*/) {}

	void glueDataAdded(const GMethodGlueDataPointer & glueData);
	void glueDataRemoved(const GMethodGlueDataPointer & glueData);
	GMethodGlueDataPointer newMethodGlueData(const GScriptValue & scriptValue);

	void glueDataAdded(const GObjectGlueDataPointer & glueData);
	void glueDataRemoved(const GObjectGlueDataPointer & glueData);
	GObjectGlueDataPointer newObjectGlueData(
		const GClassGlueDataPointer & classData,
		const GObjectInstancePointer & objectInstance,
		const GScriptInstanceCv cv
	);

	void glueDataAdded(const GObjectAndMethodGlueDataPointer & glueData);
	void glueDataRemoved(const GObjectAndMethodGlueDataPointer & glueData);
	GObjectAndMethodGlueDataPointer newObjectAndMethodGlueData(
		const GObjectGlueDataPointer & objectData,
		const GMethodGlueDataPointer & methodData
	);

	void glueDataAdded(const GEnumGlueDataPointer & glueData);
	void glueDataRemoved(const GEnumGlueDataPointer & glueData);
	GEnumGlueDataPointer newEnumGlueData(IMetaEnum * metaEnum);

	void glueDataAdded(const GAccessibleGlueDataPointer & glueData);
	void glueDataRemoved(const GAccessibleGlueDataPointer & glueData);
	GAccessibleGlueDataPointer newAccessibleGlueData(void * instance, IMetaAccessible * accessible);

	void glueDataAdded(const GRawGlueDataPointer & glueData);
	void glueDataRemoved(const GRawGlueDataPointer & glueData);
	GRawGlueDataPointer newRawGlueData(const GVariant & data);

	void glueDataAdded(const GOperatorGlueDataPointer & glueData);
	void glueDataRemoved(const GOperatorGlueDataPointer & glueData);
	GOperatorGlueDataPointer newOperatorGlueData(
		const GObjectGlueDataPointer & objectData,
		IMetaClass * metaClass,
		const GMetaOpType op
	);

	void objectInstanceAdded(const GObjectInstancePointer & objectData);
	void objectInstanceDestroyed(const GObjectInstance * objectData);
	GObjectInstancePointer findObjectInstance(const GVariant & instance);

	GClassGlueDataPointer getClassData(IMetaClass * metaClass);
	void classDestroyed(IMetaClass * metaClass);

private:
	MethodKey doMakeMethodKey(const GScriptValue & scriptValue);
	ObjectKey doMakeObjectKey(const GObjectGlueDataPointer & glueData);

private:
	std::weak_ptr<GBindingContext> context;
	GMetaMap metaMap;

	std::map<MethodKey, GWeakMethodGlueDataPointer> methodMap;
	std::map<ObjectKey, GWeakObjectGlueDataPointer> objectMap;
	std::map<void *, GWeakObjectInstancePointer> instanceMap;
	std::map<IMetaClass *, GClassGlueDataPointer> classMap;
	std::map<ObjectAndMethodKey, GWeakObjectAndMethodGlueDataPointer> objectAndMethodMap;
	std::map<IMetaEnum *, GWeakEnumGlueDataPointer> enumMap;
	std::map<AccessibleKey, GWeakAccessibleGlueDataPointer> accessibleMap;
	std::map<OperatorKey, GWeakOperatorGlueDataPointer> operatorMap;
};

class GBindingContext : public std::enable_shared_from_this<GBindingContext>
{
public:
	explicit GBindingContext(IMetaService * service);
	virtual ~GBindingContext();

	IMetaService * getService() const {
		return this->service.get();
	}

	void bindScriptCoreService(GScriptObject * scriptObject, const char * bindName, IScriptLibraryLoader * libraryLoader);

	IScriptContext * borrowScriptContext() const;

public:
	GClassGlueDataPointer getClassData(IMetaClass * metaClass);
	void classDestroyed(IMetaClass * metaClass);

	GObjectInstancePointer findObjectInstance(const GVariant & instance);

	GObjectGlueDataPointer newObjectGlueData(
		const GClassGlueDataPointer & classData,
		const GVariant & instance,
		const bool allowGC,
		const GScriptInstanceCv cv
	);

	GMethodGlueDataPointer newMethodGlueData(const GScriptValue & scriptValue);

	GEnumGlueDataPointer newEnumGlueData(IMetaEnum * metaEnum);

	GAccessibleGlueDataPointer newAccessibleGlueData(void * instance, IMetaAccessible * accessible);

	GRawGlueDataPointer newRawGlueData(const GVariant & data);

	GObjectAndMethodGlueDataPointer newObjectAndMethodGlueData(
		const GObjectGlueDataPointer & objectData,
		const GMethodGlueDataPointer & methodData
	);

	GOperatorGlueDataPointer newOperatorGlueData(
		const GObjectGlueDataPointer & objectData,
		IMetaClass * metaClass,
		GMetaOpType op
	);

	GBindingPool * getBindingPool();

private:
	GSharedInterface<IMetaService> service;
	std::shared_ptr<GBindingPool> bindingPool;

	std::unique_ptr<GScriptCoreService> scriptCoreService;
	GScopedInterface<IScriptContext> scriptContext;

private:
	template <typename T>
	friend class GGlueDataWrapperImplement;

	friend class GClassGlueData;
	friend class GObjectInstance;
	friend class GObjectGlueData;
};


} //namespace bind_internal

} //namespace cpgf


#endif
