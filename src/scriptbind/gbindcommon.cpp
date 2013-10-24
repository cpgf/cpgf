#include "../pinclude/gbindcommon.h"

#include "cpgf/gstringmap.h"
#include "cpgf/gcallback.h"
#include "cpgf/gmetaclasstraveller.h"
#include "cpgf/scriptbind/gscriptservice.h"
#include "cpgf/glifecycle.h"
#include "cpgf/gerrorcode.h"

#include "cpgf/metatraits/gmetaobjectlifemanager_iobject.h"
#include "cpgf/metatraits/gmetatraitsparam.h"

#include <vector>


#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4996) // warning C4996: 'mbstowcs': This function or variable may be unsafe. Consider using mbstowcs_s instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.
#endif

using namespace std;


namespace cpgf {

G_GUARD_LIBRARY_LIFE

GScriptCoreService * doBindScriptCoreService(GScriptObject * scriptObject, const char * bindName, IScriptLibraryLoader * libraryLoader);

namespace bind_internal {


//*********************************************
// Declarations
//*********************************************

const int ValueMatchRank_Unknown = 0; // such as 2 or more dimensions pointer
const int ValueMatchRank_Convert = 50000;
const int ValueMatchRank_ConvertBwtweenFamily = 51000;
const int ValueMatchRank_ConvertWithinFamily = 52000;
const int ValueMatchRank_ConvertWithinFamilySameSigned = 53000;
const int ValueMatchRank_Implicit_Begin = 70000;
const int ValueMatchRank_Implicit_WideStringToString = ValueMatchRank_Implicit_Begin + 0;
const int ValueMatchRank_Implicit_StringToWideString = ValueMatchRank_Implicit_Begin + 1;
const int ValueMatchRank_Implicit_SharedPointerToRaw = ValueMatchRank_Implicit_Begin + 2;
const int ValueMatchRank_Implicit_CastToBase = ValueMatchRank_Implicit_Begin + 3;
const int ValueMatchRank_Implicit_CastToDerived = ValueMatchRank_Implicit_Begin + 4;
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


class GClassPool
{
private:
	typedef map<void *, GWeakObjectInstancePointer> InstanceMapType;
	
	typedef vector<GClassGlueDataPointer> ClassMapListType;
	typedef GStringMap<ClassMapListType, GStringMapReuseKey> ClassMapType;

public:
	explicit GClassPool(GBindingContext * context);

	void objectCreated(const GObjectInstancePointer & objectData);
	void objectDestroyed(const GObjectInstance * objectData);
	void classDestroyed(IMetaClass * metaClass);

	GObjectInstancePointer findObjectData(const GVariant & instancecv);

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

	if(metaClass == NULL) {
		return;
	}
	
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


GObjectInstance::GObjectInstance(const GContextPointer & context, const GVariant & instance, const GClassGlueDataPointer & classData, IMetaObjectLifeManager * objectLifeManager, bool allowGC)
	: context(context), instance(instance), classData(classData), objectLifeManager(objectLifeManager), allowGC(allowGC), isSharedPointer(false)
{
	objectLifeManager->retainObject(this->getInstanceAddress());
}

GObjectInstance::~GObjectInstance()
{
	// We don't call getInstanceAddress if it's a shared pointer, since we don't own the pointer, so the pointer may already be freed.
	if(! this->isSharedPointer) {
		this->objectLifeManager->releaseObject(this->getInstanceAddress());
		if(this->isAllowGC()) {
			this->objectLifeManager->freeObject(this->getInstanceAddress(), this->classData->getMetaClass());
		}
	}
	if(! this->context.expired()) {
		this->getContext()->getClassPool()->objectDestroyed(this);
	}
}

void GObjectInstance::setDataStorage(IScriptDataStorage * dataStorage)
{
	this->dataStorage.reset(dataStorage);
}

void * GObjectInstance::getInstanceAddress() const
{
	return objectAddressFromVariant(this->instance);
}

GScriptDataHolder * GObjectInstance::getDataHolder() const
{
	return this->dataHolder.get();
}

GScriptDataHolder * GObjectInstance::requireDataHolder() const
{
	if(! this->dataHolder) {
		this->dataHolder.reset(new GScriptDataHolder());
	}
	return this->dataHolder.get();
}


GObjectGlueData::GObjectGlueData(const GContextPointer & context, const GClassGlueDataPointer & classGlueData, const GVariant & instance,
	const GBindValueFlags & flags, ObjectPointerCV cv)
	: super(gdtObject, context), classGlueData(classGlueData), flags(flags), cv(cv)
{
	GScopedInterface<IMetaObjectLifeManager> objectLifeManager(createObjectLifeManagerForInterface(instance));

	if(! objectLifeManager) {
		objectLifeManager.reset(metaGetItemExtendType(this->getClassData()->getMetaClass(), GExtendTypeCreateFlag_ObjectLifeManager).getObjectLifeManager());
	}
	objectInstance.reset(new GObjectInstance(context, instance, this->getClassData(), objectLifeManager.get(), this->flags.has(bvfAllowGC)));
}

GObjectGlueData::GObjectGlueData(const GContextPointer & context, const GClassGlueDataPointer & classGlueData, const GObjectInstancePointer & objectInstance,
	const GBindValueFlags & flags, ObjectPointerCV cv)
	: super(gdtObject, context), classGlueData(classGlueData), flags(flags), cv(cv), objectInstance(objectInstance)
{
}

GObjectGlueData::~GObjectGlueData()
{
}

void GObjectGlueData::initialize()
{
	GScopedInterface<IMetaScriptWrapper> scriptWrapper(metaGetItemExtendType(this->getClassData()->getMetaClass(), GExtendTypeCreateFlag_ScriptWrapper).getScriptWrapper());
	if(scriptWrapper) {
		if(! this->objectInstance->dataStorage) {
			this->objectInstance->dataStorage.reset(new GScriptDataStorage(GObjectGlueDataPointer(this->shareFromThis())));
		}
		scriptWrapper->setScriptDataStorage(this->getInstanceAddress(), this->objectInstance->dataStorage.get());
	}
}

std::string GMethodGlueData::getName() const
{
	return getMethodNameFromMethodList(this->methodList.get());
}

GScriptDataStorage::GScriptDataStorage(const GObjectGlueDataPointer & object)
	: object(object)
{
}

GScriptDataStorage::~GScriptDataStorage()
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

void GScriptDataHolder::setScriptValue(const char * name, const GScriptValue & value)
{
	GASSERT(value.isScriptFunction());

	this->requireDataMap();

	this->dataMap->insert(MapValueType(name, value.getValue()));
	gdynamic_cast<IScriptFunction *>(fromVariant<IObject *>(value.getValue()))->weaken();
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

void GClassPool::objectCreated(const GObjectInstancePointer & objectData)
{
	// Only store the object data that owns the object (allow gc or it's a shadow object)
	// If don't check for this, things goes messy if we get the first element address in object array, where two kinds of objects share the same address
	if(objectData->isAllowGC() || objectData->getInstance().getType() == vtShadow) {
		void * instance = objectData->getInstanceAddress();
		if(this->instanceMap.find(instance) == instanceMap.end()) {
			this->instanceMap[instance] = GWeakObjectInstancePointer(objectData);
		}
	}
}

void GClassPool::objectDestroyed(const GObjectInstance * objectData)
{
	if(isLibraryLive()) {
		void * instance = objectData->getInstanceAddress();
		this->instanceMap.erase(instance);
	}
}

GObjectInstancePointer GClassPool::findObjectData(const GVariant & instance)
{
	InstanceMapType::iterator it = this->instanceMap.find(objectAddressFromVariant(instance));
	if(it != instanceMap.end() && it->second) {
		GObjectInstancePointer data(it->second.get());
		return data;
	}
	return GObjectInstancePointer();
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

void GBindingContext::bindScriptCoreService(GScriptObject * scriptObject, const char * bindName, IScriptLibraryLoader * libraryLoader)
{
	if(this->scriptCoreService) {
		return;
	}

	this->scriptCoreService.reset(doBindScriptCoreService(scriptObject, bindName, libraryLoader));
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
	this->classPool->objectCreated(data->getObjectInstance());
	return data;
}

GObjectGlueDataPointer GBindingContext::newOrReuseObjectGlueData(const GClassGlueDataPointer & classData, const GVariant & instance,
	const GBindValueFlags & flags, ObjectPointerCV cv)
{
	GObjectInstancePointer objectInstance(this->classPool->findObjectData(instance));

	GObjectGlueDataPointer data;
	if(objectInstance) {
		data.reset(new GObjectGlueData(this->shareFromThis(), classData, objectInstance, flags, cv));
	}
	else {
		data.reset(new GObjectGlueData(this->shareFromThis(), classData, instance, flags, cv));
		data->initialize();
		this->classPool->objectCreated(data->getObjectInstance());
	}

	return data;
}

GMethodGlueDataPointer GBindingContext::newMethodGlueData(const GClassGlueDataPointer & classData,
	IMetaList * methodList)
{
	GMethodGlueDataPointer data(new GMethodGlueData(this->shareFromThis(), classData, methodList));
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

GOperatorGlueDataPointer GBindingContext::newOperatorGlueData(const GObjectGlueDataPointer & objectData, IMetaClass * metaClass, GMetaOpType op)
{
	GOperatorGlueDataPointer operatorData(new GOperatorGlueData(this->shareFromThis(), objectData, metaClass, op));
	return operatorData;
}


ConvertRank::ConvertRank()
{
	this->reset();
}

void ConvertRank::reset()
{
	this->weight = ValueMatchRank_Unknown;
	this->sourceClass.reset();
	this->targetClass.reset();
}

InvokeCallableParam::InvokeCallableParam(size_t paramCount)
	: paramCount(paramCount)
{
	if(this->paramCount > REF_MAX_ARITY) {
		raiseCoreException(Error_ScriptBinding_CallMethodWithTooManyParameters);
	}
}

InvokeCallableParam::~InvokeCallableParam()
{
}


GScriptObjectBase::GScriptObjectBase(const GContextPointer & context, const GScriptConfig & config)
	: super(config), context(context)
{
}

GScriptObjectBase::GScriptObjectBase(const GScriptObjectBase & other)
	: super(other), context(other.context)
{
}

GScriptObjectBase::~GScriptObjectBase()
{
}

IMetaClass * GScriptObjectBase::cloneMetaClass(IMetaClass * metaClass)
{
	IMetaClass * newMetaClass = gdynamic_cast<IMetaClass *>(metaClass->clone());

	this->context->getClassData(metaClass);
	this->context->newClassData(newMetaClass);

	return newMetaClass;
}

IMetaService * GScriptObjectBase::getMetaService() {
	IMetaService * service = this->context->getService();
	service->addReference();
	return service;
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
	else if(type.isReference()) {
		if(type.isReferenceToConst()) {
			return opcvConst;
		}
		else if(type.isReferenceToVolatile()) {
			return opcvVolatile;
		}
		else if(type.isReferenceToConstVolatile()) {
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


ObjectPointerCV getCallableConstness(IMetaCallable * callable)
{
	if(! callable->isExplicitThis()) {
		// normal function
		GMetaType methodType = metaGetItemType(callable);
		if(methodType.isConstFunction()) {
			return opcvConst;
		}
					
		if(methodType.isVolatileFunction()) {
			return opcvVolatile;
		}
					
		if(methodType.isConstVolatileFunction()) {
			return opcvConstVolatile;
		}
	}
	else {
		// "explicit this" function
		GMetaType selfType = metaGetParamType(callable, abstractParameterIndexBase);
		if(selfType.isPointerToConst() || selfType.isReferenceToConst()) {
			return opcvConst;
		}
					
		if(selfType.isPointerToVolatile() || selfType.isReferenceToVolatile()) {
			return opcvVolatile;
		}
					
		if(selfType.isPointerToConstVolatile() || selfType.isReferenceToConstVolatile()) {
			return opcvConstVolatile;
		}
	}

	return opcvNone;
}

bool allowInvokeCallable(const GScriptConfig & config, const GGlueDataPointer & glueData, IMetaCallable * callable)
{
	if(getGlueDataInstance(glueData) != NULL) {
		if(! config.allowAccessStaticMethodViaInstance()) {
			if(callable->isStatic()) {
				return false;
			}
		}
	}
	else {
		if(! callable->isStatic()) {
			return false;
		}
	}
	
	ObjectPointerCV cv = getGlueDataCV(glueData);
	if(cv != opcvNone) {
		ObjectPointerCV methodCV = getCallableConstness(callable);
		if(cv != methodCV) {
			return false;
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


bool isParamImplicitConvert(const ConvertRank & rank)
{
	return rank.weight >= ValueMatchRank_Implicit_Begin && rank.weight < ValueMatchRank_Implicit_End;
}

void rankImplicitConvertForString(ConvertRank * outputRank, const GVariant & sourceData, const GMetaType & targetType)
{
	if(variantIsString(sourceData) && targetType.isWideString()) {
		outputRank->weight = ValueMatchRank_Implicit_StringToWideString;
	}
	else if(variantIsWideString(sourceData) && targetType.isString()) {
		outputRank->weight = ValueMatchRank_Implicit_WideStringToString;
	}
}

void rankImplicitConvertForSharedPointer(ConvertRank * outputRank, const GGlueDataPointer & valueGlueData, const GMetaExtendType & targetExtendType)
{
	IMetaSharedPointerTraits * paramSharedPointerTraits = getGlueDataSharedPointerTraits(valueGlueData);
	if(paramSharedPointerTraits != NULL) {
		GScopedInterface<IMetaSharedPointerTraits> sharedPointerTraits(targetExtendType.getSharedPointerTraits());
		if(! sharedPointerTraits) {
			outputRank->weight = ValueMatchRank_Implicit_SharedPointerToRaw;
		}
	}
}

void rankImplicitConvertForMetaClass(ConvertRank * outputRank, IMetaItem * sourceType, IMetaItem * targetType)
{

	if(sourceType == NULL || targetType == NULL) {
		return;
	}

	if(! metaIsClass(sourceType->getCategory()) || ! metaIsClass(targetType->getCategory())) {
		return;
	}

	IMetaClass * sourceClass = static_cast<IMetaClass *>(sourceType);
	IMetaClass * targetClass = static_cast<IMetaClass *>(targetType);

	if(sourceClass->equals(targetClass)) {
		outputRank->weight = ValueMatchRank_Equal;
	}
	else {
		if(sourceClass->isInheritedFrom(targetClass)) {
			outputRank->weight = ValueMatchRank_Implicit_CastToBase;
		}
		else if(targetClass->isInheritedFrom(sourceClass)) {
			outputRank->weight = ValueMatchRank_Implicit_CastToDerived;
		}
		
		if(outputRank->weight != ValueMatchRank_Unknown) {
			outputRank->sourceClass.reset(sourceClass);
			outputRank->targetClass.reset(targetClass);
		}
	}
}

void rankCallableImplicitConvert(ConvertRank * outputRank, IMetaService * service, IMetaCallable * callable, const InvokeCallableParam * callbackParam, size_t paramIndex, const GMetaType & targetType)
{
	rankImplicitConvertForString(outputRank, getVariantRealValue(callbackParam->params[paramIndex].value.getValue()), targetType);
	
	if(outputRank->weight == ValueMatchRank_Unknown) {
		rankImplicitConvertForSharedPointer(outputRank, callbackParam->params[paramIndex].paramGlueData,
			metaGetParamExtendType(callable, GExtendTypeCreateFlag_SharedPointerTraits, static_cast<uint32_t>(paramIndex)));
	}

	if(outputRank->weight == ValueMatchRank_Unknown) {
		GScopedInterface<IMetaTypedItem> typedItem(getTypedItemFromScriptValue(callbackParam->params[paramIndex].value));
		if(typedItem) {
			GScopedInterface<IMetaTypedItem> protoType(service->findTypedItemByName(targetType.getBaseName()));
			rankImplicitConvertForMetaClass(outputRank, typedItem.get(), protoType.get());
		}
	}
}

int rankFundamental(GVariantType protoType, GVariantType paramType)
{
	if(protoType == paramType) {
		return ValueMatchRank_Equal;
	}

	if(vtIsBoolean(protoType) || vtIsBoolean(paramType)) {
		return ValueMatchRank_Convert;
	}

	if(vtIsInteger(protoType) && vtIsInteger(paramType)) {
		if(vtIsSignedInteger(protoType) && vtIsSignedInteger(paramType)) {
			return ValueMatchRank_ConvertWithinFamilySameSigned;
		}
		if(vtIsUnsignedInteger(protoType) && vtIsUnsignedInteger(paramType)) {
			return ValueMatchRank_ConvertWithinFamilySameSigned;
		}
		return ValueMatchRank_ConvertWithinFamily;
	}

	if(vtIsReal(protoType) && vtIsReal(paramType)) {
		return ValueMatchRank_ConvertWithinFamilySameSigned;
	}

	return ValueMatchRank_ConvertBwtweenFamily;
}

void rankCallableParam(ConvertRank * outputRank, IMetaService * service, IMetaCallable * callable, const InvokeCallableParam * callbackParam, size_t paramIndex)
{
	outputRank->reset();

	GMetaType proto = metaGetParamType(callable, paramIndex);
	GScriptValue::Type type = callbackParam->params[paramIndex].value.getType();
	
	if(type == GScriptValue::typeNull) {
		outputRank->weight = ValueMatchRank_Equal;
		return;
	}
	
	if(proto.isFundamental() && type == GScriptValue::typeFundamental) {
		outputRank->weight = rankFundamental(proto.getVariantType(),
			callbackParam->params[paramIndex].value.toFundamental().getType());
		return;
	}

	if(type == GScriptValue::typeScriptFunction && vtIsInterface(proto.getVariantType())) {
		outputRank->weight = ValueMatchRank_Convert;
		return;
	}

	if(proto.getPointerDimension() > 1) {
		outputRank->weight = ValueMatchRank_Unknown;
		return;
	}

	rankCallableImplicitConvert(outputRank, service, callable, callbackParam, paramIndex, proto);
}

int rankCallable(IMetaService * service, const GObjectGlueDataPointer & objectData, IMetaCallable * callable, const InvokeCallableParam * callbackParam, ConvertRank * paramRanks)
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
	
	ObjectPointerCV cv = getGlueDataCV(objectData);
	ObjectPointerCV methodCV = getCallableConstness(callable);
	if(cv == methodCV) {
		rank += ValueMatchRank_Equal;
	}
	else {
		rank += ValueMatchRank_Convert;
	}

	for(size_t i = 0; i < callbackParam->paramCount; ++i) {
		rankCallableParam(&paramRanks[i], service, callable, callbackParam, i);
		rank += paramRanks[i].weight;

		if(! isParamImplicitConvert(paramRanks[i])) {
			bool ok = !! callable->checkParam(&callbackParam->params[i].value.getValue().refData(), static_cast<uint32_t>(i));
			metaCheckError(callable);
			if(! ok) {
				return -1;
			}
		}
	}

	return rank;
}

bool implicitConvertForMetaClassCast(const ConvertRank & rank, GVariant * v);
bool doConvertForMetaClassCast(const GContextPointer & context, GVariant * v, const GMetaType & targetType, const GGlueDataPointer & valueGlueData)
{
	IMetaClass * sourceClass = getGlueDataMetaClass(valueGlueData);
	if(sourceClass != NULL) {
		GScopedInterface<IMetaClass> targetClass(context->getService()->findClassByName(targetType.getBaseName()));
		if(targetClass) {
			ConvertRank rank;

			rankImplicitConvertForMetaClass(&rank, sourceClass, targetClass.get());
			return implicitConvertForMetaClassCast(rank, v);
		}
	}

	return false;
}

bool implicitConvertForString(const ConvertRank & rank, GVariant * v, GVariant * holder)
{
	switch(rank.weight) {
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

bool implicitConvertForSharedPointer(const GContextPointer & context, const ConvertRank & rank, GVariant * v, const GMetaType & targetType, const GGlueDataPointer & valueGlueData)
{
	switch(rank.weight) {
		case ValueMatchRank_Implicit_SharedPointerToRaw: {
			IMetaSharedPointerTraits * paramSharedPointerTraits = getGlueDataSharedPointerTraits(valueGlueData);
			*v = paramSharedPointerTraits->getPointer(objectAddressFromVariant(*v));
			
			doConvertForMetaClassCast(context, v, targetType, valueGlueData);

			return true;
		}
	}

	return false;
}

bool implicitConvertForMetaClassCast(const ConvertRank & rank, GVariant * v)
{
	switch(rank.weight) {
		case ValueMatchRank_Implicit_CastToBase: {
			*v = metaCastToBase(objectAddressFromVariant(*v), rank.sourceClass.get(), rank.targetClass.get());

			return true;
		}

		case ValueMatchRank_Implicit_CastToDerived: {
			*v = metaCastToDerived(objectAddressFromVariant(*v), rank.sourceClass.get(), rank.targetClass.get());

			return true;
		}
	}

	return false;
}

void implicitConvertCallableParam(const GContextPointer & context, const ConvertRank & rank, GVariant * v, GVariant * holder, const GMetaType & targetType, const GGlueDataPointer & valueGlueData)
{
	if(! implicitConvertForString(rank, v, holder)) {
		if(! implicitConvertForSharedPointer(context, rank, v, targetType, valueGlueData)) {
			implicitConvertForMetaClassCast(rank, v);
		}
	}
}

void doInvokeCallable(const GContextPointer & context, void * instance, IMetaCallable * callable, InvokeCallableParam * callableParam, InvokeCallableResult * result)
{
	result->resultCount = callable->hasResult() ? 1 : 0;

	GVariant holders[REF_MAX_ARITY];

	for(size_t i = 0; i < callableParam->paramCount; ++i) {
		if(isParamImplicitConvert(callableParam->paramRanks[i])) {
			implicitConvertCallableParam(
				context,
				callableParam->paramRanks[i],
				&callableParam->params[i].value.getValue(),
				&holders[i],
				metaGetParamType(callable, i),
				callableParam->params[i].paramGlueData
			);
		}
	}

	const GVariantData * data[REF_MAX_ARITY];
	for(size_t i = 0; i < callableParam->paramCount; ++i) {
		data[i] = &callableParam->params[i].value.getValue().refData();
	}
	callable->executeIndirectly(&result->resultData.refData(), instance, data, static_cast<uint32_t>(callableParam->paramCount));
	metaCheckError(callable);

	for(uint32_t i = 0; i < callableParam->paramCount; ++i) {
		if(callable->isParamTransferOwnership(i)
			&& callableParam->params[i].paramGlueData
			&& callableParam->params[i].paramGlueData->getType() == gdtObject) {
			static_cast<GObjectGlueData *>(callableParam->params[i].paramGlueData.get())->setAllowGC(false);
		}
	}
}


void * doInvokeConstructor(const GContextPointer & context, IMetaService * service, IMetaClass * metaClass, InvokeCallableParam * callableParam)
{
	void * instance = NULL;

	if(callableParam->paramCount == 0 && metaClass->canCreateInstance()) {
		instance = metaClass->createInstance();
	}
	else {
		int maxRankIndex = findAppropriateCallable(service, GObjectGlueDataPointer(),
			makeCallback(metaClass, &IMetaClass::getConstructorAt), metaClass->getConstructorCount(),
			callableParam, FindCallablePredict());

		if(maxRankIndex >= 0) {
			InvokeCallableResult result;
		
			GScopedInterface<IMetaConstructor> constructor(metaClass->getConstructorAt(static_cast<uint32_t>(maxRankIndex)));
			doInvokeCallable(context, NULL, constructor.get(), callableParam, &result);
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

	int maxRankIndex = findAppropriateCallable(context->getService(), objectData,
		makeCallback(methodList.get(), &IMetaList::getAt), methodList->getCount(),
		callableParam, FindCallablePredict());

	if(maxRankIndex >= 0) {
		InvokeCallableResult result;
		GScopedInterface<IMetaCallable> callable(gdynamic_cast<IMetaCallable *>(methodList->getAt(maxRankIndex)));
		doInvokeCallable(context, methodList->getInstanceAt(static_cast<uint32_t>(maxRankIndex)), callable.get(), callableParam, &result);
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

GScriptValue glueDataToScriptValue(const GGlueDataPointer & glueData)
{
	if(glueData) {
		switch(glueData->getType()) {
			case gdtClass: {
				GClassGlueDataPointer classData = sharedStaticCast<GClassGlueData>(glueData);;
				return GScriptValue::fromClass(classData->getMetaClass());
			}

			case gdtObject: {
				GObjectGlueDataPointer objectData = sharedStaticCast<GObjectGlueData>(glueData);
				return GScriptValue::fromObject(objectData->getInstance(), objectData->getClassData()->getMetaClass(), objectData->isAllowGC());
			}

			case gdtRaw: {
				GRawGlueDataPointer rawData = sharedStaticCast<GRawGlueData>(glueData);
				return GScriptValue::fromRaw(rawData->getData());
			}

			case gdtMethod:
			case gdtObjectAndMethod: {
				GMethodGlueDataPointer methodGlueData;
				if(glueData->getType() == gdtMethod) {
					methodGlueData = sharedStaticCast<GMethodGlueData>(glueData);
				}
				else {
					methodGlueData = sharedStaticCast<GObjectAndMethodGlueData>(glueData)->getMethodData();
				}
				if(methodGlueData->getMethodList()->getCount() == 1) {
					GScopedInterface<IMetaItem> metaItem(methodGlueData->getMethodList()->getAt(0));
					return GScriptValue::fromMethod(methodGlueData->getMethodList()->getInstanceAt(0),
						static_cast<IMetaMethod *>(metaItem.get()));
				}
				else {
					return GScriptValue::fromOverloadedMethods(methodGlueData->getMethodList());
				}
			}

			case gdtEnum:
				return GScriptValue::fromEnum(sharedStaticCast<GEnumGlueData>(glueData)->getMetaEnum());

			default:
				break;
		}
	}

	return GScriptValue();
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

void setValueToScriptDataHolder(const GGlueDataPointer & glueData, const char * name, const GScriptValue & value)
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

void doSetValueOnAccessible(const GContextPointer & context, IMetaAccessible * accessible, const GGlueDataPointer & instanceGlueData, GVariant value, const GGlueDataPointer & valueGlueData)
{
	ConvertRank rank;

	GMetaType targetType(metaGetItemType(accessible));
	rankImplicitConvertForSharedPointer(&rank, valueGlueData, metaGetItemExtendType(accessible, GExtendTypeCreateFlag_SharedPointerTraits));
	if(rank.weight != ValueMatchRank_Unknown) {
		implicitConvertForSharedPointer(context, rank, &value, targetType, valueGlueData);
	}
	else {
		doConvertForMetaClassCast(context, &value, targetType, valueGlueData);
	}

	metaSetValue(accessible, getGlueDataInstance(instanceGlueData), value);
}

bool setValueOnNamedMember(const GGlueDataPointer & instanceGlueData, const char * name,
	const GScriptValue & value, const GGlueDataPointer & valueGlueData)
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
			raiseCoreException(Error_ScriptBinding_CantSetDynamicPropertyOnCppObject);
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
					doSetValueOnAccessible(context, data.get(), instanceGlueData, value.getValue(), valueGlueData);
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

InvokeCallableResult doInvokeOperator(const GContextPointer & context, const GObjectGlueDataPointer & objectData, IMetaClass * metaClass, GMetaOpType op, InvokeCallableParam * callableParam)
{
	int maxRankIndex = findAppropriateCallable(context->getService(), objectData,
		makeCallback(metaClass, &IMetaClass::getOperatorAt), metaClass->getOperatorCount(),
		callableParam, OperatorCallablePredict(op));

	if(maxRankIndex >= 0) {
		InvokeCallableResult result;

		GScopedInterface<IMetaOperator> metaOperator(metaClass->getOperatorAt(maxRankIndex));
		doInvokeCallable(context, objectData->getInstanceAddress(), metaOperator.get(), callableParam, &result);
		result.callable.reset(metaOperator.get());
		return result;
	}

	raiseCoreException(Error_ScriptBinding_CantFindMatchedOperator);

	return InvokeCallableResult();
}

IMetaObjectLifeManager * createObjectLifeManagerForInterface(const GVariant & value)
{
	if(vtIsInterface(value.getType())) {
		return metatraits_internal::doCreateObjectLifeManagerForIObject(GTypeConverter<IObject *, IObject *>());
	}
	else {
		return NULL;
	}
}

IMetaList * getMethodListFromMapItem(GMetaMapItem * mapItem, void * instance)
{
	if(mapItem->getType() == mmitMethod) {
		GScopedInterface<IMetaMethod> method(gdynamic_cast<IMetaMethod *>(mapItem->getItem()));
		IMetaList * methodList = createMetaList();
		methodList->add(method.get(), instance);
		return methodList;
	}
	else {
		if(mapItem->getType() == mmitMethodList) {
			IMetaList * newMethodList = gdynamic_cast<IMetaList *>(mapItem->getItem());
			return newMethodList;
		}
	}

	return NULL;
}

std::string getMethodNameFromMethodList(IMetaList * methodList)
{
	if(methodList->getCount() > 0) {
		GScopedInterface<IMetaItem> item(methodList->getAt(0));
		return item->getName();
	}
	else {
		return "";
	}
}


} // namespace bind_internal


} // namespace cpgf



