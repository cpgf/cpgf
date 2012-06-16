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

GMetaMap * createMetaMap();

enum ObjectPointerCV {
	opcvNone,
	opcvConst,
	opcvVolatile,
	opcvConstVolatile,
};

enum ClassUserDataType {
	cudtNormal,
	cudtInterface
};

struct GBindDataType
{
	GScriptDataType dataType;
	GScopedInterface<IMetaTypedItem> typeItem;
};

class GScriptUserData;

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
	GScopedInterface<IMetaService> service;
	GScriptConfig config;
	GScopedPointer<GMetaMap> metaMap;
};

typedef GSharedPointer<GScriptBindingParam> GBindingParamPointer;

enum GScriptUserDataType {
	udtClass,
	udtMethodList,
	udtExtendMethod,
	udtEnum,
	udtOperator,
	udtAccessible,
	udtRaw
};

class GSharedInstance : public GNoncopyable {
public:
	GSharedInstance();
	GSharedInstance(IMetaClass * metaClass, void * instance, bool isInstance,
		bool allowGC, ObjectPointerCV cv, ClassUserDataType dataType);
	~GSharedInstance();
		
	IMetaClass * metaClass;
	union {
		void * instance;
		IObject * interfaceObject;
	};
	bool isInstance;
	bool allowGC;
	ObjectPointerCV cv;
	ClassUserDataType dataType;
};

class GScriptUserData
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

class GClassUserData : public GScriptUserData
{
private:
	typedef GScriptUserData super;

public:
	explicit GClassUserData(const GBindingParamPointer & param);
	GClassUserData(const GBindingParamPointer & param, IMetaClass * metaClass, void * instance, bool isInstance,
		bool allowGC, ObjectPointerCV cv, ClassUserDataType dataType);
	virtual ~GClassUserData();

	GClassUserData(const GClassUserData &);

	bool isAllowGC() const {
		return this->data->allowGC;
	}

	void * getInstance() const {
		return this->data->instance;
	}

private:
	GClassUserData & operator = (const GClassUserData &);

public:
	GSharedPointer<GSharedInstance> data;
};

class GRawUserData : public GScriptUserData
{
private:
	typedef GScriptUserData super;

public:
	GRawUserData(const GBindingParamPointer & param, const GVariant & v)
		: super(udtRaw, param), data(v) {
	}

public:
	GVariant data;
};

enum GUserDataMethodType {
	udmtMethod,
	udmtMethodList,
	udmtInternal
};

class GMethodListUserData : public GScriptUserData
{
private:
	typedef GScriptUserData super;

public:
	GMethodListUserData(const GBindingParamPointer & param, IMetaList * methodList, GUserDataMethodType methodType)
		: super(udtMethodList, param), methodList(methodList), methodType(methodType) {
		this->methodList->addReference();
	}

	virtual ~GMethodListUserData() {
		this->methodList->releaseReference();
	}

public:
	IMetaList * methodList;
	GUserDataMethodType methodType;
};

class GExtendMethodUserData : public GScriptUserData
{
private:
	typedef GScriptUserData super;

public:
	GExtendMethodUserData(const GBindingParamPointer & param, IMetaClass * metaClass, IMetaList * methodList, const char * name, GUserDataMethodType methodType)
		: super(udtExtendMethod, param), metaClass(metaClass), methodList(methodList), name(name), methodType(methodType) {
		if(this->metaClass != NULL) {
			this->metaClass->addReference();
		}
		if(this->methodList != NULL) {
			this->methodList->addReference();
		}
	}

	virtual ~GExtendMethodUserData() {
		if(this->metaClass != NULL) {
			this->metaClass->releaseReference();
		}
		if(this->methodList != NULL) {
			this->methodList->releaseReference();
		}
	}

public:
	IMetaClass * metaClass;
	IMetaList * methodList;
	std::string name;
	GUserDataMethodType methodType;
};

class GOperatorUserData : public GScriptUserData
{
private:
	typedef GScriptUserData super;

public:
	GOperatorUserData(const GBindingParamPointer & param, void * instance, IMetaClass * metaClass, GMetaOpType op)
		: super(udtOperator, param), instance(instance), metaClass(metaClass), op(op) {
	}

	virtual ~GOperatorUserData() {
	}

public:
	void * instance;
	IMetaClass * metaClass;
	GMetaOpType op;
};

class GEnumUserData : public GScriptUserData
{
private:
	typedef GScriptUserData super;

public:
	GEnumUserData(const GBindingParamPointer & param, IMetaEnum * metaEnum)
		: super(udtEnum, param), metaEnum(metaEnum) {
		this->metaEnum->addReference();
	}

	virtual ~GEnumUserData() {
		this->metaEnum->releaseReference();
	}

public:
	IMetaEnum * metaEnum;
};

class GAccessibleUserData : public GScriptUserData
{
private:
	typedef GScriptUserData super;

public:
	GAccessibleUserData(const GBindingParamPointer & param, void * instance, IMetaAccessible * accessible)
		: super(udtAccessible, param), instance(instance), accessible(accessible) {
		this->accessible->addReference();
	}

	virtual ~GAccessibleUserData() {
		this->accessible->releaseReference();
	}

public:
	void * instance;
	IMetaAccessible * accessible;
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
	GMetaMapItem(IMetaList * metaList);
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
	IObject * item;
	GMetaMapItemType type;
	size_t enumIndex;
	GScopedPointer<GMetaMapItemData> data;
};

inline void swap(GMetaMapItem & a, GMetaMapItem & b)
{
	a.swap(b);
}


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

bool metaMapItemIsAccessible(GMetaMapItemType type);
bool metaMapItemIsInvokable(GMetaMapItemType type);
GMetaMapItem * findMetaMapItem(GMetaMap * metaMap, IMetaClass * metaClass, const char * itemName);


ObjectPointerCV metaTypeToCV(const GMetaType & type);

struct FindCallablePredict {
	bool operator () (IMetaCallable *) {
		return true;
	}
};

struct OperatorCallablePredict {
	explicit OperatorCallablePredict(GMetaOpType op) : op(op) {}

	bool operator () (IMetaCallable * t) {
		return gdynamic_cast<IMetaOperator *>(t)->getOperator() == this->op;
	}

private:
	GMetaOpType op;
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

class InvokeCallableParam
{
public:
	explicit InvokeCallableParam(size_t paramCount);
	~InvokeCallableParam();

public:
	GVariantData paramsData[REF_MAX_ARITY];
	GBindDataType paramsType[REF_MAX_ARITY];
	size_t paramCount;
	InvokeParamRank paramsRank;
};

class InvokeCallableResult
{
public:
	InvokeCallableResult();
	~InvokeCallableResult();

public:
	int resultCount;
	GVariantData resultData;
};

bool variantIsScriptRawData(GVariantType vt);

int rankCallable(IMetaService * service, IMetaCallable * callable, InvokeCallableParam * callbackParam, InvokeParamRank * paramsRank);

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

bool allowInvokeCallable(GClassUserData * userData, IMetaCallable * method);
bool allowAccessData(GClassUserData * userData, IMetaAccessible * accessible);

void * doInvokeConstructor(IMetaService * service, IMetaClass * metaClass, InvokeCallableParam * callableParam);
void doInvokeCallable(void * instance, IMetaCallable * callable, InvokeCallableParam * callableParam, InvokeCallableResult * result);

void loadMethodList(GMetaClassTraveller * traveller,
	IMetaList * metaList, GMetaMap * metaMap, GMetaMapItem * mapItem,
	void * instance, GClassUserData * userData, const char * methodName, bool allowAny);
void loadMethodList(GMetaClassTraveller * traveller,
	IMetaList * metaList, GMetaMap * metaMap, GMetaMapItem * mapItem,
	void * instance, GClassUserData * userData, const char * methodName);

void loadMethodList(IMetaList * methodList, GMetaMap * metaMap, IMetaClass * objectMetaClass,
	void * objectInstance, GClassUserData * userData, const char * methodName);

GScriptDataType methodTypeToUserDataType(GUserDataMethodType methodType);

GMetaVariant userDataToVariant(GScriptUserData * userData);

GVariant getAccessibleValueAndType(void * instance, IMetaAccessible * accessible, GMetaType * outType, bool instanceIsConst);

bool shouldRemoveReference(const GMetaType & type);

// We only apply implicit convert (such as WideString to String) on method parameter.
// Don't do it for field access because it requires temporary memory allocation which is invalid after the access.
int rankImplicitConvert(const GVariantData & sourceData, const GMetaType & targetType);

wchar_t * stringToWideString(const char * s);
char * wideStringToString(const wchar_t * ws);


} // namespace bind_internal


} // namespace cpgf



#endif

