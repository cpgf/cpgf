#include "gbindcontext.h"
#include "gbindcommon.h"
#include "gbindobjectcache.h"

namespace cpgf {

GScriptCoreService * doBindScriptCoreService(GScriptObject * scriptObject, const char * bindName, IScriptLibraryLoader * libraryLoader);

namespace bind_internal {

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

void GScriptContext::setAllowGC(const GVariantData * instance, gapi_bool allowGC)
{
	GObjectInstancePointer object = bindingContext->findObjectInstance(createVariantFromData(*instance));
	if (object) {
		object->setAllowGC(!! allowGC);
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
			GScriptInstanceCv::sicvNone
		)
	);
}

GBindingContext::GBindingContext(IMetaService * service)
	: service(service), scriptContext(new GScriptContext(this))
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

GObjectGlueDataPointer GBindingContext::newObjectGlueData(
		const GClassGlueDataPointer & classData,
		const GVariant & instance,
		const GBindValueFlags & flags,
		const GScriptInstanceCv cv
	)
{
	GObjectGlueDataPointer data(new GObjectGlueData(this->shareFromThis(), classData, instance, flags, cv));
	data->initialize();
	this->classPool->objectCreated(data->getObjectInstance());
	return data;
}

GObjectGlueDataPointer GBindingContext::newOrReuseObjectGlueData(
		const GClassGlueDataPointer & classData,
		const GVariant & instance,
		const GBindValueFlags & flags,
		const GScriptInstanceCv cv
	)
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

GMethodGlueDataPointer GBindingContext::newMethodGlueData(const GScriptValue & scriptValue)
{
	GMethodGlueDataPointer data(new GMethodGlueData(this->shareFromThis(), scriptValue));
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


GClassPool::GClassPool(GBindingContext * context)
	: context(context)
{
}

void GClassPool::objectCreated(const GObjectInstancePointer & objectData)
{
	void * instance = objectAddressFromVariant(objectData->getInstance());
	if(this->instanceMap.find(instance) == instanceMap.end()) {
		this->instanceMap[instance] = GWeakObjectInstancePointer(objectData);
	}
}

void GClassPool::objectDestroyed(const GObjectInstance * objectData)
{
	if(isLibraryLive()) {
		void * instance = objectAddressFromVariant(objectData->getInstance());
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


} //namespace bind_internal

} //namespace cpgf

