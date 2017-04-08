#include "gbindgluedata.h"
#include "gbindcommon.h"

#include "cpgf/gmemorypool.h"

#include <vector>

namespace cpgf {

namespace bind_internal {

GScriptDataStorage::GScriptDataStorage(const GScriptDataHolderPointer & instanceDataHolder, const GScriptDataHolderPointer & classDataHolder)
	: instanceDataHolder(instanceDataHolder), classDataHolder(classDataHolder)
{
}

GScriptDataStorage::~GScriptDataStorage()
{
}

IScriptFunction * G_API_CC GScriptDataStorage::getScriptFunction(const char * name)
{
	const GScriptValue * value = nullptr;
	if(this->instanceDataHolder) {
		value = this->instanceDataHolder->findValue(name);
	}
	if(value == nullptr && this->classDataHolder) {
		value = this->classDataHolder->findValue(name);
	}
	
	if(value != nullptr) {
		if(value->getType() == GScriptValue::typeScriptFunction) {
			return value->toScriptFunction();
		}
	}
	
	return nullptr;
}

void GScriptDataHolder::requireDataMap()
{
	if(! this->dataMap) {
		this->dataMap.reset(new MapType());
	}
}

void GScriptDataHolder::setScriptValue(const char * name, const GScriptValue & value)
{
	this->requireDataMap();
	
	// Overwrite any previous value.
	(*this->dataMap)[name] = value;
}

const GScriptValue * GScriptDataHolder::findValue(const char * name) const
{
	if(this->dataMap) {
		MapType::iterator it = this->dataMap->find(name);
		if(it != this->dataMap->end()) {
			return &it->second;
		}
	}
	return nullptr;
}


void * GGlueData::operator new (const std::size_t size)
{
	return cpgf::GMemoryPool::getInstance()->allocate(size);
}

void * GGlueData::operator new (const std::size_t /*size*/, void * ptr)
{
	return ptr;
}

void GGlueData::operator delete(void * p, size_t size)
{
	cpgf::GMemoryPool::getInstance()->free(p, size);
}

GGlueData::GGlueData(GGlueDataType type, const GContextPointer & context)
	: type(type), context(GWeakContextPointer(context))
{
}

GGlueData::~GGlueData()
{
}

GGlueDataType GGlueData::getType() const
{
	return this->type;
}

GContextPointer GGlueData::getBindingContext() const
{
	return GContextPointer(this->context);
}

bool GGlueData::isValid() const
{
	return ! this->context.expired();
}


const GScriptDataHolderPointer & GClassGlueData::getDataHolder() const
{
	if(! this->dataHolder) {
		this->dataHolder.reset(new GScriptDataHolder());
	}
	return this->dataHolder;
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


GObjectInstance::GObjectInstance(
		const GContextPointer & context,
		const GVariant & instance,
		const GClassGlueDataPointer & classData,
		IMetaObjectLifeManager * objectLifeManager,
		const bool allowGC
	)
	:
		context(context),
		instance(instance),
		classData(classData),
		objectLifeManager(objectLifeManager),
		allowGC(allowGC),
		isSharedPointer(false)
{
	objectLifeManager->retainObject(this->getInstanceAddress());

	GScopedInterface<IMetaScriptWrapper> scriptWrapper(metaGetItemExtendType(this->classData->getMetaClass(), GExtendTypeCreateFlag_ScriptWrapper).getScriptWrapper());
	if(scriptWrapper) {
		this->dataStorage.reset(new GScriptDataStorage(this->getDataHolder(), this->classData->getDataHolder()));
		scriptWrapper->initializeScriptWrapper(this->getInstanceAddress(), this->dataStorage.get(), this->getBindingContext()->borrowScriptContext());
	}
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

void * GObjectInstance::getInstanceAddress() const
{
	return objectAddressFromVariant(this->instance);
}

GContextPointer GObjectInstance::getBindingContext() const
{
	return GContextPointer(this->context);
}


const GScriptDataHolderPointer & GObjectInstance::getDataHolder() const
{
	if(! this->dataHolder) {
		this->dataHolder.reset(new GScriptDataHolder());
	}
	return this->dataHolder;
}


GObjectGlueData::GObjectGlueData(
		const GContextPointer & context,
		const GClassGlueDataPointer & classGlueData,
		const GVariant & instance,
		const bool allowGC,
		const GScriptInstanceCv cv
	)
	:
		super(gdtObject, context),
		classGlueData(classGlueData),
		cv(cv)
{
	GScopedInterface<IMetaObjectLifeManager> objectLifeManager(createObjectLifeManagerForInterface(instance));

	if(! objectLifeManager) {
		objectLifeManager.reset(metaGetItemExtendType(this->getClassData()->getMetaClass(), GExtendTypeCreateFlag_ObjectLifeManager).getObjectLifeManager());
	}
	this->objectInstance.reset(new GObjectInstance(context, instance, this->getClassData(), objectLifeManager.get(), allowGC));

	this->getBindingContext()->getClassPool()->objectCreated(this->objectInstance);
}

GObjectGlueData::GObjectGlueData(
		const GContextPointer & context,
		const GClassGlueDataPointer & classGlueData,
		const GObjectInstancePointer & objectInstance,
		const bool allowGC,
		const GScriptInstanceCv cv
	)
	:
		super(gdtObject, context),
		classGlueData(classGlueData),
		cv(cv),
		objectInstance(objectInstance)
{
}

GObjectGlueData::~GObjectGlueData()
{
}


GGlueDataWrapperPool::GGlueDataWrapperPool()
	: active(true)
{
}

typedef std::vector<GGlueDataWrapper *> TempListType;
GGlueDataWrapperPool::~GGlueDataWrapperPool()
{
	this->clear();
	this->active = false;
}

typedef std::vector<GGlueDataWrapper *> TempListType;
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


} //namespace bind_internal


} //namespace cpgf

