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
const int ValueMatchRank_Implicit_UserConvert = ValueMatchRank_Implicit_Begin + 5;
const int ValueMatchRank_Implicit_End = 80000;
const int ValueMatchRank_Equal = 100000;


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

	GClassGlueDataPointer getOrNewClassData(const GVariant & instance, IMetaClass * metaClass);
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
	: item(nullptr), type(mmitNone), enumIndex(0)
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
		return nullptr;
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
		return nullptr;
	}
	else {
		return &this->itemMap[name];
	}
}

void GMetaMapClass::buildMap(IMetaClass * metaClass)
{
	using namespace std;

	if(metaClass == nullptr) {
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
				metaList->add(metaItem.get(), nullptr);
				metaList->add(method.get(), nullptr);
				this->itemMap[name] = GMetaMapItem(metaList.get());
			}
			else {
				GASSERT(item.getType() == mmitMethodList);

				GScopedInterface<IMetaList> metaList(gdynamic_cast<IMetaList *>(item.getItem()));
				metaList->add(method.get(), nullptr);
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
		this->getBindingContext()->getClassPool()->classDestroyed(this->getMetaClass());
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
	if(! (this->isSharedPointer || vtIsByReference(this->instance.getType()))) {
		this->objectLifeManager->releaseObject(this->getInstanceAddress());
		if(this->isAllowGC()) {
			this->objectLifeManager->freeObject(this->getInstanceAddress(), this->classData->getMetaClass());
		}
	}
	if(! this->context.expired()) {
		this->getBindingContext()->getClassPool()->objectDestroyed(this);
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
		return nullptr;
	}

	GObjectGlueDataPointer obj(this->object);
	IScriptFunction * func = nullptr;
	if(obj->getDataHolder() != nullptr) {
		func = obj->getDataHolder()->getScriptFunction(name);
	}
	if(func == nullptr && obj->getClassData()->getDataHolder() != nullptr) {
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
	
	const GVariant variant = value.getValue();

	// Insert and overwrite the previous function if it exists.
	(*this->dataMap)[name] = variant;

	if(vtIsInteger(variant.getType())) {
		IScriptFunction * func = dynamic_cast<IScriptFunction *>(fromVariant<IObject *>(variant));
		if(func != nullptr) {
			func->weaken();
		}
	}
}

IScriptFunction * GScriptDataHolder::getScriptFunction(const char * name)
{
	if(this->dataMap) {
		MapType::iterator it = this->dataMap->find(name);
		if(it != this->dataMap->end()) {
			if(vtIsInterface(it->second.getType())) {
				IScriptFunction * func = dynamic_cast<IScriptFunction *>(fromVariant<IObject *>(it->second));
				if(func != nullptr) {
					func->addReference();
				}
				return func;
			}
		}
	}
	return nullptr;
}


GClassPool::GClassPool(GBindingContext * context)
	: context(context)
{
}

void GClassPool::objectCreated(const GObjectInstancePointer & objectData)
{
	void * instance = getInstanceHash(objectData->getInstance());
	if(this->instanceMap.find(instance) == instanceMap.end()) {
		this->instanceMap[instance] = GWeakObjectInstancePointer(objectData);
	}
}

void GClassPool::objectDestroyed(const GObjectInstance * objectData)
{
	if(isLibraryLive()) {
		void * instance = getInstanceHash(objectData->getInstance());
		this->instanceMap.erase(instance);
	}
}

GObjectInstancePointer GClassPool::findObjectData(const GVariant & instance)
{
	InstanceMapType::iterator it = this->instanceMap.find(getInstanceHash(instance));
	if(it != instanceMap.end() && it->second) {
		GObjectInstancePointer data(it->second.get());
		return data;
	}
	it = this->instanceMap.find(objectAddressFromVariant(instance));
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
		if(classDataList != nullptr) {
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

	return nullptr;
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

GClassGlueDataPointer GClassPool::getOrNewClassData(const GVariant & instance, IMetaClass * metaClass)
{
	InstanceMapType::iterator instanceIterator = this->instanceMap.find(getInstanceHash(instance));
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
	if(dataPointer != nullptr) {
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
	this->clear();
	this->active = false;
}

typedef vector<GGlueDataWrapper *> TempListType;
void GGlueDataWrapperPool::clear()
{
	this->active = false;

	TempListType tempList(this->wrapperSet.begin(), this->wrapperSet.end());
	this->wrapperSet.clear();

	// delete objects first - as they depend on other bound data
	for(TempListType::iterator it = tempList.begin(); it != tempList.end(); ++it) {
		if((*it)->getData() && (*it)->getData()->getType() == gdtObject) {
			freeGlueDataWrapper(*it);
			*it = nullptr;
		}
	}

	for(TempListType::iterator it = tempList.begin(); it != tempList.end(); ++it) {
		if(*it != nullptr) {
			if((*it)->getData() && (*it)->getData()->getType() != gdtClass) {
				freeGlueDataWrapper(*it);
				*it = nullptr;
			}
		}
	}

	/* We must free class data after other data (especially, after object data).
	   This is because when class data is freed, the meta class maybe freed which object is still using.
	   Though object holds a shared pointer of class data, the class data may free its nested class which the object is using.
	   That will cause memory access error.
	*/
	for(TempListType::iterator it = tempList.begin(); it != tempList.end(); ++it) {
		if(*it != nullptr) {
			freeGlueDataWrapper(*it);
		}
	}
	this->active = true;
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


void G_API_CC GScriptContext::addScriptUserConverter(IScriptUserConverter * converter)
{
	if(! this->scriptUserConverterList) {
		this->scriptUserConverterList.reset(new ScriptUserConverterListType);
	}
	if(this->findConverter(converter) == this->scriptUserConverterList->end()) {
		this->scriptUserConverterList->push_back(ScriptUserConverterType(converter));
	}
}

void G_API_CC GScriptContext::removeScriptUserConverter(IScriptUserConverter * converter)
{
	if(! this->scriptUserConverterList) {
		return;
	}

	ScriptUserConverterListType::iterator it = this->findConverter(converter);
	if(it != this->scriptUserConverterList->end()) {
		this->scriptUserConverterList->erase(it);
	}
}

uint32_t G_API_CC GScriptContext::getScriptUserConverterCount()
{
	if(! this->scriptUserConverterList) {
		return 0;
	}

	return (uint32_t)(this->scriptUserConverterList->size());
}

IScriptUserConverter * G_API_CC GScriptContext::getScriptUserConverterAt(uint32_t index)
{
	if(index >= getScriptUserConverterCount()) {
		return nullptr;
	}

	return this->scriptUserConverterList->at(index).get();
}

GScriptContext::ScriptUserConverterListType::iterator GScriptContext::findConverter(IScriptUserConverter * converter)
{
	for(ScriptUserConverterListType::iterator it = this->scriptUserConverterList->begin();
		it != this->scriptUserConverterList->end();
		++it) {
		if(it->get() == converter) {
			return it;
		}
	}

	return scriptUserConverterList->end();
}

void GScriptContext::setAllowGC(const GVariant & instance, bool allowGC)
{
	GObjectInstancePointer object = bindingContext->findObjectInstance(instance);
	if (object) {
		object->setAllowGC(allowGC);
	} else {
		raiseCoreException(Error_ScriptBinding_CantFindObject);
	}
}

void GScriptContext::bindExternalObjectToClass(void * address, IMetaClass * metaClass) {
	GClassGlueDataPointer classData = bindingContext->getClassData(metaClass);
	this->externalObjects.push_back(
		bindingContext->newObjectGlueData(
			classData,
			createObjectVariantFromPointer(address),
			GBindValueFlags(),
			opcvNone
		)
	);
}

GBindingContext::GBindingContext(IMetaService * service, const GScriptConfig & config)
	: service(service), config(config), scriptContext(new GScriptContext(this))
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

IScriptContext * GBindingContext::borrowScriptContext() const
{
	return this->scriptContext.get();
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

GClassGlueDataPointer GBindingContext::getOrNewClassData(const GVariant & instance, IMetaClass * metaClass)
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

GObjectInstancePointer GBindingContext::findObjectInstance(const GVariant & instance)
{
	return this->classPool->findObjectData(instance);
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


GScriptObjectCache * GBindingContext::getScriptObjectCache() {
	if (!scriptObjectCache) {
		scriptObjectCache.reset(new GScriptObjectCache());
	}
	return scriptObjectCache.get();
}


InvokeCallableParam::InvokeCallableParam(size_t paramCount, IScriptContext * scriptContext)
	:
		params((CallableParamData *)paramsBuffer),
		paramCount(paramCount),
		paramRanks((ConvertRank *)paramRanksBuffer),
		backParamRanks((ConvertRank *)paramRanksBackBuffer),
		scriptContext(scriptContext)
{
	// Use "raw" buffer to hold the object array CallableParamData and ConvertRank.
	// If we use CallableParamData[REF_MAX_ARITY] and ConvertRank[REF_MAX_ARITY], the performance is bad due to the constructors.
	memset(this->paramsBuffer, 0, sizeof(CallableParamData) * paramCount);
	memset(this->paramRanksBuffer, 0, sizeof(ConvertRank) * paramCount);
	memset(this->paramRanksBackBuffer, 0, sizeof(ConvertRank) * paramCount);

	if(this->paramCount > REF_MAX_ARITY) {
		raiseCoreException(Error_ScriptBinding_CallMethodWithTooManyParameters);
	}
}

InvokeCallableParam::~InvokeCallableParam()
{
	for(size_t i = 0; i < this->paramCount; ++i) {
		this->params[i].~CallableParamData();
		this->paramRanks[i].~ConvertRank();
		this->backParamRanks[i].~ConvertRank();
	}
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

IMetaService * GScriptObjectBase::getMetaService()
{
	IMetaService * service = this->context->getService();
	service->addReference();
	return service;
}

IScriptContext * GScriptObjectBase::getContext() const
{
	IScriptContext * scriptContext = this->context->borrowScriptContext();
	scriptContext->addReference();
	return scriptContext;
}

void GScriptObjectBase::doBindCoreService(const char * name, IScriptLibraryLoader * libraryLoader)
{
	this->getBindingContext()->bindScriptCoreService(this, name, libraryLoader);
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
	if(paramSharedPointerTraits != nullptr) {
		GScopedInterface<IMetaSharedPointerTraits> sharedPointerTraits(targetExtendType.getSharedPointerTraits());
		if(! sharedPointerTraits) {
			outputRank->weight = ValueMatchRank_Implicit_SharedPointerToRaw;
		}
	}
}

void rankImplicitConvertForMetaClass(ConvertRank * outputRank, IMetaItem * sourceType, IMetaItem * targetType)
{

	if(sourceType == nullptr || targetType == nullptr) {
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
			outputRank->sourceClass = sourceClass;
			outputRank->targetClass.reset(targetClass);
		}
	}
}

void rankImplicitConvertForUserConvert(ConvertRank * outputRank, IMetaCallable * callable,
	const InvokeCallableParam * callableParam, size_t paramIndex)
{
	uint32_t converterCount = callableParam->scriptContext->getScriptUserConverterCount();
	if(converterCount == 0) {
		return;
	}

	GScriptUserConverterParamData converterData;
	converterData.callable = callable;
	converterData.paramIndex = (uint32_t)paramIndex;
	GScriptValueData scriptValueData = callableParam->params[paramIndex].value.getData();
	GScriptValueDataScopedGuard scriptValueDataGuard(scriptValueData);
	converterData.sourceValue = &scriptValueData;

	for(uint32_t i = 0; i < converterCount; ++i) {
		GSharedInterface<IScriptUserConverter> converter(callableParam->scriptContext->getScriptUserConverterAt(i));
		uint32_t tag = converter->canConvert(&converterData);
		if(tag != 0) {
			outputRank->weight = ValueMatchRank_Implicit_UserConvert;
			outputRank->userConverter = converter.get();
			outputRank->userConverterTag = tag;

			break;
		}
	}
}

void rankCallableImplicitConvert(ConvertRank * outputRank, IMetaService * service,
	IMetaCallable * callable, const InvokeCallableParam * callableParam, size_t paramIndex, const GMetaType & targetType)
{
	rankImplicitConvertForString(outputRank, getVariantRealValue(callableParam->params[paramIndex].value.getValue()), targetType);

	if(outputRank->weight == ValueMatchRank_Unknown) {
		rankImplicitConvertForSharedPointer(outputRank, callableParam->params[paramIndex].paramGlueData,
			metaGetParamExtendType(callable, GExtendTypeCreateFlag_SharedPointerTraits, static_cast<uint32_t>(paramIndex)));
	}

	if(outputRank->weight == ValueMatchRank_Unknown) {
		GScopedInterface<IMetaTypedItem> typedItem(getTypedItemFromScriptValue(callableParam->params[paramIndex].value));
		if(typedItem) {
			GScopedInterface<IMetaTypedItem> protoType(service->findTypedItemByName(targetType.getBaseName()));
			rankImplicitConvertForMetaClass(outputRank, typedItem.get(), protoType.get());
		}
	}

	if(outputRank->weight == ValueMatchRank_Unknown) {
		rankImplicitConvertForUserConvert(outputRank, callable, callableParam, paramIndex);
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

void rankCallableParam(
	ConvertRank * outputRank,
	IMetaService * service,
	IMetaCallable * callable,
	const InvokeCallableParam * callableParam,
	size_t paramIndex
)
{
	GMetaType proto = metaGetParamType(callable, paramIndex);
	GScriptValue::Type type = callableParam->params[paramIndex].value.getType();

	if(type == GScriptValue::typeNull) {
		outputRank->weight = ValueMatchRank_Equal;
		return;
	}

	if(proto.isFundamental() && type == GScriptValue::typeFundamental) {
		outputRank->weight = rankFundamental(proto.getVariantType(),
			callableParam->params[paramIndex].value.toFundamental().getType());
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

	rankCallableImplicitConvert(outputRank, service, callable, callableParam, paramIndex, proto);
}

int rankCallable(
	IMetaService * service,
	const GObjectGlueDataPointer & objectData,
	IMetaCallable * callable,
	const InvokeCallableParam * callableParam,
	ConvertRank * paramRanks
)
{
	if(!! callable->isVariadic()) {
		return 0;
	}

	auto callableParamCount = callable->getParamCount();
	if(callableParamCount < callableParam->paramCount) {
		return -1;
	}

	if(callableParamCount > callableParam->paramCount + callable->getDefaultParamCount()) {
		return -1;
	}

	int rank = 1;

	ObjectPointerCV cv = getGlueDataCV(objectData);
	ObjectPointerCV methodCV = getCallableConstness(callable);
	if(cv == methodCV) {
		rank += ValueMatchRank_Equal;
	}
	else {
		if(cv != opcvNone) {
			return -1;
		}
		rank += ValueMatchRank_Convert;
	}

	for(size_t i = 0; i < callableParam->paramCount; ++i) {
		rankCallableParam(&paramRanks[i], service, callable, callableParam, i);
		rank += paramRanks[i].weight;

		if(! isParamImplicitConvert(paramRanks[i])) {
			bool ok = !! callable->checkParam(&callableParam->params[i].value.getValue().refData(), static_cast<uint32_t>(i));
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
	if(sourceClass != nullptr) {
		GScopedInterface<IMetaClass> targetClass(context->getService()->findClassByName(targetType.getBaseName()));
		if(targetClass) {
			ConvertRank rank = ConvertRank();

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
			*v = metaCastToBase(objectAddressFromVariant(*v), rank.sourceClass, rank.targetClass.get());

			return true;
		}

		case ValueMatchRank_Implicit_CastToDerived: {
			*v = metaCastToDerived(objectAddressFromVariant(*v), rank.sourceClass, rank.targetClass.get());

			return true;
		}
	}

	return false;
}

bool implicitConvertForUserConvert(const ConvertRank & rank, GVariant * v,
	IMetaCallable * callable, InvokeCallableParam * callableParam, size_t paramIndex)
{
	if(rank.weight != ValueMatchRank_Implicit_UserConvert) {
		return false;
	}

	// below block is copied from function rankImplicitConvertForUserConvert
	// we can't extract it to a functio because we need GScriptValueDataScopedGuard
	GScriptUserConverterParamData converterData;
	converterData.callable = callable;
	converterData.paramIndex = (uint32_t)paramIndex;
	GScriptValueData scriptValueData = callableParam->params[paramIndex].value.getData();
	GScriptValueDataScopedGuard scriptValueDataGuard(scriptValueData);
	converterData.sourceValue = &scriptValueData;

	rank.userConverter->convert(&v->refData(), &converterData, rank.userConverterTag);

	return true;
}

void implicitConvertCallableParam(const GContextPointer & context, GVariant * holder,
	IMetaCallable * callable, InvokeCallableParam * callableParam, size_t paramIndex)
{
	const ConvertRank & rank = callableParam->paramRanks[paramIndex];
	GMetaType targetType(metaGetParamType(callable, paramIndex));
	const GGlueDataPointer & valueGlueData = callableParam->params[paramIndex].paramGlueData;
	GVariant * v = &callableParam->params[paramIndex].value.getValue();

	if(! implicitConvertForString(rank, v, holder)) {
		if(! implicitConvertForSharedPointer(context, rank, v, targetType, valueGlueData)) {
			if(! implicitConvertForMetaClassCast(rank, v)) {
				implicitConvertForUserConvert(rank, v, callable, callableParam, paramIndex);
			}
		}
	}
}

void doInvokeCallable(const GContextPointer & context, void * instance, IMetaCallable * callable, InvokeCallableParam * callableParam, InvokeCallableResult * result)
{
	result->resultCount = callable->hasResult() ? 1 : 0;

	GVariant holder;

	for(size_t i = 0; i < callableParam->paramCount; ++i) {
		if(isParamImplicitConvert(callableParam->paramRanks[i])) {
			implicitConvertCallableParam(
				context,
				&holder,
				callable,
				callableParam,
				i
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
	void * instance = nullptr;

	if(callableParam->paramCount == 0 && metaClass->canCreateInstance()) {
		instance = metaClass->createInstance();
	}
	else {
		const int maxRankIndex = findAppropriateCallable(
			service,
			GObjectGlueDataPointer(),
			[=](const uint32_t index) { return metaClass->getConstructorAt(index); },
			metaClass->getConstructorCount(),
			callableParam,
			[](IMetaCallable *) { return true; }
		);

		if(maxRankIndex >= 0) {
			InvokeCallableResult result;

			GScopedInterface<IMetaConstructor> constructor(metaClass->getConstructorAt(static_cast<uint32_t>(maxRankIndex)));
			doInvokeCallable(context, nullptr, constructor.get(), callableParam, &result);
			instance = objectAddressFromVariant(GVariant(result.resultData));
		}
	}

	return instance;
}

InvokeCallableResult doInvokeMethodList(const GContextPointer & context,
										const GObjectGlueDataPointer & objectData,
										const GMethodGlueDataPointer & methodData, InvokeCallableParam * callableParam)
{
	IMetaList * methodList = methodData->getMethodList();

	const int maxRankIndex = findAppropriateCallable(
		context->getService(),
		objectData,
		[=](const uint32_t index) { return methodList->getAt(index); },
		methodList->getCount(),
		callableParam,
		[](IMetaCallable *) { return true; }
	);

	if(maxRankIndex >= 0) {
		InvokeCallableResult result;
		GScopedInterface<IMetaCallable> callable(gdynamic_cast<IMetaCallable *>(methodList->getAt(maxRankIndex)));
		void * instance = nullptr;
		if(objectData) {
			instance = objectData->getInstanceAddress();
			if(instance != nullptr) {
				auto classData = objectData->getClassData();
				if(classData) {
					GScopedInterface<IMetaClass> callableClass(gdynamic_cast<IMetaClass *>(callable->getOwnerItem()));
					if(classData->getMetaClass() != callableClass.get()) {
						instance = metaCastAny(instance, classData->getMetaClass(), callableClass.get());
					}
				}
			}
		}
		else {
			// This happens if an object method is bound to script as a global function.
			instance = methodList->getInstanceAt(static_cast<uint32_t>(maxRankIndex));
		}
		doInvokeCallable(context, instance, callable.get(), callableParam, &result);
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

struct GScriptValueBindApi : public IScriptValueBindApi
{
	G_INTERFACE_IMPL_OBJECT
public:
	GScriptValueBindApi(GObjectGlueDataPointer glueData) : glueData(glueData) {}

	virtual ~GScriptValueBindApi() {}

	virtual void G_API_CC discardOwnership() {
		glueData->setAllowGC(false);
	}

	virtual bool G_API_CC isOwnershipTransferred() {
		return glueData->isAllowGC();
	}


private:
	GObjectGlueDataPointer glueData;
};

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
				GScopedInterface<IScriptValueBindApi> bindApi(new GScriptValueBindApi(objectData));
				return GScriptValue::fromObject(objectData->getInstance(), objectData->getClassData()->getMetaClass(), bindApi.get());
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
	if(address != nullptr && !outType->isPointer() && outType->baseIsClass()) {
		value = createObjectVariant(address);

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

IMetaClass * selectBoundClass(IMetaClass * currentClass, IMetaClass * derived)
{
	if(derived == nullptr) {
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
	GScriptDataHolder * dataHolder = nullptr;

	if(glueData->getType() == gdtObject) {
		dataHolder = sharedStaticCast<GObjectGlueData>(glueData)->requireDataHolder();
	}
	else {
		if(glueData->getType() == gdtClass) {
			dataHolder = sharedStaticCast<GClassGlueData>(glueData)->requireDataHolder();
		}
	}

	if(dataHolder != nullptr) {
		dataHolder->setScriptValue(name, value);
	}
}

void doSetValueOnAccessible(const GContextPointer & context, IMetaAccessible * accessible, const GGlueDataPointer & instanceGlueData, GVariant value, const GGlueDataPointer & valueGlueData)
{
	ConvertRank rank = ConvertRank();

	GMetaType targetType(metaGetItemType(accessible));
	rankImplicitConvertForSharedPointer(&rank, valueGlueData, metaGetItemExtendType(accessible, GExtendTypeCreateFlag_SharedPointerTraits));
	if(rank.weight != ValueMatchRank_Unknown) {
		implicitConvertForSharedPointer(context, rank, &value, targetType, valueGlueData);
	}
	else {
		doConvertForMetaClassCast(context, &value, targetType, valueGlueData);
	}

	metaSetValue(accessible, getGlueDataInstanceAddress(instanceGlueData), value);
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

	const GScriptConfig & config = classData->getBindingContext()->getConfig();
	GContextPointer context = classData->getBindingContext();

	GMetaClassTraveller traveller(classData->getMetaClass(), getGlueDataInstanceAddress(instanceGlueData));

	void * instance = nullptr;

	for(;;) {
		GScopedInterface<IMetaClass> metaClass(traveller.next(&instance));
		if(!metaClass) {
			break;
		}

		GMetaMapClass * mapClass = context->getClassData(metaClass.get())->getClassMap();
		if(! mapClass) {
			continue;
		}

		GMetaMapItem * mapItem = mapClass->findItem(name);
		if(mapItem == nullptr) {
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
				return false;
		}
	}

	// We always set the value to data holder even the meta data is not found.
	// This is useful when a base class has non-public virtual method, and in the derived wrapper class
	// we want to override the virtual method from script.
	setValueToScriptDataHolder(instanceGlueData, name, value);
	return true;
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

GVariant getGlueDataInstance(const GGlueDataPointer & glueData)
{
	if(glueData) {
		if(glueData->getType() == gdtObject) {
			return sharedStaticCast<GObjectGlueData>(glueData)->getInstance();
		}
	}

	return GVariant();
}

void * getGlueDataInstanceAddress(const GGlueDataPointer & glueData)
{
	if(glueData) {
		if(glueData->getType() == gdtObject) {
			return sharedStaticCast<GObjectGlueData>(glueData)->getInstanceAddress();
		}
	}

	return nullptr;
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

	return nullptr;
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

	return nullptr;
}

InvokeCallableResult doInvokeOperator(const GContextPointer & context, const GObjectGlueDataPointer & objectData, IMetaClass * metaClass, GMetaOpType op, InvokeCallableParam * callableParam)
{
	const int maxRankIndex = findAppropriateCallable(
		context->getService(),
		objectData,
		[=](const uint32_t index) { return metaClass->getOperatorAt(index); },
		metaClass->getOperatorCount(),
		callableParam,
		[=](IMetaCallable * t) { return gdynamic_cast<IMetaOperator *>(t)->getOperator() == op; }
	);

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
		return nullptr;
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

	return nullptr;
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



