#ifndef GSCRIPTGLUEDATA_H
#define GSCRIPTGLUEDATA_H

#include "cpgf/gsharedptr.h"
#include "cpgf/gvariant.h"
#include "cpgf/gflags.h"

#include "gscriptcommon.h"

namespace cpgf {

namespace bind_internal {

class GBindingContext;
class GScriptDataHolder;
class GClassPool;

typedef GSharedPointer<GBindingContext> GContextPointer;
typedef GWeakPointer<GBindingContext> GWeakContextPointer;

enum GGlueDataType {
	gdtObject,
	gdtClass,
	gdtEnum,
	gdtAccessible,
	gdtRaw,
	gdtMethod,
	gdtObjectAndMethod, // combination of object and method, only used in Lua binding
	gdtOperator // only used in Lua binding
};

enum GBindValueFlagValues {
	bvfAllowGC = 1 << 0,
	bvfAllowRaw = 1 << 1
};

typedef GFlags<GBindValueFlagValues> GBindValueFlags;

class GGlueData : public GNoncopyable
{
public:
	GGlueData(GGlueDataType type, const GContextPointer & context)
		: type(type), context(GWeakContextPointer(context))
	{
	}

	virtual ~GGlueData() {
	}

	GGlueDataType getType() const {
		return this->type;
	}

	GContextPointer getBindingContext() const {
		return GContextPointer(this->context);
	}

	bool isValid() const {
		return ! this->context.expired();
	}

private:
	GGlueDataType type;
	GWeakContextPointer context;
};

typedef GSharedPointer<GGlueData> GGlueDataPointer;


class GClassGlueData : public GGlueData, public GShareFromThis<GClassGlueData>
{
private:
	typedef GGlueData super;

protected:
	GClassGlueData(const GContextPointer & context, IMetaClass * metaClass);

public:
	~GClassGlueData();

public:
	IMetaClass * getMetaClass() const {
		return this->metaClass.get();
	}

	GMetaMapClass * getClassMap() const {
		return &this->mapClass;
	}

	GScriptDataHolder * getDataHolder() const;
	GScriptDataHolder * requireDataHolder() const;

private:
	GSharedInterface<IMetaClass> metaClass;
	mutable GMetaMapClass mapClass;
	mutable GScopedPointer<GScriptDataHolder> dataHolder;

private:
	friend class GBindingContext;
	friend class GClassPool;
};

typedef GSharedPointer<GClassGlueData> GClassGlueDataPointer;
typedef GWeakPointer<GClassGlueData> GWeakClassGlueDataPointer;


class GObjectGlueData;

typedef GSharedPointer<GObjectGlueData> GObjectGlueDataPointer;
typedef GWeakPointer<GObjectGlueData> GWeakObjectGlueDataPointer;


class GObjectInstance
{
public:
	GObjectInstance(
		const GContextPointer & context,
		const GVariant & instance,
		const GClassGlueDataPointer & classData,
		IMetaObjectLifeManager * objectLifeManager,
		bool allowGC
	);
	~GObjectInstance();

	void setDataStorage(IScriptDataStorage * dataStorage);

	void * getInstanceAddress() const;

	bool isAllowGC() const {
		return this->allowGC;
	}

	void setAllowGC(bool allow) {
		this->allowGC = allow;
	}

	const GVariant & getInstance() const {
		return this->instance;
	}

	IMetaClass * getMetaClass() const {
		return this->classData->getMetaClass();
	}

	GClassGlueDataPointer getClassData() const {
		return this->classData;
	}

	GContextPointer getBindingContext() const {
		return GContextPointer(this->context);
	}

	GScriptDataHolder * getDataHolder() const;
	GScriptDataHolder * requireDataHolder() const;

private:
	GWeakContextPointer context;
	GVariant instance;
	GClassGlueDataPointer classData;
	GSharedInterface<IMetaObjectLifeManager> objectLifeManager;
	bool allowGC;
	bool isSharedPointer;
	GScopedInterface<IScriptDataStorage> dataStorage;
	mutable GScopedPointer<GScriptDataHolder> dataHolder;

private:
	friend class GObjectGlueData;
};

typedef GSharedPointer<GObjectInstance> GObjectInstancePointer;
typedef GWeakPointer<GObjectInstance> GWeakObjectInstancePointer;


class GObjectGlueData : public GGlueData, public GShareFromThis<GObjectGlueData>
{
private:
	typedef GGlueData super;

private:
	GObjectGlueData(
		const GContextPointer & context,
		const GClassGlueDataPointer & classGlueData,
		const GVariant & instance,
		const GBindValueFlags & flags,
		ObjectPointerCV cv
	);
	GObjectGlueData(
		const GContextPointer & context,
		const GClassGlueDataPointer & classGlueData,
		const GObjectInstancePointer & objectInstance,
		const GBindValueFlags & flags,
		ObjectPointerCV cv
	);

public:
	~GObjectGlueData();

	GClassGlueDataPointer getClassData() const {
		return this->classGlueData;
	}

	const GVariant & getInstance() const {
		return this->objectInstance->instance;
	}

	void * getInstanceAddress() const {
		if(this->sharedPointerTraits) {
			return this->sharedPointerTraits->getPointer(objectAddressFromVariant(this->getInstance()));
		}
		else {
			return objectAddressFromVariant(this->getInstance());
		}
	}

	bool isAllowGC() const {
		return this->objectInstance->isAllowGC();
	}

	void setAllowGC(bool allow) {
		this->objectInstance->setAllowGC(allow);
	}

	ObjectPointerCV getCV() const {
		return this->cv;
	}

	void setSharedPointerTraits(IMetaSharedPointerTraits * sharedPointerTraits) {
		this->sharedPointerTraits.reset(sharedPointerTraits);
		this->objectInstance->isSharedPointer = true;
	}

	IMetaSharedPointerTraits * getSharedPointerTraits() const {
		return this->sharedPointerTraits.get();
	}

	GScriptDataHolder * getDataHolder() const {
		return this->objectInstance->getDataHolder();
	}

	GScriptDataHolder * requireDataHolder() const {
		return this->objectInstance->requireDataHolder();
	}

	GObjectInstancePointer getObjectInstance() const {
		return this->objectInstance;
	}

private:
	void initialize();

private:
	GClassGlueDataPointer classGlueData;
	GBindValueFlags flags;
	ObjectPointerCV cv;
	GObjectInstancePointer objectInstance;
	GSharedInterface<IMetaSharedPointerTraits> sharedPointerTraits;

private:
	friend class GBindingContext;
};


class GMethodGlueData : public GGlueData
{
private:
	typedef GGlueData super;

private:
	GMethodGlueData(const GContextPointer & context, const GClassGlueDataPointer & classGlueData, IMetaList * methodList)
		: super(gdtMethod, context), classGlueData(classGlueData), methodList(methodList) {
	}

public:
	GClassGlueDataPointer getClassData() const {
		return this->classGlueData.get();
	}

	IMetaList * getMethodList() const {
		return this->methodList.get();
	}

	std::string getName() const;

private:
	GWeakClassGlueDataPointer classGlueData;
	GSharedInterface<IMetaList> methodList;

private:
	friend class GBindingContext;
};

typedef GSharedPointer<GMethodGlueData> GMethodGlueDataPointer;


class GEnumGlueData : public GGlueData
{
private:
	typedef GGlueData super;

private:
	GEnumGlueData(const GContextPointer & context, IMetaEnum * metaEnum)
		: super(gdtEnum, context), metaEnum(metaEnum) {
	}

public:
	IMetaEnum * getMetaEnum() const {
		return this->metaEnum.get();
	}

private:
	GSharedInterface<IMetaEnum> metaEnum;

private:
	friend class GBindingContext;
};

typedef GSharedPointer<GEnumGlueData> GEnumGlueDataPointer;


class GAccessibleGlueData : public GGlueData
{
private:
	typedef GGlueData super;

private:
	GAccessibleGlueData(const GContextPointer & context, const GVariant & instance, IMetaAccessible * accessible)
		: super(gdtAccessible, context), instance(instance), accessible(accessible) {
	}

public:
	const GVariant & getInstance() const {
		return this->instance;
	}

	void * getInstanceAddress() const {
		return objectAddressFromVariant(this->instance);
	}

	IMetaAccessible * getAccessible() const {
		return this->accessible.get();
	}

private:
	GVariant instance;
	GSharedInterface<IMetaAccessible> accessible;

private:
	friend class GBindingContext;
};

typedef GSharedPointer<GAccessibleGlueData> GAccessibleGlueDataPointer;


class GRawGlueData : public GGlueData
{
private:
	typedef GGlueData super;

private:
	GRawGlueData(const GContextPointer & context, const GVariant & data)
		: super(gdtRaw, context), data(data)
	{
	}

public:
	const GVariant & getData() const {
		return this->data;
	}

	void setSharedPointerTraits(IMetaSharedPointerTraits * sharedPointerTraits) {
		this->sharedPointerTraits.reset(sharedPointerTraits);
	}

	IMetaSharedPointerTraits * getSharedPointerTraits() const {
		return this->sharedPointerTraits.get();
	}

private:
	GVariant data;
	GSharedInterface<IMetaSharedPointerTraits> sharedPointerTraits;

private:
	friend class GBindingContext;
};

typedef GSharedPointer<GRawGlueData> GRawGlueDataPointer;


// Only used by Lua binding
class GObjectAndMethodGlueData : public GGlueData
{
private:
	typedef GGlueData super;

private:
	GObjectAndMethodGlueData(const GContextPointer & context, const GObjectGlueDataPointer & objectData, const GMethodGlueDataPointer & methodData)
		: super(gdtObjectAndMethod, context), objectData(objectData), methodData(methodData)
	{
	}

public:
	GObjectGlueDataPointer getObjectData() const {
		return this->objectData;
	}

	GMethodGlueDataPointer getMethodData() const {
		return this->methodData;
	}

private:
	GObjectGlueDataPointer objectData;
	GMethodGlueDataPointer methodData;

private:
	friend class GBindingContext;
};

typedef GSharedPointer<GObjectAndMethodGlueData> GObjectAndMethodGlueDataPointer;


// Only used by Lua binding
class GOperatorGlueData : public GGlueData
{
private:
	typedef GGlueData super;

public:
	GOperatorGlueData(
		const GContextPointer & context,
		const GObjectGlueDataPointer & objectData,
		IMetaClass * metaClass,
		GMetaOpType op
	)
		: super(gdtOperator, context), objectData(objectData), metaClass(metaClass), op(op) {
	}

	const GObjectGlueDataPointer & getObjectData() const {
		return this->objectData;
	}

	void * getInstanceAddress() const {
		return this->objectData->getInstanceAddress();
	}

	IMetaClass * getMetaClass() const {
		return this->metaClass.get();
	}

	GMetaOpType getOp() const {
		return this->op;
	}

private:
	GObjectGlueDataPointer objectData;
	GSharedInterface<IMetaClass> metaClass;
	GMetaOpType op;
};

typedef GSharedPointer<GOperatorGlueData> GOperatorGlueDataPointer;



} //namespace bind_internal


} //namespace cpgf


#endif
