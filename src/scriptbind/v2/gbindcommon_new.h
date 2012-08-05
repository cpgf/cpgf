#ifndef __GBINDCOMMON_NEW_H
#define __GBINDCOMMON_NEW_H

#include "cpgf/scriptbind/gscriptbind.h"
#include "cpgf/scriptbind/gscriptwrapper.h"
#include "cpgf/gglobal.h"
#include "cpgf/gmetaoperatorop.h"
#include "cpgf/gsharedptr.h"

#include <map>


namespace cpgf {

class GMetaClassTraveller;
class GScriptCoreService;

namespace _bind_internal {

class GClassPool;

class GBindingContext;
class GScriptDataHolder;

typedef GSharedPointer<GBindingContext> GContextPointer;
typedef GWeakPointer<GBindingContext> GWeakContextPointer;


enum ObjectPointerCV {
	opcvNone,
	opcvConst,
	opcvVolatile,
	opcvConstVolatile
};

enum ObjectGlueDataType {
	ogdtNormal,
	ogdtInterface
};

enum GGlueDataType {
	gdtObject,
	gdtClass,
	gdtExtendMethod,
	gdtEnum,
	gdtOperator,
	gdtAccessible,
	gdtRaw,
	gdtMethod
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

private:
	void buildMap(IMetaClass * metaClass);

private:
	MapType itemMap;
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

	bool isInvalid() const {
		return this->context.expired();
	}

private:
	GGlueDataType type;
	GWeakContextPointer context;
};

typedef GSharedPointer<GGlueData> GGlueDataPointer;

class GMetaMapClass;

typedef GSharedPointer<GMetaMapClass> GMetaMapClassPointer;

class GClassGlueData : public GGlueData
{
private:
	typedef GGlueData super;

private:
	GClassGlueData(const GContextPointer & context, IMetaClass * metaClass)
		: super(gdtClass, context), metaClass(metaClass)
	{
	}

public:
	IMetaClass * getMetaClass() const {
		return this->metaClass.get();
	}

	void setUserData(GUserData * userData) const {
		this->userData.reset(userData);
	}

	GUserData * getUserData() const {
		return this->userData.get();
	}

	const GMetaMapClassPointer & getClassMap() const;
	
	GScriptDataHolder * getDataHolder() const;
	GScriptDataHolder * requireDataHolder() const;

private:
	GSharedInterface<IMetaClass> metaClass;
	mutable GMetaMapClassPointer classMap;
	mutable GScopedPointer<GUserData> userData;
	mutable GScopedPointer<GScriptDataHolder> dataHolder;

private:
	friend class GBindingContext;
};

typedef GSharedPointer<GClassGlueData> GClassGlueDataPointer;


class GObjectGlueData;

typedef GSharedPointer<GObjectGlueData> GObjectGlueDataPointer;
typedef GWeakPointer<GObjectGlueData> GWeakObjectGlueDataPointer;

class GObjectGlueData : public GGlueData
{
private:
	typedef GGlueData super;

private:
	GObjectGlueData(const GContextPointer & context, const GClassGlueDataPointer & classGlueData, void * instance,
		bool allowGC, ObjectPointerCV cv, ObjectGlueDataType dataType);

public:		
	~GObjectGlueData();

	const GClassGlueDataPointer & getClassData() const {
		return this->classGlueData;
	}

	void * getInstance() const {
		return this->instance;
	}

	IObject * getInterfaceObject() const {
		return this->interfaceObject.get();
	}

	bool isAllowGC() const {
		return this->allowGC;
	}

	ObjectPointerCV getCV() const {
		return this->cv;
	}

	ObjectGlueDataType getDataType() const {
		return this->dataType;
	}

	GScriptDataHolder * getDataHolder() const;
	GScriptDataHolder * requireDataHolder() const;
	
private:
	void setWeakThis(const GWeakObjectGlueDataPointer & weakThis);

private:
	GClassGlueDataPointer classGlueData;
	void * instance;
	GSharedInterface<IObject> interfaceObject;
	bool allowGC;
	ObjectPointerCV cv;
	ObjectGlueDataType dataType;
	mutable GScopedPointer<GScriptDataHolder> dataHolder;
	GWeakObjectGlueDataPointer weakThis;
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
	const GClassGlueDataPointer & getClassData() const {
		return this->classGlueData;
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
	GClassGlueDataPointer classGlueData;
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
	GAccessibleGlueData(const GContextPointer & context, void * instance, IMetaAccessible * accessible)
		: super(gdtAccessible, context), instance(instance), accessible(accessible) {
	}

public:
	void * getInstance() const {
		return this->instance;
	}

	IMetaAccessible * getAccessible() const {
		return this->accessible.get();
	}

private:
	void * instance;
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

public:
	void setScriptValue(const char * name, const GVariant & value);
	IScriptFunction * getScriptFunction(const char * name);

private:
	void requireDataMap();

private:
	GScopedPointer<MapType> dataMap;
};


class GBindingContext
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
	GClassGlueDataPointer newClassGlueData(const GContextPointer & context, IMetaClass * metaClass);
	GClassGlueDataPointer requireClassGlueData(const GContextPointer & context, IMetaClass * metaClass);

	GObjectGlueDataPointer newObjectGlueData(const GContextPointer & context, const GClassGlueDataPointer & classData, void * instance,
		bool allowGC, ObjectPointerCV cv, ObjectGlueDataType dataType);
	
	GMethodGlueDataPointer newMethodGlueData(const GContextPointer & context, const GClassGlueDataPointer & classData,
		IMetaList * methodList, const char * name, GGlueDataMethodType methodType);
	
	GEnumGlueDataPointer newEnumGlueData(const GContextPointer & context, IMetaEnum * metaEnum);

	GAccessibleGlueDataPointer newAccessibleGlueData(const GContextPointer & context, void * instance, IMetaAccessible * accessible);

	GRawGlueDataPointer newRawGlueData(const GContextPointer & context, const GVariant & data);

private:
	GSharedInterface<IMetaService> service;
	GScriptConfig config;
	GScopedPointer<GClassPool> classPool;
	
	GScopedPointer<GScriptCoreService> scriptCoreService;
	GScopedPointer<GMetaClass> scriptCoreServiceMetaClass;
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
	}

	virtual GGlueDataPointer getData() {
		return sharedStaticCast<GGlueData>(this->dataPointer);
	}

private:
	T dataPointer;
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

inline void freeGlueDataPointer(GGlueDataWrapper * p)
{
	delete p;
}

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

GMetaVariant glueDataToVariant(const GGlueDataPointer & glueData);

GVariant getAccessibleValueAndType(void * instance, IMetaAccessible * accessible, GMetaType * outType, bool instanceIsConst);

void loadMethodList(const GContextPointer & context, IMetaList * methodList, const GClassGlueDataPointer & classData,
			const GObjectGlueDataPointer & objectData, const char * methodName);

IMetaClass * selectBoundClass(IMetaClass * currentClass, IMetaClass * derived);

bool doSetFieldValue(const GGlueDataPointer & glueData, const char * name, const GVariant & value);

ObjectPointerCV getGlueDataCV(const GGlueDataPointer & glueData);
void * getGlueDataInstance(const GGlueDataPointer & glueData);
IMetaClass * getGlueDataMetaClass(const GGlueDataPointer & glueData);


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
bool variantToScript(typename Methods::ResultType * result,
	const GContextPointer & context, const GVariant & value, const GMetaType & type, bool allowGC, bool allowRaw)
{
	GVariantType vt = static_cast<GVariantType>(value.getType() & ~byReference);

	if(! type.isEmpty() && type.getPointerDimension() <= 1) {
		GScopedInterface<IMetaTypedItem> typedItem(context->getService()->findTypedItemByName(type.getBaseName()));
		if(typedItem) {
			bool isReference = type.isReference();

			if(type.getPointerDimension() == 0 && !isReference) {
				GASSERT_MSG(!! metaIsClass(typedItem->getCategory()), "Unknown type");
				GASSERT_MSG(type.baseIsClass(), "Unknown type");

				IMetaClass * metaClass = gdynamic_cast<IMetaClass *>(typedItem.get());
				void * instance = metaClass->cloneInstance(objectAddressFromVariant(value));
				*result = Methods::doObjectToScript(context, context->newClassGlueData(context, gdynamic_cast<IMetaClass *>(typedItem.get())),
					instance, true, metaTypeToCV(type), ogdtNormal);
				return true;
			}

			if(type.getPointerDimension() == 1 || isReference) {
				GASSERT_MSG(!! metaIsClass(typedItem->getCategory()), "Unknown type");

				if(vtIsInterface(vt)) {
					GScopedInterface<IObject> ba(value.data.valueInterface);
					*result = Methods::doObjectToScript(context, context->newClassGlueData(context, gdynamic_cast<IMetaClass *>(typedItem.get())),
						value.data.valueInterface, allowGC,	metaTypeToCV(type), ogdtInterface);
					return true;
				}
				else {
					*result = Methods::doObjectToScript(context, context->newClassGlueData(context, gdynamic_cast<IMetaClass *>(typedItem.get())),
						fromVariant<void *>(value), allowGC, metaTypeToCV(type), ogdtNormal);
					return true;
				}
			}
		}
		else {
			if(vtIsInterface(vt)) {
				IObject * obj = fromVariant<IObject *>(value);
				if(dynamic_cast<IMetaClass *>(obj)) { // !!! GUID
					IMetaClass * metaClass = dynamic_cast<IMetaClass *>(obj);
					*result = Methods::doClassToScript(context, metaClass);

					return true;
				}
			}
		}

		if(shouldRemoveReference(type)) {
			GMetaType newType(type);
			newType.removeReference();

			*result = Methods::doVariantToScript(context, value, newType, allowGC, allowRaw);

			return true;
		}
	}

	if(allowRaw) {
		*result = Methods::doRawToScript(context, value);
		return true;
	}

	return false;
}

template <typename Methods>
bool methodResultToScript(typename Methods::ResultType * result,
	const GContextPointer & context, IMetaCallable * callable, InvokeCallableResult * resultValue)
{
	if(resultValue->resultCount > 0) {
		GMetaTypeData typeData;
	
		callable->getResultType(&typeData);
		metaCheckError(callable);

		GVariant value = resultValue->resultData;
		*result = Methods::doVariantToScript(context, value, GMetaType(typeData), !! callable->isResultTransferOwnership(), false);
		if(! Methods::isSuccessResult(*result)) {
			GScopedInterface<IMetaConverter> converter(metaGetResultExtendType(callable, GExtendTypeCreateFlag_Converter).getConverter());
			*result = Methods::doConverterToScript(context, value, converter.get());
		}
		if(! Methods::isSuccessResult(*result)) {
			*result = Methods::doRawToScript(context, value);
		}
		if(! Methods::isSuccessResult(*result)) {
			raiseCoreException(Error_ScriptBinding_FailVariantToScript);

			return false;
		}
		
		return true;
	}
	
	return false;
}


template <typename Methods>
bool accessibleToScript(typename Methods::ResultType * result,
	const GContextPointer & context, IMetaAccessible * accessible, void * instance, bool instanceIsConst)
{
	GMetaType type;
	GVariant value = getAccessibleValueAndType(instance, accessible, &type, instanceIsConst);

	*result = Methods::doVariantToScript(context, value, type, false, false);
	if(! Methods::isSuccessResult(*result)) {
		GScopedInterface<IMetaConverter> converter(metaGetItemExtendType(accessible, GExtendTypeCreateFlag_Converter).getConverter());
		*result = Methods::doConverterToScript(context, value, converter.get());
	}
	if(! Methods::isSuccessResult(*result)) {
		*result = Methods::doRawToScript(context, value);
	}
	if(! Methods::isSuccessResult(*result)) {
		raiseCoreException(Error_ScriptBinding_FailVariantToScript);

		return false;
	}
		
	return true;
}


template <typename Methods>
bool converterToScript(typename Methods::ResultType * result,
	const GContextPointer & context, const GVariant & value, IMetaConverter * converter)
{
	if(converter == NULL) {
		return false;
	}

	if(isMetaConverterCanRead(converter->capabilityForCString())) {
		gapi_bool needFree;
		
		GScopedInterface<IMemoryAllocator> allocator(context->getService()->getAllocator());
		const char * s = converter->readCString(objectAddressFromVariant(value), &needFree, allocator.get());

		if(s != NULL) {
			*result = Methods::doStringToScript(context, s);

			if(needFree) {
				allocator->free((void *)s);
			}

			return true;
		}
	}

	if(isMetaConverterCanRead(converter->capabilityForCWideString())) {
		gapi_bool needFree;
		
		GScopedInterface<IMemoryAllocator> allocator(context->getService()->getAllocator());
		const wchar_t * ws = converter->readCWideString(objectAddressFromVariant(value), &needFree, allocator.get());

		if(ws != NULL) {
			*result = Methods::doWideStringToScript(context, ws);

			if(needFree) {
				allocator->free((void *)ws);
			}

			return true;
		}
	}

	return false;
}


} // namespace _bind_internal


} // namespace cpgf



#endif
