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
			false,
			GScriptInstanceCv::sicvNone
		)
	);
}


GBindingPool::GBindingPool(const GSharedPointer<GBindingContext> & context)
	: context(context)
{
}

GBindingPool::~GBindingPool()
{
}

void GBindingPool::glueDataAdded(const GMethodGlueDataPointer & glueData)
{
	this->methodGlueDataMap[this->doMakeMethodKey(glueData->getScriptValue())] = GWeakMethodGlueDataPointer(glueData);
}

void GBindingPool::glueDataRemoved(const GMethodGlueDataPointer & glueData)
{
	auto it = this->methodGlueDataMap.find(this->doMakeMethodKey(glueData->getScriptValue()));
	if(it != this->methodGlueDataMap.end()) {
		this->methodGlueDataMap.erase(it);
	}
}

GMethodGlueDataPointer GBindingPool::newMethodGlueData(const GScriptValue & scriptValue)
{
	auto key = this->doMakeMethodKey(scriptValue);
	GMethodGlueDataPointer result;
	auto it = this->methodGlueDataMap.find(key);
	if(it != this->methodGlueDataMap.end() && ! it->second.expired()) {
		result = it->second.get();
	}
	if(! result) {
		result = GMethodGlueDataPointer(new GMethodGlueData(this->context.get(), scriptValue));
	}
	return result;
}

GBindingPool::MethodKey GBindingPool::doMakeMethodKey(const GScriptValue & scriptValue)
{
	if(scriptValue.getType() == GScriptValue::typeMethod) {
		void * instance;
		GScopedInterface<IMetaMethod> method(scriptValue.toMethod(&instance));
		return std::make_tuple(method.get(), instance);
	}
	else { // GScriptValue::typeOverloadedMethods
		GScopedInterface<IMetaList> methodList(scriptValue.toOverloadedMethods());
		return std::make_tuple(methodList.get(), nullptr);
	}
}

void GBindingPool::glueDataAdded(const GObjectGlueDataPointer & glueData)
{
	if(! glueData) {
		return;
	}

	const ObjectKey key = ObjectKey(
		glueData->getClassData() ? glueData->getClassData()->getMetaClass() : nullptr,
		objectAddressFromVariant(glueData->getInstance()),
		glueData->getCV(),
		glueData->isAllowGC()
	);
	this->objectGlueDataMap[key] = GWeakObjectGlueDataPointer(glueData);
}

void GBindingPool::glueDataRemoved(const GObjectGlueDataPointer & glueData)
{
	if(! glueData) {
		return;
	}

	const ObjectKey key = ObjectKey(
		glueData->getClassData() ? glueData->getClassData()->getMetaClass() : nullptr,
		objectAddressFromVariant(glueData->getInstance()),
		glueData->getCV(),
		glueData->isAllowGC()
	);
	auto it = this->objectGlueDataMap.find(key);
	if(it != this->objectGlueDataMap.end()) {
		this->objectGlueDataMap.erase(it);
	}
}

GObjectGlueDataPointer GBindingPool::newObjectGlueData(
		const GClassGlueDataPointer & classData,
		const GObjectInstancePointer & objectInstance,
		const bool allowGC,
		const GScriptInstanceCv cv
	)
{
	const ObjectKey key = ObjectKey(
		classData->getMetaClass(),
		objectAddressFromVariant(objectInstance->getInstanceAddress()),
		cv,
		allowGC
	);
	GObjectGlueDataPointer result;
	auto it = this->objectGlueDataMap.find(key);
	if(it != this->objectGlueDataMap.end()) {
		result = it->second.get();
	}
	if(! result) {
		result = GObjectGlueDataPointer(new GObjectGlueData(this->context.get(), classData, objectInstance, cv));
	}
	return result;
}

void GBindingPool::glueDataAdded(const GObjectAndMethodGlueDataPointer & glueData)
{
	this->glueDataAdded(glueData->getObjectData());
	this->glueDataAdded(glueData->getMethodData());
}

void GBindingPool::glueDataRemoved(const GObjectAndMethodGlueDataPointer & glueData)
{
	this->glueDataRemoved(glueData->getObjectData());
	this->glueDataRemoved(glueData->getMethodData());
}
	
void GBindingPool::objectInstanceAdded(const GObjectInstancePointer & objectData)
{
	void * instance = objectAddressFromVariant(objectData->getInstance());
	if(this->instanceMap.find(instance) == this->instanceMap.end()) {
		this->instanceMap[instance] = GWeakObjectInstancePointer(objectData);
	}
}

void GBindingPool::objectInstanceDestroyed(const GObjectInstance * objectData)
{
	if(isLibraryLive()) {
		void * instance = objectAddressFromVariant(objectData->getInstance());
		this->instanceMap.erase(instance);
	}
}

GObjectInstancePointer GBindingPool::findObjectInstance(const GVariant & instance)
{
	auto it = this->instanceMap.find(objectAddressFromVariant(instance));

	if(it != this->instanceMap.end() && it->second) {
		GObjectInstancePointer data(it->second);
		return data;
	}

	return GObjectInstancePointer();
}

GClassGlueDataPointer GBindingPool::getClassData(IMetaClass * metaClass)
{
	ClassKey key(metaClass);
	auto it = this->classMap.find(key);
	if(it != this->classMap.end() && it->second) {
		return it->second;
	}

	GClassGlueDataPointer result = GClassGlueDataPointer(new GClassGlueData(this->context.get(), metaClass, this->metaMap.getMetaClassMap(metaClass)));
	this->classMap[key] = result;
	return result;
}

void GBindingPool::classDestroyed(IMetaClass * metaClass)
{
	auto it = this->classMap.find(ClassKey(metaClass));
	if(it != this->classMap.end()) {
		this->classMap.erase(it);
	}
}


GBindingContext::GBindingContext(IMetaService * service)
	: service(service), scriptContext(new GScriptContext(this))
{
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

GClassGlueDataPointer GBindingContext::getClassData(IMetaClass * metaClass)
{
	return this->getBindingPool()->getClassData(metaClass);
}

GClassGlueDataPointer GBindingContext::newClassData(IMetaClass * metaClass)
{
	return this->getBindingPool()->getClassData(metaClass);
}

void GBindingContext::classDestroyed(IMetaClass * metaClass)
{
	this->getBindingPool()->classDestroyed(metaClass);
}

GObjectInstancePointer GBindingContext::findObjectInstance(const GVariant & instance)
{
	return this->getBindingPool()->findObjectInstance(instance);
}

GObjectGlueDataPointer GBindingContext::newObjectGlueData(
		const GClassGlueDataPointer & classData,
		const GVariant & instance,
		const bool allowGC,
		const GScriptInstanceCv cv
	)
{
	auto context = this->shareFromThis();
	GObjectInstancePointer objectInstance(this->getBindingPool()->findObjectInstance(instance));

	if(! objectInstance) {
		objectInstance = GObjectInstance::create(context, instance, classData, allowGC);
	}

	return this->getBindingPool()->newObjectGlueData(classData, objectInstance, allowGC, cv);
}

GMethodGlueDataPointer GBindingContext::newMethodGlueData(const GScriptValue & scriptValue)
{
	return this->getBindingPool()->newMethodGlueData(scriptValue);
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

GBindingPool * GBindingContext::getBindingPool()
{
	if(! this->bindingPool) {
		this->bindingPool.reset(new GBindingPool(this->shareFromThis()));
	}
	
	return this->bindingPool.get();
}

GScriptObjectCache * GBindingContext::getScriptObjectCache() {
	if (!scriptObjectCache) {
		scriptObjectCache.reset(new GScriptObjectCache());
	}
	return scriptObjectCache.get();
}


} //namespace bind_internal

} //namespace cpgf

