#ifndef __GBINDCOMMON_H
#define __GBINDCOMMON_H

#include "cpgf/scriptbind/gscriptbind.h"
#include "cpgf/gglobal.h"
#include "cpgf/gflags.h"
#include "cpgf/gmetaclass.h"

#include <map>
#include <vector>
#include <string>


namespace cpgf {


class GMetaMap;

enum ObjectPointerCV {
	opcvNone,
	opcvConst,
	opcvVolatile,
	opcvConstVolatile,
};

struct GBindDataType
{
	GScriptDataType dataType;
	GScopedInterface<IMetaTypedItem> typeItem;
};

class GScriptUserData;

class GScriptBindingParam
{
private:
	typedef std::vector<GScriptUserData *> UserListType;

public:
	GScriptBindingParam(IMetaService * service, const GScriptConfig & config, GMetaMap * metaMap)
		: service(service), config(config), metaMap(metaMap) {
		this->service->addReference();
	}

	~GScriptBindingParam();

	IMetaService * getService() const {
		return this->service.get();
	}

	const GScriptConfig & getConfig() const {
		return this->config;
	}

	GMetaMap * getMetaMap() const {
		return this->metaMap;
	}

	void addUserData(GScriptUserData * userData);
	void removeUserData(GScriptUserData * userData);

private:
	GScopedInterface<IMetaService> service;
	const GScriptConfig & config;
	GMetaMap * metaMap;
	GScopedPointer<UserListType> userDataList;
};

enum GScriptUserDataType {
	udtClass,
	udtMethod,
	udtMethodList,
	udtEnum,
	udtOperator,
	udtAccessible
};

class GScriptUserData
{
public:
	GScriptUserData(GScriptUserDataType type, GScriptBindingParam * param) : type(type), param(param) {
	}

	virtual ~GScriptUserData() {
	}

	GScriptUserDataType getType() const {
		return this->type;
	}

	GScriptBindingParam * getParam() const {
		return this->param;
	}

private:
	GScriptUserDataType type;
	GScriptBindingParam * param;
};

class GClassUserData : public GScriptUserData
{
private:
	typedef GScriptUserData super;

public:
	GClassUserData(GScriptBindingParam * param, IMetaClass * metaClass, void * instance, bool isInstance, bool allowGC, ObjectPointerCV cv)
		: super(udtClass, param), metaClass(metaClass), instance(instance), isInstance(isInstance), allowGC(allowGC), cv(cv) {
		this->metaClass->addReference();
	}

	virtual ~GClassUserData() {
		if(this->allowGC) {
			this->metaClass->destroyInstance(instance);
		}

		this->metaClass->releaseReference();
	}

public:
	IMetaClass * metaClass;
	void * instance;
	bool isInstance;
	bool allowGC;
	ObjectPointerCV cv;
};

class GMethodUserData : public GScriptUserData
{
private:
	typedef GScriptUserData super;

public:
	GMethodUserData(GScriptBindingParam * param, void * instance, IMetaMethod * method)
		: super(udtMethod, param), instance(instance), method(method) {
		this->method->addReference();
	}

	virtual ~GMethodUserData() {
		this->method->releaseReference();
	}

public:
	void * instance;
	IMetaMethod * method;
};

class GMethodListUserData : public GScriptUserData
{
private:
	typedef GScriptUserData super;

public:
	GMethodListUserData(GScriptBindingParam * param, IMetaList * methodList)
		: super(udtMethodList, param), methodList(methodList) {
		this->methodList->addReference();
	}

	virtual ~GMethodListUserData() {
		this->methodList->releaseReference();
	}

public:
	IMetaList * methodList;
};

// NOTE: Now the other code assumes that GOperatorUserData doesn't need to free any resource.
// Whenever GOperatorUserData needs to free resource in destructor, we need to refactor other code to avoid memory leaks.
class GOperatorUserData : public GScriptUserData
{
private:
	typedef GScriptUserData super;

public:
	GOperatorUserData(GScriptBindingParam * param, void * instance, IMetaClass * metaClass, GMetaOpType op)
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
	GEnumUserData(GScriptBindingParam * param, IMetaEnum * metaEnum)
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
	GAccessibleUserData(GScriptBindingParam * param, void * instance, IMetaAccessible * accessible)
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


struct CStringCompare
{
	bool operator () (const char * a, const char * b) const {
		return strcmp(a, b) < 0;
	}
};

class GMetaMapClass
{
private:
	typedef std::map<const char *, GMetaMapItem, CStringCompare> MapType;

public:
	GMetaMapClass(IMetaClass * metaClass);

	GMetaMapItem * findItem(const char * name);

	void setData(GMetaMapItemData * newData) {
		this->data.reset(newData);
	}

	GMetaMapItemData * getData() const {
		return this->data.get();
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
	typedef std::map<const char *, GMetaMapClass *, CStringCompare> MapType;

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

class InvokeCallableParam
{
public:
	InvokeCallableParam() : paramCount(0) {
	}

	~InvokeCallableParam() {
		for(size_t i = 0; i < this->paramCount; ++i) {
			freeVarData(&this->paramsData[i]);
		}
	}

public:
	GVariantData paramsData[REF_MAX_ARITY];
	GBindDataType paramsType[REF_MAX_ARITY];
	size_t paramCount;
};

struct InvokeCallableResult
{
	int resultCount;
	GVariantData resultData;
};

int rankCallable(IMetaService * service, IMetaCallable * callable, InvokeCallableParam * callbackParam);

template <typename Getter, typename Predict>
int findAppropriateCallable(IMetaService * service,
	const Getter & getter, size_t count,
	InvokeCallableParam * callableParam, Predict predict)
{
	int maxRank = -1;
	int maxRankIndex = 0;

	for(size_t i = 0; i < count; ++i) {
		GScopedInterface<IMetaCallable> meta(gdynamic_cast<IMetaCallable *>(getter(static_cast<uint32_t>(i))));
		if(predict(meta.get())) {
			int rank = rankCallable(service, meta.get(), callableParam);
			if(rank > maxRank) {
				maxRank = rank;
				maxRankIndex = static_cast<int>(i);
			}
		}
	}

	return maxRank >= 0 ? maxRankIndex : -1;
}

bool allowInvokeMethod(GClassUserData * userData, IMetaMethod * method);
bool allowAccessData(GClassUserData * userData, IMetaAccessible * accessible);

void doInvokeCallable(void * instance, IMetaCallable * callable, GVariantData * paramsData, size_t paramCount, InvokeCallableResult * result);

class GMetaClassTraveller;
void loadMethodList(GMetaClassTraveller * traveller,
	IMetaList * metaList, GMetaMap * metaMap, GMetaMapItem * mapItem,
	void * instance, GClassUserData * userData, const char * methodName);


} // namespace cpgf



#endif

