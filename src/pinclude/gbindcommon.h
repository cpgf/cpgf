#ifndef __GBINDCOMMON_H
#define __GBINDCOMMON_H

#include "cpgf/scriptbind/gscriptbind.h"
#include "cpgf/gglobal.h"
#include "cpgf/gflags.h"
#include "cpgf/gclassutil.h"
#include "cpgf/gbytearrayapi.h"
#include "cpgf/gmetaoperatorop.h"
#include "cpgf/gsharedptr.h"

#include <map>
#include <vector>
#include <string>


namespace cpgf {

class GMetaClassTraveller;


namespace bind_internal {


class GMetaMap;

enum ObjectPointerCV {
	opcvNone,
	opcvConst,
	opcvVolatile,
	opcvConstVolatile,
};

enum ClassUserDataType {
	cudtClass,
	cudtNormal,
	cudtInterface
};

enum GScriptUserDataType {
	udtObject,
	udtExtendMethod,
	udtEnum,
	udtOperator,
	udtAccessible,
	udtRaw,
	udtMethod
};

enum GUserDataMethodType {
	udmtMethod,
	udmtMethodList,
	udmtInternal
};

class GScriptBindingParam : public GNoncopyable
{
public:
	GScriptBindingParam(IMetaService * service, const GScriptConfig & config);
	virtual ~GScriptBindingParam();

	IMetaService * getService() const {
		return this->service.get();
	}

	const GScriptConfig & getConfig() const {
		return this->config;
	}

	GMetaMap * getMetaMap() const {
		return this->metaMap.get();
	}

private:
	GSharedInterface<IMetaService> service;
	GScriptConfig config;
	GScopedPointer<GMetaMap> metaMap;
};

typedef GSharedPointer<GScriptBindingParam> GBindingParamPointer;

class GObjectData
{
public:
	GObjectData();
	GObjectData(IMetaClass * metaClass, void * instance,
		bool allowGC, ObjectPointerCV cv, ClassUserDataType dataType);
	~GObjectData();

	IMetaClass * getMetaClass() const {
		return this->metaClass.get();
	}

	void * getInstance() const {
		return this->instance;
	}

	IObject * getInterfaceObject() const {
		return this->interfaceObject.get();
	}

	bool isInstance() const {
		return this->dataType != cudtClass;
	}

	bool isAllowGC() const {
		return this->allowGC;
	}

	ObjectPointerCV getCV() const {
		return this->cv;
	}

	ClassUserDataType getDataType() const {
		return this->dataType;
	}

private:
	GSharedInterface<IMetaClass> metaClass;
	void * instance;
	GSharedInterface<IObject> interfaceObject;
	bool allowGC;
	ObjectPointerCV cv;
	ClassUserDataType dataType;
};

typedef GSharedPointer<GObjectData> GSharedObjectData;

class GMethodData
{
public:
	GMethodData(IMetaClass * metaClass, IMetaList * methodList, const char * name, GUserDataMethodType methodType)
		: metaClass(metaClass), methodList(methodList), name(name), methodType(methodType) {
	}

	IMetaClass * getMetaClass() const {
		return this->metaClass.get();
	}

	IMetaList * getMethodList() const {
		return this->methodList.get();
	}

	const std::string & getName() const {
		return this->name;
	}

	void setName(const std::string & name) const {
		this->name = name;
	}

	GUserDataMethodType getMethodType() const {
		return this->methodType;
	}

private:
	GSharedInterface<IMetaClass> metaClass;
	GSharedInterface<IMetaList> methodList;
	mutable std::string name;
	GUserDataMethodType methodType;
};

class GScriptUserData : public GNoncopyable
{
public:
	GScriptUserData(GScriptUserDataType type, const GBindingParamPointer & param) : type(type), param(param) {
	}

	virtual ~GScriptUserData() {
	}

	GScriptUserDataType getType() const {
		return this->type;
	}

	const GBindingParamPointer & getParam() const {
		return this->param;
	}

private:
	GScriptUserDataType type;
	GBindingParamPointer param;
};

class GObjectUserData : public GScriptUserData
{
private:
	typedef GScriptUserData super;

public:
	GObjectUserData(const GBindingParamPointer & param, IMetaClass * metaClass, void * instance,
		bool allowGC, ObjectPointerCV cv, ClassUserDataType dataType);
	GObjectUserData(const GBindingParamPointer & param, const GSharedObjectData & data);

	const GSharedObjectData & getObjectData() const {
		return this->data;
	}

private:
	GSharedObjectData data;
};

class GRawUserData : public GScriptUserData
{
private:
	typedef GScriptUserData super;

public:
	GRawUserData(const GBindingParamPointer & param, const GVariant & v)
		: super(udtRaw, param), data(v) {
	}

	const GVariant & getData() const {
		return this->data;
	}

private:
	GVariant data;
};

class GMethodUserData : public GScriptUserData
{
private:
	typedef GScriptUserData super;

public:
	GMethodUserData(const GBindingParamPointer & param, IMetaClass * metaClass, IMetaList * methodList, const char * name, GUserDataMethodType methodType)
		: super(udtExtendMethod, param), data(metaClass, methodList, name, methodType) {
	}

	const GMethodData & getMethodData() const {
		return this->data;
	}

private:
	GMethodData data;
};

class GObjectMethodUserData : public GScriptUserData
{
private:
	typedef GScriptUserData super;

public:
	GObjectMethodUserData(const GBindingParamPointer & param, const GSharedObjectData & objectData, const GMethodData & methodData)
		: super(udtMethod, param), objectData(objectData), methodData(methodData) {
	}

	const GSharedObjectData & getObjectData() const {
		return this->objectData;
	}

	const GMethodData & getMethodData() const {
		return this->methodData;
	}

private:
	GSharedObjectData objectData;
	GMethodData methodData;
};

class GOperatorUserData : public GScriptUserData
{
private:
	typedef GScriptUserData super;

public:
	GOperatorUserData(const GBindingParamPointer & param, void * instance, IMetaClass * metaClass, GMetaOpType op)
		: super(udtOperator, param), instance(instance), metaClass(metaClass), op(op) {
	}

	void * getInstance() const {
		return this->instance;
	}

	IMetaClass * getMetaClass() const {
		return this->metaClass.get();
	}

	GMetaOpType getOp() const {
		return this->op;
	}

private:
	void * instance;
	GSharedInterface<IMetaClass> metaClass;
	GMetaOpType op;
};

class GEnumUserData : public GScriptUserData
{
private:
	typedef GScriptUserData super;

public:
	GEnumUserData(const GBindingParamPointer & param, IMetaEnum * metaEnum)
		: super(udtEnum, param), metaEnum(metaEnum) {
	}

	IMetaEnum * getMetaEnum() const {
		return this->metaEnum.get();
	}

private:
	GSharedInterface<IMetaEnum> metaEnum;
};

class GAccessibleUserData : public GScriptUserData
{
private:
	typedef GScriptUserData super;

public:
	GAccessibleUserData(const GBindingParamPointer & param, void * instance, IMetaAccessible * accessible)
		: super(udtAccessible, param), instance(instance), accessible(accessible) {
	}

	void * getInstance() const {
		return this->instance;
	}

	IMetaAccessible * getAccessible() const {
		return this->accessible.get();
	}

private:
	void * instance;
	GSharedInterface<IMetaAccessible> accessible;
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

class GMetaMapItemData
{
public:
	GMetaMapItemData() {}
	virtual ~GMetaMapItemData() {}
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

	void setData(GMetaMapItemData * newData) {
		this->data.reset(newData);
	}

	GMetaMapItemData * getData() const {
		return this->data.get();
	}

private:
	GSharedInterface<IObject> item;
	GMetaMapItemType type;
	size_t enumIndex;
	GScopedPointer<GMetaMapItemData> data;
};

inline void swap(GMetaMapItem & a, GMetaMapItem & b)
{
	a.swap(b);
}

class GMapItemMethodData : public GMetaMapItemData
{
public:
	explicit GMapItemMethodData(const GMethodData & methodData)
		: methodData(methodData) {
	}

	const GMethodData & getMethodData() const {
		return this->methodData;
	}

private:
	GMethodData methodData;
};

class GMetaMapClass
{
public:
	typedef std::map<const char *, GMetaMapItem, meta_internal::CStringCompare> MapType;

public:
	GMetaMapClass(IMetaClass * metaClass);

	GMetaMapItem * findItem(const char * name);

	void setData(GMetaMapItemData * newData) {
		this->data.reset(newData);
	}

	GMetaMapItemData * getData() const {
		return this->data.get();
	}
	
	const MapType * getMap() const {
		return &this->itemMap;
	}

private:
	void buildMap(IMetaClass * metaClass);

private:
	MapType itemMap;
	GScopedPointer<GMetaMapItemData> data;
};

class GMetaMap
{
private:
	typedef std::map<const char *, GMetaMapClass *, meta_internal::CStringCompare> MapType;

public:
	GMetaMap();
	~GMetaMap();

	GMetaMapClass * findClassMap(IMetaClass * metaClass);

private:
	MapType classMap;
};

class InvokeParamRank
{
public:
	explicit InvokeParamRank(size_t paramCount);
	InvokeParamRank(const InvokeParamRank & other);
	InvokeParamRank & operator = (const InvokeParamRank & other);

public:
	size_t paramCount;
	int ranks[REF_MAX_ARITY];
};

struct CallableParamDataType
{
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

GMetaMap * createMetaMap();

GMetaMapItem * findMetaMapItem(GMetaMap * metaMap, IMetaClass * metaClass, const char * itemName);

ObjectPointerCV metaTypeToCV(const GMetaType & type);

int rankCallable(IMetaService * service, IMetaCallable * callable, InvokeCallableParam * callbackParam, InvokeParamRank * paramsRank);

bool allowAccessData(const GScriptConfig & config, GObjectData * userData, IMetaAccessible * accessible);

void * doInvokeConstructor(IMetaService * service, IMetaClass * metaClass, InvokeCallableParam * callableParam);

InvokeCallableResult doCallbackMethodList(const GBindingParamPointer & param, GObjectUserData * objectUserData, const GMethodData & methodData, InvokeCallableParam * callableParam);
InvokeCallableResult doInvokeOperator(const GBindingParamPointer & param, void * instance, IMetaClass * metaClass, GMetaOpType op, InvokeCallableParam * callableParam);

void loadMethodList(GMetaClassTraveller * traveller,
	IMetaList * methodList, GMetaMap * metaMap, GMetaMapItem * mapItem,
	void * instance, const char * methodName);

GScriptDataType methodTypeToUserDataType(GUserDataMethodType methodType);

GMetaVariant userDataToVariant(GScriptUserData * userData);

GVariant getAccessibleValueAndType(void * instance, IMetaAccessible * accessible, GMetaType * outType, bool instanceIsConst);

bool shouldRemoveReference(const GMetaType & type);

wchar_t * stringToWideString(const char * s);
char * wideStringToString(const wchar_t * ws);

GSharedObjectData getSharedObjectData(GObjectUserData * objectUserData);
GObjectData * getObjectData(GObjectUserData * objectUserData);

IMetaClass * selectBoundClass(IMetaClass * currentClass, IMetaClass * derived);


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
	const GBindingParamPointer & param, const GVariant & value, const GMetaType & type, bool allowGC, bool allowRaw)
{
	GVariantType vt = static_cast<GVariantType>(value.getType() & ~byReference);

	if(! type.isEmpty() && type.getPointerDimension() <= 1) {
		GScopedInterface<IMetaTypedItem> typedItem(param->getService()->findTypedItemByName(type.getBaseName()));
		if(typedItem) {
			bool isReference = type.isReference();

			if(type.getPointerDimension() == 0 && !isReference) {
				GASSERT_MSG(!! metaIsClass(typedItem->getCategory()), "Unknown type");
				GASSERT_MSG(type.baseIsClass(), "Unknown type");

				IMetaClass * metaClass = gdynamic_cast<IMetaClass *>(typedItem.get());
				void * instance = metaClass->cloneInstance(objectAddressFromVariant(value));
				*result = Methods::doObjectToScript(param, instance, gdynamic_cast<IMetaClass *>(typedItem.get()), true, metaTypeToCV(type), cudtNormal);
				return true;
			}

			if(type.getPointerDimension() == 1 || isReference) {
				GASSERT_MSG(!! metaIsClass(typedItem->getCategory()), "Unknown type");

				if(vtIsInterface(vt)) {
					GScopedInterface<IObject> ba(value.data.valueInterface);
					*result = Methods::doObjectToScript(param, value.data.valueInterface, gdynamic_cast<IMetaClass *>(typedItem.get()), allowGC,
						metaTypeToCV(type), cudtInterface);
					return true;
				}
				else {
					*result = Methods::doObjectToScript(param, fromVariant<void *>(value), gdynamic_cast<IMetaClass *>(typedItem.get()), allowGC, metaTypeToCV(type), cudtNormal);
					return true;
				}
			}
		}

		if(bind_internal::shouldRemoveReference(type)) {
			GMetaType newType(type);
			newType.removeReference();

			*result = Methods::doVariantToScript(param, value, newType, allowGC, allowRaw);

			return true;
		}
	}

	if(allowRaw) {
		*result = Methods::doRawToScript(param, value);
		return true;
	}

	return false;
}

template <typename Methods>
bool methodResultToScript(typename Methods::ResultType * result,
	const GBindingParamPointer & param, IMetaCallable * callable, InvokeCallableResult * resultValue)
{
	if(resultValue->resultCount > 0) {
		GMetaTypeData typeData;
	
		callable->getResultType(&typeData);
		metaCheckError(callable);

		GVariant value = resultValue->resultData;
		*result = Methods::doVariantToScript(param, value, GMetaType(typeData), !! callable->isResultTransferOwnership(), false);
		if(! Methods::isSuccessResult(*result)) {
			GScopedInterface<IMetaConverter> converter(metaGetResultExtendType(callable, GExtendTypeCreateFlag_Converter).getConverter());
			*result = Methods::doConverterToScript(param, value, converter.get());
		}
		if(! Methods::isSuccessResult(*result)) {
			*result = Methods::doRawToScript(param, value);
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
	const GBindingParamPointer & param, IMetaAccessible * accessible, void * instance, bool instanceIsConst)
{
	GMetaType type;
	GVariant value = getAccessibleValueAndType(instance, accessible, &type, instanceIsConst);

	*result = Methods::doVariantToScript(param, value, type, false, false);
	if(! Methods::isSuccessResult(*result)) {
		GScopedInterface<IMetaConverter> converter(metaGetItemExtendType(accessible, GExtendTypeCreateFlag_Converter).getConverter());
		*result = Methods::doConverterToScript(param, value, converter.get());
	}
	if(! Methods::isSuccessResult(*result)) {
		*result = Methods::doRawToScript(param, value);
	}
	if(! Methods::isSuccessResult(*result)) {
		raiseCoreException(Error_ScriptBinding_FailVariantToScript);

		return false;
	}
		
	return true;
}


template <typename Methods>
bool converterToScript(typename Methods::ResultType * result,
	const GBindingParamPointer & param, const GVariant & value, IMetaConverter * converter)
{
	if(converter == NULL) {
		return false;
	}

	if(isMetaConverterCanRead(converter->capabilityForCString())) {
		gapi_bool needFree;
		
		GScopedInterface<IMemoryAllocator> allocator(param->getService()->getAllocator());
		const char * s = converter->readCString(objectAddressFromVariant(value), &needFree, allocator.get());

		if(s != NULL) {
			*result = Methods::doStringToScript(param, s);

			if(needFree) {
				allocator->free((void *)s);
			}

			return true;
		}
	}

	if(isMetaConverterCanRead(converter->capabilityForCWideString())) {
		gapi_bool needFree;
		
		GScopedInterface<IMemoryAllocator> allocator(param->getService()->getAllocator());
		const wchar_t * ws = converter->readCWideString(objectAddressFromVariant(value), &needFree, allocator.get());

		if(ws != NULL) {
			*result = Methods::doWideStringToScript(param, ws);

			if(needFree) {
				allocator->free((void *)ws);
			}

			return true;
		}
	}

	return false;
}


} // namespace bind_internal


} // namespace cpgf



#endif

