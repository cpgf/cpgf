#include "../pinclude/gbindgluedata.h"
#include "../pinclude/gbindcommon.h"

#include <vector>

namespace cpgf {

namespace bind_internal {

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

