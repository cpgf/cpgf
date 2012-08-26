#include "gbindcommon_new.h"

#include "cpgf/gstringmap.h"
#include "cpgf/gcallback.h"
#include "cpgf/gmetaclasstraveller.h"
#include "cpgf/scriptbind/gscriptservice.h"

#include <vector>

using namespace std;


namespace cpgf {

GScriptCoreService * doCreateScriptCoreService(GScriptObject * scriptObject);
GMetaClass * doBindScriptCoreService(GScriptObject * scriptObject, const char * bindName, GScriptCoreService * scriptCoreService);

namespace _bind_internal {


//*********************************************
// Declarations
//*********************************************

// such as 2 or more dimensions pointer
const int ParamMatchRank_Unknown = 0;
const int ParamMatchRank_Convert = 50000;
const int ParamMatchRank_Implicit_Begin = 70000;
const int ParamMatchRank_Implicit_WideStringToString = ParamMatchRank_Implicit_Begin + 0;
const int ParamMatchRank_Implicit_StringToWideString = ParamMatchRank_Implicit_Begin + 1;
const int ParamMatchRank_Implicit_End = 80000;
const int ParamMatchRank_Equal = 100000;


struct FindCallablePredict {
	bool operator () (IMetaCallable *) {
		return true;
	}
};

class GClassItem
{
private:
	typedef vector<GClassGlueDataPointer> ListType;

public:
	explicit GClassItem(const GClassGlueDataPointer & classGlueData)
		: classGlueData(classGlueData)
	{
	}

	const GClassGlueDataPointer & getClassGlueData() const {
		return this->classGlueData;
	}

private:
	GClassGlueDataPointer classGlueData;
};

class GClassPool
{
private:
	typedef map<void *, GWeakObjectGlueDataPointer> InstanceMapType;
	typedef GStringMap<GWeakClassGlueDataPointer, GStringMapReuseKey> ClassNameMapType;
	typedef map<IMetaClass *, GWeakClassGlueDataPointer> ClassPointerMapType;

public:
	explicit GClassPool(GBindingContext * context);

	void objectCreated(const GObjectGlueDataPointer & objectData);
	void objectDestroyed(void * instance);
	void classCreated(const GClassGlueDataPointer & classData);
	void classDestroyed(IMetaClass * metaClass);

	GClassGlueDataPointer findClassDataByPointer(IMetaClass * metaClass);
	GClassGlueDataPointer findClassDataByName(IMetaClass * metaClass);
	GClassGlueDataPointer getOrNewClassData(void * instance, IMetaClass * metaClass);

private:
	InstanceMapType instanceMap;
	ClassNameMapType classNameMap;
	ClassPointerMapType classPointerMap;
	GBindingContext * context;
};


//*********************************************
// Classes implementations
//*********************************************


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

GMetaMapClass * GMetaMap::getClassMap(IMetaClass * metaClass)
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


GScriptDataHolder * GClassGlueData::getDataHolder() const
{
	return this->dataHolder.get();
}

GScriptDataHolder * GClassGlueData::requireDataHolder() const
{
	if(! this->dataHolder) {
		this->dataHolder.reset(new GScriptDataHolder());
	}
	return this->dataHolder.get();
}


GClassGlueData::GClassGlueData(const GContextPointer & context, IMetaClass * metaClass)
	: super(gdtClass, context), metaClass(metaClass)
{
}

GClassGlueData::~GClassGlueData()
{
	if(this->isValid()) {
		this->getContext()->getClassPool()->classDestroyed(this->metaClass.get());
	}
}


GObjectGlueData::GObjectGlueData(const GContextPointer & context, const GClassGlueDataPointer & classGlueData, void * instance,
	bool allowGC, ObjectPointerCV cv, ObjectGlueDataType dataType)
	: super(gdtObject, context), classGlueData(classGlueData), instance(instance), allowGC(allowGC), cv(cv), dataType(dataType)
{
	switch(dataType) {
		case ogdtInterface:
			this->allowGC = false;
			this->interfaceObject.reset(static_cast<IObject *>(instance));
			break;

		default:
			break;
	}
}

GObjectGlueData::~GObjectGlueData()
{
	if(this->isValid()) {
		this->getContext()->getClassPool()->objectDestroyed(this->instance);
	}

	if(this->allowGC) {
		this->getClassData()->getMetaClass()->destroyInstance(this->instance);
	}
}

GScriptDataHolder * GObjectGlueData::getDataHolder() const
{
	return this->dataHolder.get();
}

GScriptDataHolder * GObjectGlueData::requireDataHolder() const
{
	if(! this->dataHolder) {
		this->dataHolder.reset(new GScriptDataHolder());
	}
	return this->dataHolder.get();
}

void GObjectGlueData::initialize()
{
	GScopedInterface<IMetaScriptWrapper> scriptWrapper(metaGetItemExtendType(this->getClassData()->getMetaClass(), GExtendTypeCreateFlag_ScriptWrapper).getScriptWrapper());
	if(scriptWrapper) {
		if(! this->dataStorage) {
			this->dataStorage.reset(new GScriptDataStorage(GObjectGlueDataPointer(this->shareFromThis())));
		}
		scriptWrapper->setScriptDataStorage(this->instance, this->dataStorage.get());
	}
}


GScriptDataStorage::GScriptDataStorage(const GObjectGlueDataPointer & object)
	: object(object)
{
}

IScriptFunction * G_API_CC GScriptDataStorage::getScriptFunction(const char * name)
{
	if(this->object.expired()) {
		return NULL;
	}
	
	GObjectGlueDataPointer obj(this->object);
	IScriptFunction * func = NULL;
	if(obj->getDataHolder() != NULL) {
		func = obj->getDataHolder()->getScriptFunction(name);
	}
	if(func == NULL && obj->getClassData()->getDataHolder() != NULL) {
		func = obj->getClassData()->getDataHolder()->getScriptFunction(name);
	}
	return func;
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

	this->dataMap->insert(MapValueType(name, value));
}

IScriptFunction * GScriptDataHolder::getScriptFunction(const char * name)
{
	if(this->dataMap) {
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


GClassPool::GClassPool(GBindingContext * context)
	: context(context)
{
}

void GClassPool::objectCreated(const GObjectGlueDataPointer & objectData)
{
	if(this->instanceMap.find(objectData->getInstance()) == instanceMap.end()) {
		this->instanceMap[objectData->getInstance()] = GWeakObjectGlueDataPointer(objectData);
	}
}

void GClassPool::objectDestroyed(void * instance)
{
	GASSERT(this->instanceMap.find(instance) != instanceMap.end());

	this->instanceMap.erase(instance);
}

void GClassPool::classCreated(const GClassGlueDataPointer & classData)
{
	if(this->classNameMap.find(classData->getMetaClass()->getQualifiedName()) == this->classNameMap.end()) {
		this->classNameMap.set(classData->getMetaClass()->getQualifiedName(), GWeakClassGlueDataPointer(classData));
	}
	
	if(this->classPointerMap.find(classData->getMetaClass()) == this->classPointerMap.end()) {
		this->classPointerMap[classData->getMetaClass()] = GWeakClassGlueDataPointer(classData);
	}
}

void GClassPool::classDestroyed(IMetaClass * metaClass)
{
	this->classNameMap.remove(metaClass->getQualifiedName());
	this->classPointerMap.erase(metaClass);
}

GClassGlueDataPointer GClassPool::findClassDataByPointer(IMetaClass * metaClass)
{
	ClassPointerMapType::iterator classPointerIterator = this->classPointerMap.find(metaClass);
	if(classPointerIterator != this->classPointerMap.end()) {
		if(classPointerIterator->second.expired()) {
			this->classPointerMap.erase(metaClass);
		}
		else {
			return classPointerIterator->second.get();
		}
	}
	
	return GClassGlueDataPointer();
}

GClassGlueDataPointer GClassPool::findClassDataByName(IMetaClass * metaClass)
{
	ClassNameMapType::iterator classIterator = this->classNameMap.find(metaClass->getQualifiedName());
	if(classIterator != this->classNameMap.end()) {
		if(classIterator->second.expired()) {
			this->classNameMap.remove(metaClass->getQualifiedName());
		}
		else {
			return classIterator->second.get();
		}
	}

	return GClassGlueDataPointer();
}

GClassGlueDataPointer GClassPool::getOrNewClassData(void * instance, IMetaClass * metaClass)
{
	InstanceMapType::iterator instanceIterator = this->instanceMap.find(instance);
	if(instanceIterator != this->instanceMap.end()) {
		if(instanceIterator->second.expired()) {
			this->instanceMap.erase(instanceIterator);
		}
		else {
			if(metaClass->equals(instanceIterator->second.get()->getClassData()->getMetaClass())) {
				return instanceIterator->second.get()->getClassData();
			}
		}
	}
	
	GClassGlueDataPointer classData = this->findClassDataByPointer(metaClass);
	if(! classData) {
		classData = this->context->newClassGlueData(metaClass);
	}
	return classData;
}


GGlueDataWrapperPool::GGlueDataWrapperPool()
	: active(true)
{
}

GGlueDataWrapperPool::~GGlueDataWrapperPool()
{
	this->active = false;
	for(SetType::iterator it = this->wrapperSet.begin(); it != this->wrapperSet.end(); ++it) {
//		freeGlueDataWrapper(*it);
	}
}

void GGlueDataWrapperPool::dataWrapperCreated(GGlueDataWrapper * dataWrapper)
{
	if(this->active) {
		GASSERT(this->wrapperSet.find(dataWrapper) == this->wrapperSet.end());
		this->wrapperSet.insert(dataWrapper);
	}
}

void GGlueDataWrapperPool::dataWrapperDestroyed(GGlueDataWrapper * dataWrapper)
{
	if(this->active) {
		this->wrapperSet.erase(dataWrapper);
	}
}


GBindingContext::GBindingContext(IMetaService * service, const GScriptConfig & config)
	: service(service), config(config)
{
	this->classPool.reset(new GClassPool(this));
}

GBindingContext::~GBindingContext()
{
}

void GBindingContext::bindScriptCoreService(GScriptObject * scriptObject, const char * bindName)
{
	if(this->scriptCoreService) {
		return;
	}

	this->scriptCoreService.reset(doCreateScriptCoreService(scriptObject));
	this->scriptCoreServiceMetaClass.reset(doBindScriptCoreService(scriptObject, bindName, this->scriptCoreService.get()));
}

GClassPool * GBindingContext::getClassPool()
{
	return this->classPool.get();
}

GClassGlueDataPointer GBindingContext::newClassGlueData(IMetaClass * metaClass)
{
	GClassGlueDataPointer data(new GClassGlueData(this->shareFromThis(), metaClass));
	this->classPool->classCreated(data);
	return data;
}

GClassGlueDataPointer GBindingContext::getOrNewClassData(void * instance, IMetaClass * metaClass)
{
	return this->classPool->getOrNewClassData(instance, metaClass);
}

GClassGlueDataPointer GBindingContext::requireClassGlueData(IMetaClass * metaClass)
{
	GClassGlueDataPointer classData = this->classPool->findClassDataByPointer(metaClass);
	if(! classData) {
		classData = this->newClassGlueData(metaClass);
	}

	return classData;
}

GClassGlueDataPointer GBindingContext::requireAnyClassGlueData(IMetaClass * metaClass)
{
	GClassGlueDataPointer classData = this->classPool->findClassDataByName(metaClass);
	if(! classData) {
		classData = this->newClassGlueData(metaClass);
	}

	return classData;
}

GObjectGlueDataPointer GBindingContext::newObjectGlueData(const GClassGlueDataPointer & classData, void * instance,
	bool allowGC, ObjectPointerCV cv, ObjectGlueDataType dataType)
{
	GObjectGlueDataPointer data(new GObjectGlueData(this->shareFromThis(), classData, instance, allowGC, cv, dataType));
	data->initialize();
	this->classPool->objectCreated(data);
	return data;
}

GMethodGlueDataPointer GBindingContext::newMethodGlueData(const GClassGlueDataPointer & classData,
	IMetaList * methodList, const char * name, GGlueDataMethodType methodType)
{
	GMethodGlueDataPointer data(new GMethodGlueData(this->shareFromThis(), classData, methodList, name, methodType));
	return data;
}

GEnumGlueDataPointer GBindingContext::newEnumGlueData(IMetaEnum * metaEnum)
{
	GEnumGlueDataPointer enumData(new GEnumGlueData(this->shareFromThis(), metaEnum));
	return enumData;
}

GAccessibleGlueDataPointer GBindingContext::newAccessibleGlueData(void * instance, IMetaAccessible * accessible)
{
	GAccessibleGlueDataPointer accessibleData(new GAccessibleGlueData(this->shareFromThis(), instance, accessible));
	return accessibleData;
}

GRawGlueDataPointer GBindingContext::newRawGlueData(const GVariant & data)
{
	GRawGlueDataPointer rawData(new GRawGlueData(this->shareFromThis(), data));
	return rawData;
}

GMetaMapClass * GBindingContext::getClassMap(IMetaClass * metaClass)
{
	return this->metaMap.getClassMap(metaClass);
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


//*********************************************
// Global function implementations
//*********************************************

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

bool allowInvokeCallable(const GScriptConfig & config, const GGlueDataPointer & glueData, IMetaCallable * method)
{
	if(getGlueDataInstance(glueData) != NULL) {
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
	
	ObjectPointerCV cv = getGlueDataCV(glueData);
	if(cv != opcvNone) {
		const GMetaType & methodType = metaGetItemType(method);
		switch(cv) {
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

bool allowAccessData(const GScriptConfig & config, bool isInstance, IMetaAccessible * accessible)
{
	if(isInstance) {
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

InvokeCallableResult doInvokeMethodList(const GContextPointer & context,
										const GObjectGlueDataPointer & objectData,
										const GMethodGlueDataPointer & methodData, InvokeCallableParam * callableParam)
{
	void * instance = getGlueDataInstance(objectData);

	GScopedInterface<IMetaList> methodList;
	if((! methodData->getClassData() || ! methodData->getClassData()->getMetaClass()) && methodData->getMethodList()->getCount() > 0) {
		methodList.reset(methodData->getMethodList());
		methodList->addReference();
	}
	else {
		methodList.reset(createMetaList());
		loadMethodList(context, methodList.get(), objectData ? objectData->getClassData() : methodData->getClassData(),
			objectData, methodData->getName().c_str());
	}

	int maxRankIndex = findAppropriateCallable(context->getService(),
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

GMetaVariant glueDataToVariant(const GGlueDataPointer & glueData)
{
	if(glueData) {
		switch(glueData->getType()) {
			case gdtClass: {
				GClassGlueDataPointer classData = sharedStaticCast<GClassGlueData>(glueData);;
				GMetaTypeData typeData;
				classData->getMetaClass()->getMetaType(&typeData);
				metaCheckError(classData->getMetaClass());
				GMetaType type(typeData);
				type.addPointer();
				return GMetaVariant(classData->getMetaClass(), type);
			}

			case gdtObject: {
				GObjectGlueDataPointer objectData = sharedStaticCast<GObjectGlueData>(glueData);
				GMetaTypeData typeData;
				objectData->getClassData()->getMetaClass()->getMetaType(&typeData);
				metaCheckError(objectData->getClassData()->getMetaClass());
				GMetaType type(typeData);
				type.addPointer();
				switch(objectData->getDataType()) {
					case ogdtNormal:
						return GMetaVariant(pointerToObjectVariant(objectData->getInstance()), type);
					
					case ogdtInterface:
						return GMetaVariant(GVariant(objectData->getInterfaceObject()), type);
				}

				break;
			}

			case gdtRaw: {
				GRawGlueDataPointer rawData = sharedStaticCast<GRawGlueData>(glueData);
				return GMetaVariant(rawData->getData(), GMetaType());
			}

			default:
				break;
		}
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

void doLoadMethodList(const GContextPointer & context, GMetaClassTraveller * traveller,
	IMetaList * methodList, GMetaMapItem * mapItem,
	void * instance, const GGlueDataPointer & glueData, const char * methodName, bool allowAny)
{
	while(mapItem != NULL) {
		if(mapItem->getType() == mmitMethod) {
			GScopedInterface<IMetaMethod> method(gdynamic_cast<IMetaMethod *>(mapItem->getItem()));
			if(allowAny || allowInvokeCallable(context->getConfig(), glueData, method.get())) {
				methodList->add(method.get(), instance);
			}
		}
		else {
			if(mapItem->getType() == mmitMethodList) {
				GScopedInterface<IMetaList> newMethodList(gdynamic_cast<IMetaList *>(mapItem->getItem()));
				for(uint32_t i = 0; i < newMethodList->getCount(); ++i) {
					GScopedInterface<IMetaItem> item(newMethodList->getAt(i));
					if(allowAny || allowInvokeCallable(context->getConfig(), glueData, gdynamic_cast<IMetaMethod *>(item.get()))) {
						methodList->add(item.get(), instance);
					}
				}
			}
		}
		
		GScopedInterface<IMetaClass> metaClass(traveller->next(&instance));
		if(!metaClass) {
			break;
		}
		GClassGlueDataPointer classData = context->requireClassGlueData(metaClass.get());
		mapItem = context->getClassMap(classData->getMetaClass())->findItem(methodName);
	}
}

void loadMethodList(const GContextPointer & context, IMetaList * methodList, const GClassGlueDataPointer & classData,
			const GObjectGlueDataPointer & objectData, const char * methodName)
{
	GMetaClassTraveller traveller(classData->getMetaClass(), objectData? objectData->getInstance() : NULL);
	void * instance = NULL;

	for(;;) {
		GScopedInterface<IMetaClass> metaClass(traveller.next(&instance));
		if(!metaClass) {
			break;
		}

		GMetaMapClass * mapClass = context->getClassMap(metaClass.get());
		if(! mapClass) {
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
				doLoadMethodList(context, &traveller, methodList, mapItem, instance, objectData, methodName, false);
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

void setValueToScriptDataHolder(const GGlueDataPointer & glueData, const char * name, const GVariant & value)
{
	GScriptDataHolder * dataHolder = NULL;
	
	if(glueData->getType() == gdtObject) {
		dataHolder = sharedStaticCast<GObjectGlueData>(glueData)->requireDataHolder();
	}
	else {
		if(glueData->getType() == gdtClass) {
			dataHolder = sharedStaticCast<GClassGlueData>(glueData)->requireDataHolder();
		}
	}
	
	if(dataHolder != NULL) {
		dataHolder->setScriptValue(name, value);
	}
}

bool doSetFieldValue(const GGlueDataPointer & glueData, const char * name, const GVariant & value)
{
	if(getGlueDataCV(glueData) == opcvConst) {
		raiseCoreException(Error_ScriptBinding_CantWriteToConstObject);

		return false;
	}

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

	for(;;) {
		GScopedInterface<IMetaClass> metaClass(traveller.next(&instance));
		if(!metaClass) {
			return false;
		}
		
		GMetaMapClass * mapClass = classData->getContext()->getClassMap(classData->getMetaClass());
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
					metaSetValue(data.get(), getGlueDataInstance(glueData), value);
					return true;
				}
			}
			   break;

			case mmitMethod:
			case mmitMethodList:
				setValueToScriptDataHolder(glueData, name, value);
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

ObjectPointerCV getGlueDataCV(const GGlueDataPointer & glueData)
{
	if(glueData) {
		if(glueData->getType() == gdtObject) {
			return sharedStaticCast<GObjectGlueData>(glueData)->getCV();
		}
	}

	return opcvNone;
}

void * getGlueDataInstance(const GGlueDataPointer & glueData)
{
	if(glueData) {
		if(glueData->getType() == gdtObject) {
			return sharedStaticCast<GObjectGlueData>(glueData)->getInstance();
		}
	}

	return NULL;
}

IMetaClass * getGlueDataMetaClass(const GGlueDataPointer & glueData)
{
	if(glueData) {
		if(glueData->getType() == gdtObject) {
			return sharedStaticCast<GObjectGlueData>(glueData)->getClassData()->getMetaClass();
		}
		else {
			if(glueData->getType() == gdtClass) {
				return sharedStaticCast<GClassGlueData>(glueData)->getMetaClass();
			}
		}
	}

	return NULL;
}

GScriptDataType methodTypeToGlueDataType(GGlueDataMethodType methodType)
{
	switch(methodType) {
		case gdmtMethod:
			return sdtMethod;

		case gdmtMethodList:
			return sdtMethodList;

		case gdmtInternal:
			return sdtScriptMethod;

		default:
			return sdtUnknown;
	}
}



} // namespace _bind_internal


} // namespace cpgf



