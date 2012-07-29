#include "../pinclude/gbindcommon.h"

#include "cpgf/gmetaclasstraveller.h"
#include "cpgf/gcallback.h"
#include "cpgf/scriptbind/gscriptservice.h"

#include <stdarg.h>
#include <stdlib.h>

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4996)
#endif


namespace cpgf {

GScriptCoreService * doCreateScriptCoreService(GScriptObject * scriptObject);
GMetaClass * doBindScriptCoreService(GScriptObject * scriptObject, const char * bindName, GScriptCoreService * scriptCoreService);

namespace bind_internal {

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


// such as 2 or more dimensions pointer
const int ParamMatchRank_Unknown = 0;
const int ParamMatchRank_Convert = 50000;
const int ParamMatchRank_Implicit_Begin = 70000;
const int ParamMatchRank_Implicit_WideStringToString = ParamMatchRank_Implicit_Begin + 0;
const int ParamMatchRank_Implicit_StringToWideString = ParamMatchRank_Implicit_Begin + 1;
const int ParamMatchRank_Implicit_End = 80000;
const int ParamMatchRank_Equal = 100000;


GScriptBindingParam::GScriptBindingParam(IMetaService * service, const GScriptConfig & config)
	: service(service), config(config), metaMap(new GMetaMap)
{
}

GScriptBindingParam::~GScriptBindingParam()
{
}

void GScriptBindingParam::bindScriptCoreService(GScriptObject * scriptObject, const char * bindName)
{
	if(this->scriptCoreService) {
		return;
	}

	this->scriptCoreService.reset(doCreateScriptCoreService(scriptObject));
	this->scriptCoreServiceMetaClass.reset(doBindScriptCoreService(scriptObject, bindName, this->scriptCoreService.get()));
}


void GScriptDataHolder::requireDataMap()
{
	if(! this->dataMap) {
		this->dataMap.reset(new MapType());
	}
}

void GScriptDataHolder::setScriptValue(const char * name, const GVariant & value)
{
	this->requireDataMap();

	(*(this->dataMap))[name] = value;
}

IScriptFunction * GScriptDataHolder::getScriptFunction(const char * name)
{
	if(this->dataMap)
	{
		MapType::iterator it = this->dataMap->find(name);
		if(it != this->dataMap->end()) {
			if(vtIsInterface(it->second.getType())) {
				IScriptFunction * func = gdynamic_cast<IScriptFunction *>(fromVariant<IObject *>(it->second));
				if(func != NULL) {
					func->addReference();
				}
				return func;
			}
		}
	}
	return NULL;
}

void GScriptDataHolder::merge(const GScriptDataHolder * other)
{
	if(other->dataMap) {
		this->requireDataMap();
		this->dataMap->insert(other->dataMap->begin(), other->dataMap->end());
	}
}


GScriptDataExtendStorage::GScriptDataExtendStorage(GScriptBindingParam * param, IMetaClass * metaClass, bool isInstance)
	: param(param), metaClass(metaClass), isInstance(isInstance)
{
//	this->isInstance = true;
}

void GScriptDataExtendStorage::setScriptValue(const char * name, const GVariant & value)
{
	GScriptDataHolder * storage = NULL;
	if(! this->isInstance) {
		GMetaMapClass * mapClass = this->param->getMetaMap()->findClassMap(this->metaClass.get());
		storage = mapClass->getDataStorage();
	}
	else {
		storage = this->getDataHolder();
	}
	if(storage != NULL) {
		storage->setScriptValue(name, value);
	}
}

IScriptFunction * G_API_CC GScriptDataExtendStorage::getScriptFunction(const char * name)
{
	IScriptFunction * func = NULL;
	
	if(this->isInstance) {
		if(this->instanceDataStorage) {
			func = this->instanceDataStorage->getScriptFunction(name);
		}
	}

	if(func == NULL) {
		GMetaMapClass * mapClass = this->param->getMetaMap()->findClassMap(this->metaClass.get());
		if(mapClass->hasDataStorage()) {
			func = mapClass->getDataStorage()->getScriptFunction(name);
		}
		
		if(this->classDataStorage) {
//			func = this->classDataStorage->getScriptFunction(name);
		}
	}

	return func;
}

bool GScriptDataExtendStorage::hasDataHolder()
{
	return this->instanceDataStorage;
}

GScriptDataHolder * GScriptDataExtendStorage::getDataHolder()
{
	if(! this->instanceDataStorage) {
		this->instanceDataStorage.reset(new GScriptDataHolder());
	}
	return this->instanceDataStorage.get();
}

void GScriptDataExtendStorage::setClassDataStorage(IScriptDataExtendStorage * classDataStorage)
{
	this->classDataStorage.reset(classDataStorage);
}


GObjectData::GObjectData(GScriptBindingParam * param, IMetaClass * metaClass, void * instance,
	bool allowGC, ObjectPointerCV cv, ObjectUserDataType dataType)
	: param(param), metaClass(metaClass), instance(instance), allowGC(allowGC), cv(cv), dataType(dataType)
{
	switch(dataType) {
		case cudtInterface:
			this->allowGC = false;
			this->interfaceObject.reset(static_cast<IObject *>(instance));
			break;

		default:
			break;
	}

	if(this->isInstance()) {
		GScopedInterface<IMetaScriptWrapper> scriptWrapper(metaGetItemExtendType(metaClass, GExtendTypeCreateFlag_ScriptWrapper).getScriptWrapper());
		if(scriptWrapper) {
			scriptWrapper->setScriptDataStorage(instance, this->getDataStorage());
		}
	}
}

GObjectData::~GObjectData()
{
	if(this->metaClass) {
		if(this->allowGC) {
			this->metaClass->destroyInstance(instance);
		}
	}
}

IScriptDataExtendStorage * GObjectData::getDataStorage() const
{
	if(! this->dataStorage) {
		this->dataStorage.reset(new GScriptDataExtendStorage(this->param, this->metaClass.get(), this->isInstance()));
	}
	return this->dataStorage.get();
}

void GObjectData::setScriptValue(const char * name, const GVariant & value)
{
	this->getDataStorage()->setScriptValue(name, value);
}

void GObjectData::mergeDataStorage(const GObjectData * other)
{
	if(other->dataStorage && other->dataStorage->hasDataHolder()) {
		this->getDataStorage()->getDataHolder()->merge(other->dataStorage->getDataHolder());
	}
}


GObjectUserData::GObjectUserData(const GBindingParamPointer & param, IMetaClass * metaClass, void * instance,
	bool allowGC, ObjectPointerCV cv, ObjectUserDataType dataType)
	: super(udtObject, param), data(new GObjectData(param.get(), metaClass, instance, allowGC, cv, dataType))
{
}

GObjectUserData::GObjectUserData(const GBindingParamPointer & param, const GSharedObjectData & data)
	: super(udtObject, param), data(data)
{
}


GMetaMapItem::GMetaMapItem()
	: item(NULL), type(mmitNone), enumIndex(0)
{
}

GMetaMapItem::GMetaMapItem(IMetaItem * item, GMetaMapItemType type)
	: item(item), type(type), enumIndex(0)
{
}

GMetaMapItem::GMetaMapItem(size_t enumIndex, IMetaEnum * item)
	: item(item), type(mmitEnumValue), enumIndex(enumIndex)
{
}

GMetaMapItem::GMetaMapItem(IMetaList * metaList)
	: item(metaList), type(mmitMethodList), enumIndex(0)
{
}

GMetaMapItem::GMetaMapItem(const GMetaMapItem & other)
	: item(other.item), type(other.type), enumIndex(other.enumIndex)
{
}

GMetaMapItem::~GMetaMapItem()
{
}

GMetaMapItem & GMetaMapItem::operator = (GMetaMapItem other)
{
	this->swap(other);

	return *this;
}

void GMetaMapItem::swap(GMetaMapItem & other)
{
	using std::swap;
	
	swap(this->item, other.item);
	swap(this->type, other.type);
	swap(this->enumIndex, other.enumIndex);
	swap(this->data, other.data);
}

GMetaMapItemType GMetaMapItem::getType() const
{
	return this->type;
}

IObject * GMetaMapItem::getItem() const
{
	if(! this->item) {
		return NULL;
	}
	
	this->item->addReference();
	
	return this->item.get();
}


GMetaMapClass::GMetaMapClass(IMetaClass * metaClass)
{
	this->buildMap(metaClass);
}

GMetaMapItem * GMetaMapClass::findItem(const char * name)
{
	MapType::iterator it = this->itemMap.find(name);
	if(it == this->itemMap.end()) {
		return NULL;
	}
	else {
		return &this->itemMap[name];
	}
}

void GMetaMapClass::buildMap(IMetaClass * metaClass)
{
	using namespace std;
	
	uint32_t count;
	uint32_t i;
	
	count = metaClass->getClassCount();
	for(i = 0; i < count; ++i) {
		GScopedInterface<IMetaClass> innerClass(metaClass->getClassAt(i));
		const char * name = innerClass->getName();
		this->itemMap[name] = GMetaMapItem(innerClass.get(), mmitClass);
	}

	count = metaClass->getEnumCount();
	for(i = 0; i < count; ++i) {
		GScopedInterface<IMetaEnum> metaEnum(metaClass->getEnumAt(i));
		const char * name = metaEnum->getName();
		this->itemMap[name] = GMetaMapItem(metaEnum.get(), mmitEnum);
		
		uint32_t keyCount = metaEnum->getCount();
		for(uint32_t k = 0; k < keyCount; ++k) {
			const char * keyName = metaEnum->getKey(k);
			this->itemMap[keyName] = GMetaMapItem(k, metaEnum.get());
		}
	}
	
	count = metaClass->getFieldCount();
	for(i = 0; i < count; ++i) {
		GScopedInterface<IMetaField> field(metaClass->getFieldAt(i));
		const char * name = field->getName();
		this->itemMap[name] = GMetaMapItem(field.get(), mmitField);
	}
	
	count = metaClass->getPropertyCount();
	for(i = 0; i < count; ++i) {
		GScopedInterface<IMetaProperty> prop(metaClass->getPropertyAt(i));
		const char * name = prop->getName();
		this->itemMap[name] = GMetaMapItem(prop.get(), mmitProperty);
	}
	
	count = metaClass->getMethodCount();
	for(i = 0; i < count; ++i) {
		GScopedInterface<IMetaMethod> method(metaClass->getMethodAt(i));
		const char * name = method->getName();
		MapType::iterator it = this->itemMap.find(name);
		if(it == this->itemMap.end()) {
			this->itemMap[name] = GMetaMapItem(method.get(), mmitMethod);
		}
		else {
			GMetaMapItem & item = it->second;
			if(item.getType() == mmitMethod) {
				GScopedInterface<IMetaList> metaList(createMetaList());
				GScopedInterface<IMetaItem> metaItem(gdynamic_cast<IMetaItem *>(item.getItem()));
				metaList->add(metaItem.get(), NULL);
				metaList->add(method.get(), NULL);
				this->itemMap[name] = GMetaMapItem(metaList.get());
			}
			else {
				GASSERT(item.getType() == mmitMethodList);
				
				GScopedInterface<IMetaList> metaList(gdynamic_cast<IMetaList *>(item.getItem()));
				metaList->add(method.get(), NULL);
			}
		}
	}
}


GMetaMap::GMetaMap()
{
}

GMetaMap::~GMetaMap()
{
	for(MapType::iterator it = this->classMap.begin(); it != this->classMap.end(); ++it) {
		delete it->second;
	}
}

GMetaMapClass * GMetaMap::findClassMap(IMetaClass * metaClass)
{
	using namespace std;
	
	const char * name = metaClass->getQualifiedName();
	MapType::iterator it = this->classMap.find(name);
	
	if(it != this->classMap.end()) {
		return it->second;
	}
	else {
		return this->classMap.insert(MapType::value_type(name, new GMetaMapClass(metaClass))).first->second;
	}
}


InvokeParamRank::InvokeParamRank(size_t paramCount) : paramCount(paramCount)
{
	for(size_t i = 0; i < this->paramCount; ++i) {
		this->ranks[i] = ParamMatchRank_Unknown;
	}
}

InvokeParamRank::InvokeParamRank(const InvokeParamRank & other) : paramCount(other.paramCount)
{
	for(size_t i = 0; i < this->paramCount; ++i) {
		this->ranks[i] = other.ranks[i];
	}
}

InvokeParamRank & InvokeParamRank::operator = (const InvokeParamRank & other)
{
	this->paramCount = other.paramCount;

	for(size_t i = 0; i < this->paramCount; ++i) {
		this->ranks[i] = other.ranks[i];
	}

	return *this;
}


InvokeCallableParam::InvokeCallableParam(size_t paramCount)
	: paramCount(paramCount), paramsRank(paramCount)
{
	if(this->paramCount > REF_MAX_ARITY) {
		raiseCoreException(Error_ScriptBinding_CallMethodWithTooManyParameters);
	}
}

InvokeCallableParam::~InvokeCallableParam()
{
}


GMetaMapItem * findMetaMapItem(GMetaMap * metaMap, IMetaClass * metaClass, const char * itemName)
{
	GMetaMapClass * mapClass = metaMap->findClassMap(metaClass);

	if(mapClass != NULL) {
		return mapClass->findItem(itemName);
	}
	else {
		return NULL;
	}
}


ObjectPointerCV metaTypeToCV(const GMetaType & type)
{
	if(type.isPointer()) {
		if(type.isPointerToConst()) {
			return opcvConst;
		}
		else if(type.isPointerToVolatile()) {
			return opcvVolatile;
		}
		else if(type.isPointerToConstVolatile()) {
			return opcvConstVolatile;
		}
	}
	else {
		if(type.isConst()) {
			return opcvConst;
		}
		else if(type.isVolatile()) {
			return opcvVolatile;
		}
		else if(type.isConstVolatile()) {
			return opcvConstVolatile;
		}
	}
	
	return opcvNone;
}


bool isParamImplicitConvert(int paramRank)
{
	return paramRank >= ParamMatchRank_Implicit_Begin && paramRank < ParamMatchRank_Implicit_End;
}

int rankImplicitConvert(const GVariant & sourceData, const GMetaType & targetType)
{
	if(variantIsString(sourceData) && targetType.isWideString()) {
		return ParamMatchRank_Implicit_StringToWideString;
	}

	if(variantIsWideString(sourceData) && targetType.isString()) {
		return ParamMatchRank_Implicit_WideStringToString;
	}

	return ParamMatchRank_Unknown;
}

int rankCallableParam(IMetaService * service, IMetaCallable * callable, const InvokeCallableParam * callbackParam, size_t paramIndex)
{
	GMetaType proto = metaGetParamType(callable, paramIndex);
	GScriptDataType sdt = callbackParam->paramsType[paramIndex].dataType;
	
	if(sdt == sdtNull) {
		return ParamMatchRank_Equal;
	}
	
	if(proto.isFundamental() && sdt == sdtFundamental) {
		return ParamMatchRank_Equal;
	}

	if(sdt == sdtScriptMethod && vtIsInterface(proto.getVariantType())) {
		return ParamMatchRank_Convert;
	}

	if(proto.getPointerDimension() > 1) {
		return ParamMatchRank_Unknown;
	}

	int implicitRank = rankImplicitConvert(callbackParam->paramsData[paramIndex], proto);
	if(implicitRank != ParamMatchRank_Unknown) {
		return implicitRank;
	}

	// check for meta class

	if(! callbackParam->paramsType[paramIndex].typeItem) {
		return ParamMatchRank_Unknown;
	}

	if(metaIsClass(callbackParam->paramsType[paramIndex].typeItem->getCategory())) {
		GScopedInterface<IMetaTypedItem> protoType(service->findTypedItemByName(proto.getBaseName()));
		if(! protoType || ! metaIsClass(protoType->getCategory())) {
			return ParamMatchRank_Unknown;
		}

		IMetaClass * paramClass = static_cast<IMetaClass *>(callbackParam->paramsType[paramIndex].typeItem.get());
		IMetaClass * protoClass = static_cast<IMetaClass *>(protoType.get());

		if(paramClass->equals(protoClass)) {
			return ParamMatchRank_Equal;
		}
		else {
			if(paramClass->isInheritedFrom(protoClass)) {
				return ParamMatchRank_Convert;
			}
		}
	}

	return ParamMatchRank_Unknown;
}

int rankCallable(IMetaService * service, IMetaCallable * callable, const InvokeCallableParam * callbackParam, const InvokeParamRank * paramsRank)
{
	if(!! callable->isVariadic()) {
		return 0;
	}

	if(callable->getParamCount() < callbackParam->paramCount) {
		return -1;
	}

	if(callable->getParamCount() - callable->getDefaultParamCount() > callbackParam->paramCount) {
		return -1;
	}

	int rank = 1;
	
	for(size_t i = 0; i < callbackParam->paramCount; ++i) {
		int paramRank = rankCallableParam(service, callable, callbackParam, i);
		rank += paramRank;
		paramsRank->ranks[i] = paramRank;

		if(! isParamImplicitConvert(paramRank)) {
			bool ok = !! callable->checkParam(&callbackParam->paramsData[i].data, static_cast<uint32_t>(i));
			metaCheckError(callable);
			if(! ok) {
				return -1;
			}
		}
	}

	return rank;
}

bool allowInvokeCallable(const GScriptConfig & config, const GObjectData * userData, IMetaCallable * method)
{
	if(userData != NULL && userData->isInstance()) {
		if(! config.allowAccessStaticMethodViaInstance()) {
			if(method->isStatic()) {
				return false;
			}
		}
	}
	else {
		if(! method->isStatic()) {
			return false;
		}
	}
	
	if(userData != NULL) {
		const GMetaType & methodType = metaGetItemType(method);
		switch(userData->getCV()) {
			case opcvConst:
				return methodType.isConstFunction();
				
			case opcvVolatile:
				return methodType.isVolatileFunction();
				
			case opcvConstVolatile:
				return methodType.isConstVolatileFunction();
				
			default:
				break;
		}
	}
	
	return true;
}

bool allowAccessData(const GScriptConfig & config, const GObjectData * userData, IMetaAccessible * accessible)
{
	if(userData->isInstance()) {
		if(! config.allowAccessStaticDataViaInstance()) {
			if(accessible->isStatic()) {
				return false;
			}
		}
	}
	else {
		if(! accessible->isStatic()) {
			return false;
		}
	}
	
	return true;
}

void convertParam(GVariant * v, int paramRank, GVariant * holder)
{
	switch(paramRank) {
		case ParamMatchRank_Implicit_StringToWideString: {
			*holder = *v;
			*v = GVariant();
			const char * s = fromVariant<char *>(*holder);
			GScopedArray<wchar_t> ws(stringToWideString(s));
			*v = createWideStringVariant(ws.get());
		}
			break;

		case ParamMatchRank_Implicit_WideStringToString: {
			*holder = *v;
			*v = GVariant();
			const wchar_t * ws = fromVariant<wchar_t *>(*holder);
			GScopedArray<char> s(wideStringToString(ws));
			*v = createStringVariant(s.get());
		}
			break;
	}
}

void doLoadMethodList(const GScriptConfig & config, GMetaClassTraveller * traveller,
	IMetaList * methodList, GMetaMap * metaMap, GMetaMapItem * mapItem,
	void * instance, const GObjectData * userData, const char * methodName, bool allowAny)
{
	while(mapItem != NULL) {
		if(mapItem->getType() == mmitMethod) {
			GScopedInterface<IMetaMethod> method(gdynamic_cast<IMetaMethod *>(mapItem->getItem()));
			if(allowAny || allowInvokeCallable(config, userData, method.get())) {
				methodList->add(method.get(), instance);
			}
		}
		else {
			if(mapItem->getType() == mmitMethodList) {
				GScopedInterface<IMetaList> newMethodList(gdynamic_cast<IMetaList *>(mapItem->getItem()));
				for(uint32_t i = 0; i < newMethodList->getCount(); ++i) {
					GScopedInterface<IMetaItem> item(newMethodList->getAt(i));
					if(allowAny || allowInvokeCallable(config, userData, gdynamic_cast<IMetaMethod *>(item.get()))) {
						methodList->add(item.get(), instance);
					}
				}
			}
		}
		
		GScopedInterface<IMetaClass> metaClass(traveller->next(&instance));
		if(!metaClass) {
			break;
		}
		mapItem = findMetaMapItem(metaMap, metaClass.get(), methodName);
	}
}

void loadMethodList(GMetaClassTraveller * traveller,
	IMetaList * methodList, GMetaMap * metaMap, GMetaMapItem * mapItem,
	void * instance, const char * methodName)
{
	doLoadMethodList(GScriptConfig(), traveller, methodList, metaMap, mapItem, instance, NULL, methodName, true);
}

void callbackLoadMethodList(const GScriptConfig & config, IMetaList * methodList, GMetaMap * metaMap, IMetaClass * objectMetaClass,
	void * objectInstance, const GObjectData * userData, const char * methodName)
{
	GMetaClassTraveller traveller(objectMetaClass, objectInstance);
	void * instance = NULL;

	for(;;) {
		GScopedInterface<IMetaClass> metaClass(traveller.next(&instance));
		if(!metaClass) {
			break;
		}

		GMetaMapClass * mapClass = metaMap->findClassMap(metaClass.get());
		if(mapClass == NULL) {
			continue;
		}
		GMetaMapItem * mapItem = mapClass->findItem(methodName);
		if(mapItem == NULL) {
			continue;
		}

		switch(mapItem->getType()) {
			case mmitField:
			case mmitProperty:
			   return;

			case mmitMethod:
			case mmitMethodList: {
				doLoadMethodList(config, &traveller, methodList, metaMap, mapItem, instance, userData, methodName, false);
				return;
			}

			case mmitEnum:
			case mmitEnumValue:
			case mmitClass:
				return;

			default:
				break;
		}
	}
}

void doInvokeCallable(void * instance, IMetaCallable * callable, InvokeCallableParam * callableParam, InvokeCallableResult * result)
{
	result->resultCount = callable->hasResult() ? 1 : 0;

	GVariant holders[REF_MAX_ARITY];

	for(size_t i = 0; i < callableParam->paramCount; ++i) {
		if(isParamImplicitConvert(callableParam->paramsRank.ranks[i])) {
			convertParam(&callableParam->paramsData[i], callableParam->paramsRank.ranks[i], &holders[i]);
		}
	}

	GVariantData * data[REF_MAX_ARITY];
	for(size_t i = 0; i < callableParam->paramCount; ++i) {
		data[i] = & callableParam->paramsData[i].data;
	}
	callable->executeIndirectly(&result->resultData.data, instance, data, static_cast<uint32_t>(callableParam->paramCount));
	metaCheckError(callable);
}


void * doInvokeConstructor(IMetaService * service, IMetaClass * metaClass, InvokeCallableParam * callableParam)
{
	void * instance = NULL;

	if(callableParam->paramCount == 0 && metaClass->canCreateInstance()) {
		instance = metaClass->createInstance();
	}
	else {
		int maxRankIndex = findAppropriateCallable(service,
			makeCallback(metaClass, &IMetaClass::getConstructorAt), metaClass->getConstructorCount(),
			callableParam, FindCallablePredict());

		if(maxRankIndex >= 0) {
			InvokeCallableResult result;
		
			GScopedInterface<IMetaConstructor> constructor(metaClass->getConstructorAt(static_cast<uint32_t>(maxRankIndex)));
			doInvokeCallable(NULL, constructor.get(), callableParam, &result);
			instance = fromVariant<void *>(GVariant(result.resultData));
		}
	}

	return instance;
}

InvokeCallableResult doInvokeMethodList(const GBindingParamPointer & param, const GObjectUserData * objectUserData, const GMethodData & methodData, InvokeCallableParam * callableParam)
{
	void * instance = NULL;
	if(objectUserData != NULL && ! objectUserData->getObjectData()) {
		objectUserData = NULL;
	}
	if(objectUserData != NULL) {
		instance = objectUserData->getObjectData()->getInstance();
	}

	GScopedInterface<IMetaList> methodList;
	if(! methodData.getMetaClass() && methodData.getMethodList()) {
		methodList.reset(methodData.getMethodList());
		methodList->addReference();
	}
	else {
		methodList.reset(createMetaList());
		callbackLoadMethodList(param->getConfig(), methodList.get(), param->getMetaMap(),
			objectUserData == NULL? methodData.getMetaClass() : objectUserData->getObjectData()->getMetaClass(),
			instance,  getObjectData(objectUserData), methodData.getName().c_str());
	}

	int maxRankIndex = findAppropriateCallable(param->getService(),
		makeCallback(methodList.get(), &IMetaList::getAt), methodList->getCount(),
		callableParam, FindCallablePredict());

	if(maxRankIndex >= 0) {
		InvokeCallableResult result;
		GScopedInterface<IMetaCallable> callable(gdynamic_cast<IMetaCallable *>(methodList->getAt(maxRankIndex)));
		doInvokeCallable(methodList->getInstanceAt(static_cast<uint32_t>(maxRankIndex)), callable.get(), callableParam, &result);
		result.callable.reset(callable.get());
		return result;
	}

	raiseCoreException(Error_ScriptBinding_CantFindMatchedMethod);

	return InvokeCallableResult();
}

InvokeCallableResult doInvokeOperator(const GBindingParamPointer & param, void * instance, IMetaClass * metaClass, GMetaOpType op, InvokeCallableParam * callableParam)
{
	int maxRankIndex = findAppropriateCallable(param->getService(),
		makeCallback(metaClass, &IMetaClass::getOperatorAt), metaClass->getOperatorCount(),
		callableParam, OperatorCallablePredict(op));

	if(maxRankIndex >= 0) {
		InvokeCallableResult result;

		GScopedInterface<IMetaOperator> metaOperator(metaClass->getOperatorAt(maxRankIndex));
		doInvokeCallable(instance, metaOperator.get(), callableParam, &result);
		result.callable.reset(metaOperator.get());
		return result;
	}

	return InvokeCallableResult();
}

GScriptDataType methodTypeToUserDataType(GUserDataMethodType methodType)
{
	switch(methodType) {
		case udmtMethod:
			return sdtMethod;

		case udmtMethodList:
			return sdtMethodList;

		case udmtInternal:
			return sdtScriptMethod;

		default:
			return sdtUnknown;
	}
}

GMetaVariant userDataToVariant(const GScriptUserData * userData)
{
	switch(userData->getType()) {
		case udtObject: {
			const GObjectUserData * classData = static_cast<const GObjectUserData *>(userData);
			GMetaTypeData typeData;
			classData->getObjectData()->getMetaClass()->getMetaType(&typeData);
			metaCheckError(classData->getObjectData()->getMetaClass());
			GMetaType type(typeData);
			type.addPointer();
			switch(classData->getObjectData()->getDataType()) {
				case cudtClass:
					return GMetaVariant(classData->getObjectData()->getMetaClass(), type);

				case cudtObject:
					return GMetaVariant(pointerToObjectVariant(classData->getObjectData()->getInstance()), type);
				
				case cudtInterface:
					return GMetaVariant(GVariant(classData->getObjectData()->getInterfaceObject()), type);
			}

			break;
		}

		case udtRaw: {
			const GRawUserData * rawData = static_cast<const GRawUserData *>(userData);
			return GMetaVariant(rawData->getData(), GMetaType());
		}

		default:
			break;
	}

	return GMetaVariant();
}

GVariant getAccessibleValueAndType(void * instance, IMetaAccessible * accessible, GMetaType * outType, bool instanceIsConst)
{
	GVariant value;
	GMetaTypeData typeData;

	accessible->getItemType(&typeData);
	metaCheckError(accessible);

	*outType = GMetaType(typeData);

	void * address = accessible->getAddress(instance);
	if(address != NULL && !outType->isPointer() && outType->baseIsClass()) {
		value = address;

		if(instanceIsConst) {
			outType->addPointerToConst();
		}
		else {
			outType->addPointer();
		}
	}
	else {
		accessible->get(&value.data, instance);
		metaCheckError(accessible);
	}

	return value;
}

bool shouldRemoveReference(const GMetaType & type)
{
	return type.isReference()
		&& (type.isPointer() || vtIsFundamental(vtGetBaseType(type.getVariantType())))
		;
}

wchar_t * stringToWideString(const char * s)
{
	size_t len = strlen(s);
	GScopedArray<wchar_t> ws(new wchar_t[len + 1]);
	std::fill(ws.get(), ws.get() + len + 1, 0);
	mbstowcs(ws.get(), s, len);

	return ws.take();
}

char * wideStringToString(const wchar_t * ws)
{
	size_t len = wcslen(ws);
	GScopedArray<char> s(new char[len + 1]);
	std::fill(s.get(), s.get() + len + 1, 0);
	wcstombs(s.get(), ws, len);

	return s.take();
}


GSharedObjectData getSharedObjectData(const GObjectUserData * objectUserData)
{
	if(objectUserData != NULL) {
		return objectUserData->getObjectData();
	}
	else {
		return GSharedObjectData();
	}
}

GObjectData * getObjectData(const GObjectUserData * objectUserData)
{
	if(objectUserData != NULL) {
		return objectUserData->getObjectData().get();
	}
	else {
		return NULL;
	}
}

IMetaClass * selectBoundClass(IMetaClass * currentClass, IMetaClass * derived)
{
	if(derived == NULL) {
		currentClass->addReference();
		return currentClass;
	}
	else {
		if(derived->getBaseCount() > 0 && derived->getBaseClass(0)) {
			// always choose first base because we only support single inheritance in script binding
			return derived->getBaseClass(0);
		}
		else {
			derived->addReference();
			return derived;
		}
	}
}

bool doSetFieldValue(GObjectUserData * userData, const char * name, const GVariant & value)
{
	if(userData->getObjectData()->getCV() == opcvConst) {
		raiseCoreException(Error_ScriptBinding_CantWriteToConstObject);

		return false;
	}

	GMetaClassTraveller traveller(userData->getObjectData()->getMetaClass(), userData->getObjectData()->getInstance());
	
	void * instance = NULL;

	for(;;) {
		GScopedInterface<IMetaClass> metaClass(traveller.next(&instance));
		if(!metaClass) {
			return false;
		}
		
		GMetaMapItem * mapItem = findMetaMapItem(userData->getParam()->getMetaMap(), metaClass.get(), name);
		if(mapItem == NULL) {
			continue;
		}

		switch(mapItem->getType()) {
			case mmitField:
			case mmitProperty: {
				GScopedInterface<IMetaAccessible> data(gdynamic_cast<IMetaAccessible *>(mapItem->getItem()));
				if(allowAccessData(userData->getParam()->getConfig(), getObjectData(userData), data.get())) {
					metaSetValue(data.get(), userData->getObjectData()->getInstance(), value);
					return true;
				}
			}
			   break;

			case mmitMethod:
			case mmitMethodList:
				userData->getObjectData()->setScriptValue(name, value);
				return true;

			case mmitEnum:
			case mmitEnumValue:
			case mmitClass:
				raiseCoreException(Error_ScriptBinding_CantAssignToEnumMethodClass);
				return false;

			default:
				break;
		}
	}
}

void instanceCreated(GObjectUserData * instanceUserData, GObjectUserData * classUserData)
{
	if(instanceUserData != NULL && classUserData != NULL) {
//		instanceUserData->getObjectData()->mergeDataStorage(classUserData->getObjectData().get());
		instanceUserData->getObjectData()->getDataStorage()->setClassDataStorage(classUserData->getObjectData()->getDataStorage());
	}
}


} // namespace bind_internal

} // namespace cpgf



#if defined(_MSC_VER)
#pragma warning(pop)
#endif

