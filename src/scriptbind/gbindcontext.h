#ifndef GBINDCONTEXT_H
#define GBINDCONTEXT_H

#include "cpgf/scriptbind/gscriptbind.h"
#include "cpgf/scriptbind/gscriptservice.h"
#include "cpgf/gsharedinterface.h"
#include "cpgf/gscopedptr.h"
#include "cpgf/gstringmap.h"
#include "cpgf/glifecycle.h"

#include "gbindgluedata.h"

#include <vector>

namespace cpgf {

namespace bind_internal {

class GBindingContext;
class GClassPool;
class GScriptObjectCache;

class GScriptContext : public IScriptContext
{
public:
	explicit GScriptContext(GBindingContext * bindingContext) : bindingContext(bindingContext) {}
	virtual ~GScriptContext() {}

private:
	typedef GSharedInterface<IScriptUserConverter> ScriptUserConverterType;
	typedef std::vector<ScriptUserConverterType> ScriptUserConverterListType;

	G_INTERFACE_IMPL_OBJECT

protected:
	virtual void G_API_CC addScriptUserConverter(IScriptUserConverter * converter) override;
	virtual void G_API_CC removeScriptUserConverter(IScriptUserConverter * converter) override;
	virtual uint32_t G_API_CC getScriptUserConverterCount() override;
	virtual IScriptUserConverter * G_API_CC getScriptUserConverterAt(uint32_t index) override;
	virtual void G_API_CC setAllowGC(const GVariantData * instance, bool allowGC) override;
	virtual void G_API_CC bindExternalObjectToClass(void * address, IMetaClass * metaClass) override;

private:
	ScriptUserConverterListType::iterator findConverter(IScriptUserConverter * converter);

private:
	GScopedPointer<ScriptUserConverterListType> scriptUserConverterList;
	GBindingContext * bindingContext;
	std::vector<GObjectGlueDataPointer> externalObjects;
};


class GBindingContext : public GShareFromThis<GBindingContext>
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
	GClassGlueDataPointer getOrNewClassData(const GVariant & instance, IMetaClass * metaClass);
	GClassGlueDataPointer getClassData(IMetaClass * metaClass);
	GClassGlueDataPointer newClassData(IMetaClass * metaClass);

	GObjectInstancePointer findObjectInstance(const GVariant & instance);

	GObjectGlueDataPointer newObjectGlueData(
		const GClassGlueDataPointer & classData,
		const GVariant & instance,
		const GBindValueFlags & flags,
		const GScriptInstanceCv cv
	);
	GObjectGlueDataPointer newOrReuseObjectGlueData(
		const GClassGlueDataPointer & classData,
		const GVariant & instance,
		const GBindValueFlags & flags,
		const GScriptInstanceCv cv
	);

	GMethodGlueDataPointer newMethodGlueData(
		const GClassGlueDataPointer & classData,
		IMetaList * methodList
	);

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

	GScriptObjectCache * getScriptObjectCache();

private:
	GClassPool * getClassPool();
	GClassGlueDataPointer createClassGlueData(IMetaClass * metaClass);

private:
	GSharedInterface<IMetaService> service;
	GScopedPointer<GClassPool> classPool;

	GScopedPointer<GScriptCoreService> scriptCoreService;
	GScopedInterface<IScriptContext> scriptContext;

	GScopedPointer<GScriptObjectCache> scriptObjectCache;
private:
	template <typename T>
	friend class GGlueDataWrapperImplement;

	friend class GClassGlueData;
	friend class GObjectInstance;
	friend class GObjectGlueData;
	friend class GClassPool;
};

class GClassPool
{
private:
	typedef std::map<void *, GWeakObjectInstancePointer> InstanceMapType;

	typedef std::vector<GClassGlueDataPointer> ClassMapListType;
	typedef GStringMap<ClassMapListType, GStringMapReuseKey> ClassMapType;

public:
	explicit GClassPool(GBindingContext * context);

	void objectCreated(const GObjectInstancePointer & objectData);
	void objectDestroyed(const GObjectInstance * objectData);
	void classDestroyed(IMetaClass * metaClass);

	GObjectInstancePointer findObjectData(const GVariant & instancecv);

	GClassGlueDataPointer getOrNewClassData(const GVariant & instance, IMetaClass * metaClass);
	GClassGlueDataPointer getClassData(IMetaClass * metaClass);
	GClassGlueDataPointer newClassData(IMetaClass * metaClass);

private:
	ClassMapListType * getList(IMetaClass * metaClass);
	GClassGlueDataPointer * findClassDataByPointer(ClassMapListType * classDataList, IMetaClass * metaClass);
	GClassGlueDataPointer createClassDataAtSlot(ClassMapListType * classDataList, IMetaClass * metaClass, size_t slot);
	size_t getFreeSlot(ClassMapListType * classDataList, size_t startSlot);

private:
	InstanceMapType instanceMap;
	ClassMapType classMap;
	GBindingContext * context;
};



} //namespace bind_internal

} //namespace cpgf


#endif
