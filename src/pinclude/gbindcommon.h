#ifndef __GBINDCOMMON_H
#define __GBINDCOMMON_H

#include "cpgf/scriptbind/gscriptbind.h"
#include "cpgf/scriptbind/gscriptwrapper.h"
#include "cpgf/gmetaclasstraveller.h"
#include "cpgf/gmetaclass.h"
#include "cpgf/gglobal.h"
#include "cpgf/gmetaoperatorop.h"
#include "cpgf/gsharedptr.h"

#include <map>
#include <set>


namespace cpgf {

class GMetaClassTraveller;
class GScriptCoreService;

namespace bind_internal {

class GBindingContext;
class GScriptDataHolder;
class GClassPool;

typedef GSharedPointer<GBindingContext> GContextPointer;
typedef GWeakPointer<GBindingContext> GWeakContextPointer;

class GClassGlueData;


enum ObjectPointerCV {
	opcvNone,
	opcvConst,
	opcvVolatile,
	opcvConstVolatile
};

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

enum GMetaMapItemType {
	mmitMethod = 0,
	mmitMethodList = 1,
	mmitProperty = 2,
	mmitField = 3,
	mmitEnum = 4,
	mmitEnumValue = 5,
	mmitClass = 6,
	mmitNone = 100,
};

enum GGlueDataMethodType {
	gdmtMethod,
	gdmtMethodList,
	gdmtInternal
};

class GUserData
{
public:
	GUserData() {}
	virtual ~GUserData() {}
};

class GMetaMapItem
{
public:
	GMetaMapItem();
	GMetaMapItem(IMetaItem * item, GMetaMapItemType type);
	GMetaMapItem(size_t enumIndex, IMetaEnum * item);
	explicit GMetaMapItem(IMetaList * metaList);
	GMetaMapItem(const GMetaMapItem & other);
	~GMetaMapItem();

	GMetaMapItem & operator = (GMetaMapItem other);

	void swap(GMetaMapItem & other);

	GMetaMapItemType getType() const;
	IObject * getItem() const;

	size_t getEnumIndex() const {
		return this->enumIndex;
	}

	void setUserData(GUserData * userData) const {
		this->userData.reset(userData);
	}

	GUserData * getUserData() const {
		return this->userData.get();
	}

private:
	GSharedInterface<IObject> item;
	GMetaMapItemType type;
	size_t enumIndex;
	mutable GScopedPointer<GUserData> userData;
};

inline void swap(GMetaMapItem & a, GMetaMapItem & b)
{
	a.swap(b);
}

class GMetaMapClass : public GNoncopyable
{
public:
	typedef std::map<const char *, GMetaMapItem, meta_internal::CStringCompare> MapType;

public:
	GMetaMapClass(IMetaClass * metaClass);

	GMetaMapItem * findItem(const char * name);

	const MapType * getMap() const {
		return &this->itemMap;
	}

	void setUserData(GUserData * data) {
		this->userData.reset(data);
	}

	GUserData * getUserData() const {
		return this->userData.get();
	}

private:
	void buildMap(IMetaClass * metaClass);

private:
	MapType itemMap;
	GScopedPointer<GUserData> userData;
};

class GMetaMap
{
private:
	typedef std::map<const char *, GMetaMapClass *, meta_internal::CStringCompare> MapType;

public:
	GMetaMap();
	~GMetaMap();

	GMetaMapClass * getClassMap(IMetaClass * metaClass);

private:
	MapType classMap;
};


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

	GContextPointer getContext() const {
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

private:
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

class GObjectGlueData : public GGlueData, public GShareFromThis<GObjectGlueData>
{
private:
	typedef GGlueData super;

private:
	GObjectGlueData(const GContextPointer & context, const GClassGlueDataPointer & classGlueData, const GVariant & instance,
		bool allowGC, ObjectPointerCV cv);

public:		
	~GObjectGlueData();

	GClassGlueDataPointer getClassData() const {
		return this->classGlueData;
	}

	const GVariant & getInstance() const {
		return this->instance;
	}

	void * getInstanceAddress() const {
		return objectAddressFromVariant(this->instance);
	}

	bool isAllowGC() const {
		return this->allowGC;
	}

	ObjectPointerCV getCV() const {
		return this->cv;
	}

	GScriptDataHolder * getDataHolder() const;
	GScriptDataHolder * requireDataHolder() const;
	
private:
	void initialize();

private:
	GClassGlueDataPointer classGlueData;
	GVariant instance;
	bool allowGC;
	ObjectPointerCV cv;
	mutable GScopedPointer<GScriptDataHolder> dataHolder;
	GScopedInterface<IScriptDataStorage> dataStorage;

private:
	friend class GBindingContext;
};


class GMethodGlueData : public GGlueData
{
private:
	typedef GGlueData super;

private:
	GMethodGlueData(const GContextPointer & context, const GClassGlueDataPointer & classGlueData, IMetaList * methodList, const char * name, GGlueDataMethodType methodType)
		: super(gdtMethod, context), classGlueData(classGlueData), methodList(methodList), name(name), methodType(methodType) {
	}

public:
	GClassGlueDataPointer getClassData() const {
		return this->classGlueData.get();
	}

	IMetaList * getMethodList() const {
		return this->methodList.get();
	}

	const std::string & getName() const {
		return this->name;
	}

	GGlueDataMethodType getMethodType() const {
		return this->methodType;
	}

private:
	GWeakClassGlueDataPointer classGlueData;
	GSharedInterface<IMetaList> methodList;
	std::string name;
	GGlueDataMethodType methodType;

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

private:
	GVariant data;

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
	GOperatorGlueData(const GContextPointer & context, const GVariant & instance, IMetaClass * metaClass, GMetaOpType op)
		: super(gdtOperator, context), instance(instance), metaClass(metaClass), op(op) {
	}

	const GVariant & getInstance() const {
		return this->instance;
	}

	void * getInstanceAddress() const {
		return objectAddressFromVariant(this->instance);
	}

	IMetaClass * getMetaClass() const {
		return this->metaClass.get();
	}

	GMetaOpType getOp() const {
		return this->op;
	}

private:
	GVariant instance;
	GSharedInterface<IMetaClass> metaClass;
	GMetaOpType op;
};

typedef GSharedPointer<GOperatorGlueData> GOperatorGlueDataPointer;


class GScriptDataStorage : public IScriptDataStorage
{
	G_INTERFACE_IMPL_OBJECT

private:
	explicit GScriptDataStorage(const GObjectGlueDataPointer & object);

protected:
	virtual IScriptFunction * G_API_CC getScriptFunction(const char * name);

private:
	GWeakObjectGlueDataPointer object;

private:
	friend class GBindingContext;
	friend class GObjectGlueData;
};


class GScriptDataHolder
{
private:
	typedef std::map<std::string, GVariant> MapType;
	typedef std::pair<std::string, GVariant> MapValueType;

public:
	void setScriptValue(const char * name, const GVariant & value);
	IScriptFunction * getScriptFunction(const char * name);

private:
	void requireDataMap();

private:
	GScopedPointer<MapType> dataMap;
};


class GGlueDataWrapper
{
public:
	virtual ~GGlueDataWrapper() {
	}

	virtual GGlueDataPointer getData() = 0;

	template <typename T>
	GSharedPointer<T> getAs() {
		return sharedStaticCast<T>(this->getData());
	}
};

template <typename T>
class GGlueDataWrapperImplement : public GGlueDataWrapper
{
public:
	explicit GGlueDataWrapperImplement(const T & p) : dataPointer(p) {
		GASSERT((bool)p);

	}

	virtual ~GGlueDataWrapperImplement() {
	}

	virtual GGlueDataPointer getData() {
		return sharedStaticCast<GGlueData>(this->dataPointer);
	}

private:
	T dataPointer;
};


class GGlueDataWrapperPool
{
private:
	typedef std::set<GGlueDataWrapper *> SetType;

public:
	GGlueDataWrapperPool();
	~GGlueDataWrapperPool();
	
	void dataWrapperCreated(GGlueDataWrapper * dataWrapper);
	void dataWrapperDestroyed(GGlueDataWrapper * dataWrapper);

private:
	bool active;
	SetType wrapperSet;
};


class GBindingContext : public GShareFromThis<GBindingContext>
{
public:
	GBindingContext(IMetaService * service, const GScriptConfig & config);
	virtual ~GBindingContext();

	IMetaService * getService() const {
		return this->service.get();
	}

	const GScriptConfig & getConfig() const {
		return this->config;
	}

	void bindScriptCoreService(GScriptObject * scriptObject, const char * bindName);

public:
	GClassGlueDataPointer getOrNewClassData(void * instance, IMetaClass * metaClass);
	GClassGlueDataPointer getClassData(IMetaClass * metaClass);
	GClassGlueDataPointer newClassData(IMetaClass * metaClass);

	GObjectGlueDataPointer newObjectGlueData(const GClassGlueDataPointer & classData, const GVariant & instance,
		bool allowGC, ObjectPointerCV cv);
	
	GMethodGlueDataPointer newMethodGlueData(const GClassGlueDataPointer & classData,
		IMetaList * methodList, const char * name, GGlueDataMethodType methodType);
	
	GEnumGlueDataPointer newEnumGlueData(IMetaEnum * metaEnum);

	GAccessibleGlueDataPointer newAccessibleGlueData(void * instance, IMetaAccessible * accessible);

	GRawGlueDataPointer newRawGlueData(const GVariant & data);
	
	GObjectAndMethodGlueDataPointer newObjectAndMethodGlueData(const GObjectGlueDataPointer & objectData, const GMethodGlueDataPointer & methodData);

	GOperatorGlueDataPointer newOperatorGlueData(void * instance, IMetaClass * metaClass, GMetaOpType op);

private:
	GClassPool * getClassPool();
	GClassGlueDataPointer createClassGlueData(IMetaClass * metaClass);

private:
	GSharedInterface<IMetaService> service;
	GScriptConfig config;
	GScopedPointer<GClassPool> classPool;
	
	GScopedPointer<GScriptCoreService> scriptCoreService;
	GScopedPointer<GMetaClass> scriptCoreServiceMetaClass;

private:
	template <typename T>
	friend class GGlueDataWrapperImplement;

	friend class GClassGlueData;
	friend class GObjectGlueData;
	friend class GClassPool;
};


class InvokeParamRank
{
public:
	explicit InvokeParamRank(size_t paramCount);
	InvokeParamRank(const InvokeParamRank & other);
	InvokeParamRank & operator = (const InvokeParamRank & other);

public:
	size_t paramCount;
	mutable int ranks[REF_MAX_ARITY];
};

class CallableParamDataType
{
public:
	GScriptDataType dataType;
	GScopedInterface<IMetaTypedItem> typeItem;
};

class InvokeCallableParam
{
public:
	explicit InvokeCallableParam(size_t paramCount);
	~InvokeCallableParam();

public:
	GVariant paramsData[REF_MAX_ARITY];
	CallableParamDataType paramsType[REF_MAX_ARITY];
	size_t paramCount;
	InvokeParamRank paramsRank;
};

class InvokeCallableResult
{
public:
	int resultCount;
	GVariant resultData;
	GSharedInterface<IMetaCallable> callable;
};

template <typename T>
GGlueDataWrapper * newGlueDataWrapper(const T & p)
{
	return new GGlueDataWrapperImplement<T>(p);
}

template <typename T>
GGlueDataWrapper * newGlueDataWrapper(const T & p, GGlueDataWrapperPool * pool)
{
	GGlueDataWrapper * wrapper = newGlueDataWrapper(p);
	if(pool != NULL) {
		pool->dataWrapperCreated(wrapper);
	}
	return wrapper;
}

template <typename T>
GGlueDataWrapper * newGlueDataWrapper(void * address, const T & p)
{
	return new (address) GGlueDataWrapperImplement<T>(p);
}

template <typename T>
size_t getGlueDataWrapperSize()
{
	return sizeof(GGlueDataWrapperImplement<GSharedPointer<T> >);
}

inline void destroyGlueDataWrapper(GGlueDataWrapper * p)
{
	p->~GGlueDataWrapper();
}

inline void freeGlueDataWrapper(GGlueDataWrapper * p)
{
	delete p;
}

inline void freeGlueDataWrapper(GGlueDataWrapper * p, GGlueDataWrapperPool * pool)
{
	if(pool != NULL) {
		pool->dataWrapperDestroyed(p);
	}
	freeGlueDataWrapper(p);
}

class GScriptObjectBase : public GScriptObject
{
private:
	typedef GScriptObject super;

public:
	GScriptObjectBase(const GContextPointer & context, const GScriptConfig & config)
		: super(config), context(context)
	{
	}

	GScriptObjectBase(const GScriptObjectBase & other)
		: super(other), context(other.context)
	{
	}

	IMetaClass * cloneMetaClass(IMetaClass * metaClass);

protected:
	const GContextPointer & getContext() const {
		return this->context;
	}

private:
	GContextPointer context;
};


class GScriptFunctionBase : public GScriptFunction
{
private:
	typedef GScriptFunction super;

public:
	explicit GScriptFunctionBase(const GContextPointer & context)
		: context(context)
	{
	}

protected:
	GContextPointer getContext() {
		return this->context.get();
	}

private:
	GWeakContextPointer context;
};


ObjectPointerCV metaTypeToCV(const GMetaType & type);

int rankCallable(IMetaService * service, IMetaCallable * callable, const InvokeCallableParam * callbackParam, const InvokeParamRank * paramsRank);

bool allowAccessData(const GScriptConfig & config, bool isInstance, IMetaAccessible * accessible);

void * doInvokeConstructor(IMetaService * service, IMetaClass * metaClass, InvokeCallableParam * callableParam);
InvokeCallableResult doInvokeMethodList(const GContextPointer & context,
										const GObjectGlueDataPointer & objectData,
										const GMethodGlueDataPointer & methodData, InvokeCallableParam * callableParam);

bool shouldRemoveReference(const GMetaType & type);

wchar_t * stringToWideString(const char * s);
char * wideStringToString(const wchar_t * ws);

GVariant glueDataToVariant(const GGlueDataPointer & glueData);

GVariant getAccessibleValueAndType(void * instance, IMetaAccessible * accessible, GMetaType * outType, bool instanceIsConst);

void loadMethodList(const GContextPointer & context, IMetaList * methodList, const GClassGlueDataPointer & classData,
			const GObjectGlueDataPointer & objectData, const char * methodName);

IMetaClass * selectBoundClass(IMetaClass * currentClass, IMetaClass * derived);

bool doSetFieldValue(const GGlueDataPointer & glueData, const char * name, const GVariant & value);

ObjectPointerCV getGlueDataCV(const GGlueDataPointer & glueData);
void * getGlueDataInstance(const GGlueDataPointer & glueData);
IMetaClass * getGlueDataMetaClass(const GGlueDataPointer & glueData);

GScriptDataType methodTypeToGlueDataType(GGlueDataMethodType methodType);
InvokeCallableResult doInvokeOperator(const GContextPointer & context, void * instance, IMetaClass * metaClass, GMetaOpType op, InvokeCallableParam * callableParam);

template <typename Getter, typename Predict>
int findAppropriateCallable(IMetaService * service,
	const Getter & getter, size_t callableCount,
	InvokeCallableParam * callableParam, Predict predict)
{
	int maxRank = -1;
	int maxRankIndex = -1;

	InvokeParamRank paramsRank(callableParam->paramCount);

	for(size_t i = 0; i < callableCount; ++i) {
		GScopedInterface<IMetaCallable> meta(gdynamic_cast<IMetaCallable *>(getter(static_cast<uint32_t>(i))));
		if(predict(meta.get())) {
			int rank = rankCallable(service, meta.get(), callableParam, &paramsRank);
			if(rank > maxRank) {
				maxRank = rank;
				maxRankIndex = static_cast<int>(i);
				callableParam->paramsRank = paramsRank;
			}
		}
	}

	return maxRankIndex;
}


template <typename Methods>
typename Methods::ResultType variantToScript(const GContextPointer & context, const GVariant & value, const GMetaType & type, bool allowGC, bool allowRaw)
{
	GVariantType vt = static_cast<GVariantType>(value.getType() & ~byReference);

	if(! type.isEmpty() && type.getPointerDimension() <= 1) {
		GScopedInterface<IMetaTypedItem> typedItem(context->getService()->findTypedItemByName(type.getBaseName()));
		if(typedItem) {
			bool isReference = type.isReference();

			if(type.getPointerDimension() == 0 && !isReference) {
				GASSERT_MSG(!! metaIsClass(typedItem->getCategory()), "Unknown type");
				GASSERT_MSG(type.baseIsClass(), "Unknown type");

//				IMetaClass * metaClass = gdynamic_cast<IMetaClass *>(typedItem.get());
//				void * instance = metaClass->cloneInstance(objectAddressFromVariant(value));
				return Methods::doObjectToScript(context, context->getOrNewClassData(objectAddressFromVariant(value), gdynamic_cast<IMetaClass *>(typedItem.get())),
					value, false, metaTypeToCV(type));
			}

			if(type.getPointerDimension() == 1 || isReference) {
				GASSERT_MSG(!! metaIsClass(typedItem->getCategory()), "Unknown type");

				GScopedInterface<IObject> releaseIt;
				if(vtIsInterface(vt)) {
					releaseIt.reset(value.refData().valueInterface); // auto release the reference
				}
				return Methods::doObjectToScript(context, context->getOrNewClassData(objectAddressFromVariant(value), gdynamic_cast<IMetaClass *>(typedItem.get())),
					value, allowGC, metaTypeToCV(type));
			}
		}
		else {
			if(vtIsInterface(vt)) {
				IObject * obj = fromVariant<IObject *>(value);
				if(dynamic_cast<IMetaClass *>(obj)) { // !!! GUID
					IMetaClass * metaClass = dynamic_cast<IMetaClass *>(obj);
					return Methods::doClassToScript(context, metaClass);
				}
			}
		}

		if(shouldRemoveReference(type)) {
			GMetaType newType(type);
			newType.removeReference();

			return Methods::doVariantToScript(context, createTypedVariant(value, newType), allowGC, allowRaw);
		}
	}

	if(allowRaw) {
		return Methods::doRawToScript(context, value);
	}

	return typename Methods::ResultType();
}

template <typename Methods>
typename Methods::ResultType methodResultToScript(const GContextPointer & context, IMetaCallable * callable, InvokeCallableResult * resultValue)
{
	if(resultValue->resultCount > 0) {
		GMetaTypeData typeData;
	
		callable->getResultType(&typeData);
		metaCheckError(callable);

		typename Methods::ResultType result;

		GVariant value = resultValue->resultData;
		result = Methods::doVariantToScript(context, createTypedVariant(value, GMetaType(typeData)), !! callable->isResultTransferOwnership(), false);
		if(! Methods::isSuccessResult(result)) {
			GScopedInterface<IMetaConverter> converter(metaGetResultExtendType(callable, GExtendTypeCreateFlag_Converter).getConverter());
			result = Methods::doConverterToScript(context, value, converter.get());
		}
		if(! Methods::isSuccessResult(result)) {
			result = Methods::doRawToScript(context, value);
		}
		if(! Methods::isSuccessResult(result)) {
			raiseCoreException(Error_ScriptBinding_FailVariantToScript);
		}
		
		return result;
	}
	
	return typename Methods::ResultType();
}


template <typename Methods>
typename Methods::ResultType accessibleToScript(const GContextPointer & context, IMetaAccessible * accessible, void * instance, bool instanceIsConst)
{
	GMetaType type;
	GVariant value = getAccessibleValueAndType(instance, accessible, &type, instanceIsConst);

	typename Methods::ResultType result = Methods::doVariantToScript(context, createTypedVariant(value, type), false, false);
	if(! Methods::isSuccessResult(result)) {
		GScopedInterface<IMetaConverter> converter(metaGetItemExtendType(accessible, GExtendTypeCreateFlag_Converter).getConverter());
		result = Methods::doConverterToScript(context, value, converter.get());
	}
	if(! Methods::isSuccessResult(result)) {
		result = Methods::doRawToScript(context, value);
	}
	if(! Methods::isSuccessResult(result)) {
		raiseCoreException(Error_ScriptBinding_FailVariantToScript);
	}
		
	return result;
}


template <typename Methods>
typename Methods::ResultType converterToScript(const GContextPointer & context, const GVariant & value, IMetaConverter * converter)
{
	if(converter == NULL) {
		return typename Methods::ResultType();
	}

	if(isMetaConverterCanRead(converter->capabilityForCString())) {
		gapi_bool needFree;
		
		GScopedInterface<IMemoryAllocator> allocator(context->getService()->getAllocator());
		const char * s = converter->readCString(objectAddressFromVariant(value), &needFree, allocator.get());

		if(s != NULL) {
			typename Methods::ResultType result = Methods::doStringToScript(context, s);

			if(needFree) {
				allocator->free((void *)s);
			}

			return result;
		}
	}

	if(isMetaConverterCanRead(converter->capabilityForCWideString())) {
		gapi_bool needFree;
		
		GScopedInterface<IMemoryAllocator> allocator(context->getService()->getAllocator());
		const wchar_t * ws = converter->readCWideString(objectAddressFromVariant(value), &needFree, allocator.get());

		if(ws != NULL) {
			typename Methods::ResultType result = Methods::doWideStringToScript(context, ws);

			if(needFree) {
				allocator->free((void *)ws);
			}

			return result;
		}
	}

	return typename Methods::ResultType();
}

template <typename Methods>
typename Methods::ResultType namedMemberToScript(const GGlueDataPointer & glueData, const char * name)
{
	bool isInstance = (glueData->getType() == gdtObject);
	GClassGlueDataPointer classData;
	GObjectGlueDataPointer objectData;
	if(glueData->getType() == gdtObject) {
		objectData = sharedStaticCast<GObjectGlueData>(glueData);
		classData = objectData->getClassData();
	}
	else {
		GASSERT(glueData->getType() == gdtClass);
		classData = sharedStaticCast<GClassGlueData>(glueData);
	}

	const GScriptConfig & config = classData->getContext()->getConfig();
	GContextPointer context = classData->getContext();

	GMetaClassTraveller traveller(classData->getMetaClass(), getGlueDataInstance(glueData));

	void * instance = NULL;
	IMetaClass * outDerived;

	for(;;) {
		GScopedInterface<IMetaClass> metaClass(traveller.next(&instance, &outDerived));
		GScopedInterface<IMetaClass> derived(outDerived);

		if(!metaClass) {
			break;
		}

		GMetaMapClass * mapClass = context->getClassData(metaClass.get())->getClassMap();
		if(! mapClass) {
			continue;
		}

		GMetaMapItem * mapItem = mapClass->findItem(name);
		if(mapItem == NULL) {
			continue;
		}

		switch(mapItem->getType()) {
			case mmitField:
			case mmitProperty: {
				GScopedInterface<IMetaAccessible> data(gdynamic_cast<IMetaAccessible *>(mapItem->getItem()));
				if(allowAccessData(config, isInstance, data.get())) {
					return accessibleToScript<Methods>(context, data.get(), instance, getGlueDataCV(glueData) == opcvConst);
				}
			}
			   break;

			case mmitMethod:
			case mmitMethodList: {
				return Methods::doMethodsToScript(classData, mapItem, name, &traveller, metaClass.get(), derived.get(), objectData);
			}

			case mmitEnum:
				if(! isInstance || config.allowAccessEnumTypeViaInstance()) {
					return Methods::doEnumToScript(classData, mapItem, name);
				}
				break;

			case mmitEnumValue:
				if(! isInstance || config.allowAccessEnumValueViaInstance()) {
					GScopedInterface<IMetaEnum> metaEnum(gdynamic_cast<IMetaEnum *>(mapItem->getItem()));
					return Methods::doVariantToScript(context, metaGetEnumValue(metaEnum, static_cast<uint32_t>(mapItem->getEnumIndex())), false, true);
				}
				break;

			case mmitClass:
				if(! isInstance || config.allowAccessClassViaInstance()) {
					GScopedInterface<IMetaClass> innerMetaClass(gdynamic_cast<IMetaClass *>(mapItem->getItem()));
					return Methods::doClassToScript(context, innerMetaClass.get());
				}
				break;

			default:
				break;
		}
	}

	return typename Methods::ResultType();
}


class GMapItemMethodData : public GUserData
{
public:
	explicit GMapItemMethodData(const GMethodGlueDataPointer & methodData)
		: methodData(methodData) {
	}

	const GMethodGlueDataPointer & getMethodData() const {
		return this->methodData;
	}

private:
	GMethodGlueDataPointer methodData;
};




} // namespace bind_internal


} // namespace cpgf



#endif
