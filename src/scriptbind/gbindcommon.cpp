#include "../pinclude/gbindcommon.h"

#include "cpgf/gstringmap.h"
#include "cpgf/gcallback.h"
#include "cpgf/gmetaclasstraveller.h"
#include "cpgf/scriptbind/gscriptservice.h"
#include "cpgf/glifecycle.h"

#include <vector>


using namespace std;


namespace cpgf {

G_GUARD_LIBRARY_LIFE

GScriptCoreService * doCreateScriptCoreService(GScriptObject * scriptObject);
GMetaClass * doBindScriptCoreService(GScriptObject * scriptObject, const char * bindName, GScriptCoreService * scriptCoreService);

namespace bind_internal {


//*********************************************
// Declarations
//*********************************************

const int ValueMatchRank_Unknown = 0; // such as 2 or more dimensions pointer
const int ValueMatchRank_Convert = 50000;
const int ValueMatchRank_Implicit_Begin = 70000;
const int ValueMatchRank_Implicit_WideStringToString = ValueMatchRank_Implicit_Begin + 0;
const int ValueMatchRank_Implicit_StringToWideString = ValueMatchRank_Implicit_Begin + 1;
const int ValueMatchRank_Implicit_SharedPointerToRaw = ValueMatchRank_Implicit_Begin + 2;
const int ValueMatchRank_Implicit_End = 80000;
const int ValueMatchRank_Equal = 100000;


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
	
	typedef vector<GClassGlueDataPointer> ClassMapListType;
	typedef GStringMap<ClassMapListType, GStringMapReuseKey> ClassMapType;

public:
	explicit GClassPool(GBindingContext * context);

	void objectCreated(const GObjectGlueDataPointer & objectData);
	void objectDestroyed(void * instance);
	void classDestroyed(IMetaClass * metaClass);

	GClassGlueDataPointer getOrNewClassData(void * instance, IMetaClass * metaClass);
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
	: super(gdtClass, context), metaClass(metaClass), mapClass(metaClass)
{
}

GClassGlueData::~GClassGlueData()
{
	if(this->isValid()) {
		this->getContext()->getClassPool()->classDestroyed(this->getMetaClass());
	}
}


GObjectGlueData::GObjectGlueData(const GContextPointer & context, const GClassGlueDataPointer & classGlueData, const GVariant & instance,
	const GBindValueFlags & flags, ObjectPointerCV cv)
	: super(gdtObject, context), classGlueData(classGlueData), instance(instance), flags(flags), cv(cv)
{
	if(vtIsInterface(instance.getType())) {
		this->flags.clear(bvfAllowGC);
	}
}

GObjectGlueData::~GObjectGlueData()
{
	if(this->isValid()) {
		this->getContext()->getClassPool()->objectDestroyed(this->getInstanceAddress());
	}
	
	if(this->isAllowGC()) {
		this->getClassData()->getMetaClass()->destroyInstance(this->getInstanceAddress());
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
		scriptWrapper->setScriptDataStorage(this->getInstanceAddress(), this->dataStorage.get());
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
	if(this->instanceMap.find(objectData->getInstanceAddress()) == instanceMap.end()) {
		this->instanceMap[objectData->getInstanceAddress()] = GWeakObjectGlueDataPointer(objectData);
	}
}

void GClassPool::objectDestroyed(void * instance)
{
	if(isLibraryLive()) {
		this->instanceMap.erase(instance);
	}
}

void GClassPool::classDestroyed(IMetaClass * metaClass)
{
	if(isLibraryLive()) {
		ClassMapListType * classDataList = this->getList(metaClass);
		if(classDataList != NULL) {
			for(ClassMapListType::iterator it = classDataList->begin(); it != classDataList->end(); ++it) {
				if((*it)->getMetaClass() == metaClass) {
					(*it).reset();
					break;
				}
			}
		}
	}
}

GClassPool::ClassMapListType * GClassPool::getList(IMetaClass * metaClass)
{
	ClassMapType::iterator it = this->classMap.find(metaClass->getQualifiedName());
	if(it == this->classMap.end()) {
		this->classMap.set(metaClass->getQualifiedName(), ClassMapListType());
		it = this->classMap.find(metaClass->getQualifiedName());
	}
	return &it->second;
}

GClassGlueDataPointer * GClassPool::findClassDataByPointer(ClassMapListType * classDataList, IMetaClass * metaClass)
{
	for(ClassMapListType::iterator it = classDataList->begin(); it != classDataList->end(); ++it) {
		if(*it && (*it)->getMetaClass() == metaClass) {
			return &(*it);
		}
	}

	return NULL;
}

GClassGlueDataPointer GClassPool::createClassDataAtSlot(ClassMapListType * classDataList, IMetaClass * metaClass, size_t slot)
{
	while(slot >= classDataList->size()) {
		classDataList->push_back(GClassGlueDataPointer());
	}
	if(! (*classDataList)[slot]) {
		(*classDataList)[slot] = this->context->createClassGlueData(metaClass);
	}
	return (*classDataList)[slot];
}

size_t GClassPool::getFreeSlot(ClassMapListType * classDataList, size_t startSlot)
{
	size_t slot = startSlot;
	for(; slot < classDataList->size(); ++slot) {
		if(! (*classDataList)[slot]) {
			return slot;
		}
	}
	return slot;
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

	return this->getClassData(metaClass);
}

GClassGlueDataPointer GClassPool::getClassData(IMetaClass * metaClass)
{
	ClassMapListType * classDataList = this->getList(metaClass);

	GClassGlueDataPointer * dataPointer = this->findClassDataByPointer(classDataList, metaClass);
	if(dataPointer != NULL) {
		return *dataPointer;
	}
	
	return this->createClassDataAtSlot(classDataList, metaClass, 0);
}

GClassGlueDataPointer GClassPool::newClassData(IMetaClass * metaClass)
{
	ClassMapListType * classDataList = this->getList(metaClass);
	return this->createClassDataAtSlot(classDataList, metaClass, this->getFreeSlot(classDataList, 1));
}

GGlueDataWrapperPool::GGlueDataWrapperPool()
	: active(true)
{
}

typedef vector<GGlueDataWrapper *> TempListType;
GGlueDataWrapperPool::~GGlueDataWrapperPool()
{
	this->active = false;
	
	TempListType tempList(this->wrapperSet.begin(), this->wrapperSet.end());
	this->wrapperSet.clear();

	for(TempListType::iterator it = tempList.begin(); it != tempList.end(); ++it) {
		if((*it)->getData() && (*it)->getData()->getType() != gdtClass) {
			freeGlueDataWrapper(*it);
			*it = NULL;
		}
	}

	/* We must free class data after other data (especially, after object data).
	   This is because when class data is freed, the meta class maybe freed which object is still using.
	   Though object holds a shared pointer of class data, the class data may free its nested class which the object is using.
	   That will cause memory access error.
	*/
	for(TempListType::iterator it = tempList.begin(); it != tempList.end(); ++it) {
		if(*it != NULL) {
			freeGlueDataWrapper(*it);
		}
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

GClassGlueDataPointer GBindingContext::createClassGlueData(IMetaClass * metaClass)
{
	GClassGlueDataPointer data(new GClassGlueData(this->shareFromThis(), metaClass));
	return data;
}

GClassGlueDataPointer GBindingContext::getOrNewClassData(void * instance, IMetaClass * metaClass)
{
	return this->classPool->getOrNewClassData(instance, metaClass);
}

GClassGlueDataPointer GBindingContext::getClassData(IMetaClass * metaClass)
{
	return this->classPool->getClassData(metaClass);
}

GClassGlueDataPointer GBindingContext::newClassData(IMetaClass * metaClass)
{
	return this->classPool->newClassData(metaClass);
}

GObjectGlueDataPointer GBindingContext::newObjectGlueData(const GClassGlueDataPointer & classData, const GVariant & instance,
	const GBindValueFlags & flags, ObjectPointerCV cv)
{
	GObjectGlueDataPointer data(new GObjectGlueData(this->shareFromThis(), classData, instance, flags, cv));
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

GObjectAndMethodGlueDataPointer GBindingContext::newObjectAndMethodGlueData(const GObjectGlueDataPointer & objectData, const GMethodGlueDataPointer & methodData)
{
	GObjectAndMethodGlueDataPointer objectAndMethodData(new GObjectAndMethodGlueData(this->shareFromThis(), objectData, methodData));
	return objectAndMethodData;
}

GOperatorGlueDataPointer GBindingContext::newOperatorGlueData(void * instance, IMetaClass * metaClass, GMetaOpType op)
{
	GOperatorGlueDataPointer operatorData(new GOperatorGlueData(this->shareFromThis(), instance, metaClass, op));
	return operatorData;
}


InvokeParamRank::InvokeParamRank(size_t paramCount) : paramCount(paramCount)
{
	for(size_t i = 0; i < this->paramCount; ++i) {
		this->ranks[i] = ValueMatchRank_Unknown;
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


IMetaClass * GScriptObjectBase::cloneMetaClass(IMetaClass * metaClass)
{
	IMetaClass * newMetaClass = gdynamic_cast<IMetaClass *>(metaClass->clone());

	this->context->getClassData(metaClass);
	this->context->newClassData(newMetaClass);

	return newMetaClass;
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
	return paramRank >= ValueMatchRank_Implicit_Begin && paramRank < ValueMatchRank_Implicit_End;
}

int rankImplicitConvertForString(const GVariant & sourceData, const GMetaType & targetType)
{
	if(variantIsString(sourceData) && targetType.isWideString()) {
		return ValueMatchRank_Implicit_StringToWideString;
	}

	if(variantIsWideString(sourceData) && targetType.isString()) {
		return ValueMatchRank_Implicit_WideStringToString;
	}

	return ValueMatchRank_Unknown;
}

int rankImplicitConvertForSharedPointer(const GGlueDataPointer & valueGlueData, const GMetaExtendType & targetExtendType)
{
	IMetaSharedPointerTraits * paramSharedPointerTraits = getGlueDataSharedPointerTraits(valueGlueData);
	if(paramSharedPointerTraits != NULL) {
		GScopedInterface<IMetaSharedPointerTraits> sharedPointerTraits(targetExtendType.getSharedPointerTraits());
		if(! sharedPointerTraits) {
			return ValueMatchRank_Implicit_SharedPointerToRaw;
		}
	}

	return ValueMatchRank_Unknown;
}

int rankCallableImplicitConvert(IMetaCallable * callable, const InvokeCallableParam * callbackParam, size_t paramIndex, const GMetaType & targetType)
{
	int rank = rankImplicitConvertForString(callbackParam->params[paramIndex].value, targetType);
	if(rank == ValueMatchRank_Unknown) {
		rank = rankImplicitConvertForSharedPointer(callbackParam->params[paramIndex].glueData,
			metaGetParamExtendType(callable, GExtendTypeCreateFlag_SharedPointerTraits, static_cast<uint32_t>(paramIndex)));
	}
	
	return rank;
}

int rankCallableParam(IMetaService * service, IMetaCallable * callable, const InvokeCallableParam * callbackParam, size_t paramIndex)
{
	GMetaType proto = metaGetParamType(callable, paramIndex);
	GScriptDataType sdt = callbackParam->params[paramIndex].dataType;
	
	if(sdt == sdtNull) {
		return ValueMatchRank_Equal;
	}
	
	if(proto.isFundamental() && sdt == sdtFundamental) {
		return ValueMatchRank_Equal;
	}

	if(sdt == sdtScriptMethod && vtIsInterface(proto.getVariantType())) {
		return ValueMatchRank_Convert;
	}

	if(proto.getPointerDimension() > 1) {
		return ValueMatchRank_Unknown;
	}

	int implicitRank = rankCallableImplicitConvert(callable, callbackParam, paramIndex, proto);
	if(implicitRank != ValueMatchRank_Unknown) {
		return implicitRank;
	}

	// check for meta class

	if(! callbackParam->params[paramIndex].typeItem) {
		return ValueMatchRank_Unknown;
	}

	if(metaIsClass(callbackParam->params[paramIndex].typeItem->getCategory())) {
		GScopedInterface<IMetaTypedItem> protoType(service->findTypedItemByName(proto.getBaseName()));
		if(! protoType || ! metaIsClass(protoType->getCategory())) {
			return ValueMatchRank_Unknown;
		}

		IMetaClass * paramClass = static_cast<IMetaClass *>(callbackParam->params[paramIndex].typeItem.get());
		IMetaClass * protoClass = static_cast<IMetaClass *>(protoType.get());

		if(paramClass->equals(protoClass)) {
			return ValueMatchRank_Equal;
		}
		else {
			if(paramClass->isInheritedFrom(protoClass)) {
				return ValueMatchRank_Convert;
			}
		}
	}

	return ValueMatchRank_Unknown;
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
			bool ok = !! callable->checkParam(&callbackParam->params[i].value.refData(), static_cast<uint32_t>(i));
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

bool implicitConvertForString(int rank, GVariant * v, GVariant * holder)
{
	switch(rank) {
		case ValueMatchRank_Implicit_StringToWideString: {
			*holder = *v;
			*v = GVariant();
			const char * s = fromVariant<char *>(*holder);
			GScopedArray<wchar_t> ws(stringToWideString(s));
			*v = createWideStringVariant(ws.get());

			return true;
		}

		case ValueMatchRank_Implicit_WideStringToString: {
			*holder = *v;
			*v = GVariant();
			const wchar_t * ws = fromVariant<wchar_t *>(*holder);
			GScopedArray<char> s(wideStringToString(ws));
			*v = createStringVariant(s.get());

			return true;
		}
	}

	return false;
}

bool implicitConvertForSharedPointer(int rank, GVariant * v, const GGlueDataPointer & valueGlueData)
{
	switch(rank) {
		case ValueMatchRank_Implicit_SharedPointerToRaw: {
			IMetaSharedPointerTraits * paramSharedPointerTraits = getGlueDataSharedPointerTraits(valueGlueData);
			*v = paramSharedPointerTraits->getPointer(objectAddressFromVariant(*v));

			return true;
		}
	}

	return false;
}

void implicitConvertCallableParam(GVariant * v, int paramRank, GVariant * holder, InvokeCallableParam * callableParam, size_t paramIndex)
{
	if(! implicitConvertForString(paramRank, v, holder)) {
		implicitConvertForSharedPointer(paramRank, v, callableParam->params[paramIndex].glueData);
	}
}

void doInvokeCallable(void * instance, IMetaCallable * callable, InvokeCallableParam * callableParam, InvokeCallableResult * result)
{
	result->resultCount = callable->hasResult() ? 1 : 0;

	GVariant holders[REF_MAX_ARITY];

	for(size_t i = 0; i < callableParam->paramCount; ++i) {
		if(isParamImplicitConvert(callableParam->paramsRank.ranks[i])) {
			implicitConvertCallableParam(&callableParam->params[i].value, callableParam->paramsRank.ranks[i], &holders[i], callableParam, i);
		}
	}

	const GVariantData * data[REF_MAX_ARITY];
	for(size_t i = 0; i < callableParam->paramCount; ++i) {
		data[i] = & callableParam->params[i].value.refData();
	}
	callable->executeIndirectly(&result->resultData.refData(), instance, data, static_cast<uint32_t>(callableParam->paramCount));
	metaCheckError(callable);

	for(uint32_t i = 0; i < callableParam->paramCount; ++i) {
		if(callable->isParamTransferOwnership(i) && callableParam->params[i].glueData && callableParam->params[i].glueData->getType() == gdtObject) {
			static_cast<GObjectGlueData *>(callableParam->params[i].glueData.get())->setAllowGC(false);
		}
	}
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
			instance = objectAddressFromVariant(GVariant(result.resultData));
		}
	}

	return instance;
}

InvokeCallableResult doInvokeMethodList(const GContextPointer & context,
										const GObjectGlueDataPointer & objectData,
										const GMethodGlueDataPointer & methodData, InvokeCallableParam * callableParam)
{
	GScopedInterface<IMetaList> methodList;
	if((! methodData->getClassData() || ! methodData->getClassData()->getMetaClass()) && methodData->getMethodList()->getCount() > 0) {
		methodList.reset(methodData->getMethodList());
		methodList->addReference();
	}
	else {
		// Reloading the method list because the "this" pointer may change and cause the method list invalid.
		// We should find better way to handle this.
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

GVariant glueDataToVariant(const GGlueDataPointer & glueData)
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
				return createTypedVariant(classData->getMetaClass(), type);
			}

			case gdtObject: {
				GObjectGlueDataPointer objectData = sharedStaticCast<GObjectGlueData>(glueData);
				GMetaTypeData typeData;
				objectData->getClassData()->getMetaClass()->getMetaType(&typeData);
				metaCheckError(objectData->getClassData()->getMetaClass());
				GMetaType type(typeData);
				type.addPointer();
				return createTypedVariant(objectData->getInstance(), type);
			}

			case gdtRaw: {
				GRawGlueDataPointer rawData = sharedStaticCast<GRawGlueData>(glueData);
				return createTypedVariant(rawData->getData(), GMetaType());
			}

			default:
				break;
		}
	}

	return GVariant();
}

GVariant getAccessibleValueAndType(void * instance, IMetaAccessible * accessible, GMetaType * outType, bool instanceIsConst)
{
	GVariant value;

	accessible->getItemType(&outType->refData());
	metaCheckError(accessible);

	void * address = accessible->getAddress(instance);
	if(address != NULL && !outType->isPointer() && outType->baseIsClass()) {
		value = objectToVariant(address);

		if(instanceIsConst) {
			outType->addConst();
		}
	}
	else {
		accessible->get(&value.refData(), instance);
		metaCheckError(accessible);
	}

	return value;
}

void doLoadMethodList(const GContextPointer & context, GMetaClassTraveller * traveller,
	IMetaList * methodList, GMetaMapItem * mapItem,
	void * instance,
	const GClassGlueDataPointer & classData, const GGlueDataPointer & glueData, const char * methodName, bool allowAny)
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
		mapItem = classData->getClassMap()->findItem(methodName);
	}
}

void loadMethodList(const GContextPointer & context, IMetaList * methodList, const GClassGlueDataPointer & classData,
			const GObjectGlueDataPointer & objectData, const char * methodName)
{
	GMetaClassTraveller traveller(classData->getMetaClass(), objectData? objectData->getInstanceAddress() : NULL);
	void * instance = NULL;

	for(;;) {
		GScopedInterface<IMetaClass> metaClass(traveller.next(&instance));
		if(!metaClass) {
			break;
		}

		GMetaMapClass * mapClass = context->getClassData(metaClass.get())->getClassMap();
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
				doLoadMethodList(context, &traveller, methodList, mapItem, instance, classData, objectData, methodName, false);
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

void doSetValueOnAccessible(IMetaAccessible * accessible, const GGlueDataPointer & instanceGlueData, GVariant value, const GGlueDataPointer & valueGlueData)
{
	int rank = rankImplicitConvertForSharedPointer(valueGlueData, metaGetItemExtendType(accessible, GExtendTypeCreateFlag_SharedPointerTraits));
	implicitConvertForSharedPointer(rank, &value, valueGlueData);

	metaSetValue(accessible, getGlueDataInstance(instanceGlueData), value);
}

bool setValueOnNamedMember(const GGlueDataPointer & instanceGlueData, const char * name, const GVariant & value, const GGlueDataPointer & valueGlueData)
{
	if(getGlueDataCV(instanceGlueData) == opcvConst) {
		raiseCoreException(Error_ScriptBinding_CantWriteToConstObject);

		return false;
	}

	bool isInstance = (instanceGlueData->getType() == gdtObject);
	GClassGlueDataPointer classData;
	GObjectGlueDataPointer objectData;
	if(instanceGlueData->getType() == gdtObject) {
		objectData = sharedStaticCast<GObjectGlueData>(instanceGlueData);
		classData = objectData->getClassData();
	}
	else {
		GASSERT(instanceGlueData->getType() == gdtClass);
		classData = sharedStaticCast<GClassGlueData>(instanceGlueData);
	}

	const GScriptConfig & config = classData->getContext()->getConfig();
	GContextPointer context = classData->getContext();

	GMetaClassTraveller traveller(classData->getMetaClass(), getGlueDataInstance(instanceGlueData));

	void * instance = NULL;

	for(;;) {
		GScopedInterface<IMetaClass> metaClass(traveller.next(&instance));
		if(!metaClass) {
			return false;
		}
		
		GMetaMapClass * mapClass = classData->getClassMap();
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
					doSetValueOnAccessible(data.get(), instanceGlueData, value, valueGlueData);
					return true;
				}
			}
			   break;

			case mmitMethod:
			case mmitMethodList:
				setValueToScriptDataHolder(instanceGlueData, name, value);
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
			return sharedStaticCast<GObjectGlueData>(glueData)->getInstanceAddress();
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

IMetaSharedPointerTraits * getGlueDataSharedPointerTraits(const GGlueDataPointer & glueData)
{
	if(glueData) {
		if(glueData->getType() == gdtObject) {
			return sharedStaticCast<GObjectGlueData>(glueData)->getSharedPointerTraits();
		}
		else {
			if(glueData->getType() == gdtRaw) {
				return sharedStaticCast<GRawGlueData>(glueData)->getSharedPointerTraits();
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

InvokeCallableResult doInvokeOperator(const GContextPointer & context, void * instance, IMetaClass * metaClass, GMetaOpType op, InvokeCallableParam * callableParam)
{
	int maxRankIndex = findAppropriateCallable(context->getService(),
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



} // namespace bind_internal


} // namespace cpgf



