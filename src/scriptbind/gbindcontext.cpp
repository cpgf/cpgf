#include "gbindcontext.h"
#include "gbindcommon.h"

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


GBindingPool::GBindingPool(const std::shared_ptr<GBindingContext> & context)
	: context(context)
{
}

GBindingPool::~GBindingPool()
{
}

void GBindingPool::glueDataAdded(const GMethodGlueDataPointer & glueData)
{
	this->methodMap[this->doMakeMethodKey(glueData->getScriptValue())] = GWeakMethodGlueDataPointer(glueData);
}

void GBindingPool::glueDataRemoved(const GMethodGlueDataPointer & glueData)
{
	this->methodMap.erase(this->doMakeMethodKey(glueData->getScriptValue()));
}

GMethodGlueDataPointer GBindingPool::newMethodGlueData(const GScriptValue & scriptValue)
{
	auto key = this->doMakeMethodKey(scriptValue);
	GMethodGlueDataPointer result;
	auto it = this->methodMap.find(key);
	if(it != this->methodMap.end() && ! it->second.expired()) {
		result = it->second.lock();
	}
	if(! result) {
		result = GMethodGlueDataPointer(new GMethodGlueData(this->context.lock(), scriptValue));
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

GBindingPool::ObjectKey GBindingPool::doMakeObjectKey(const GObjectGlueDataPointer & glueData)
{
	return ObjectKey(
		glueData->getClassData() ? glueData->getClassData()->getMetaClass() : nullptr,
		objectAddressFromVariant(glueData->getInstance()),
		glueData->getCV()
	);
}

void GBindingPool::glueDataAdded(const GObjectGlueDataPointer & glueData)
{
	if(! glueData) {
		return;
	}

	this->objectMap[this->doMakeObjectKey(glueData)] = GWeakObjectGlueDataPointer(glueData);
}

void GBindingPool::glueDataRemoved(const GObjectGlueDataPointer & glueData)
{
	if(! glueData) {
		return;
	}

	this->objectMap.erase(this->doMakeObjectKey(glueData));
}

GObjectGlueDataPointer GBindingPool::newObjectGlueData(
		const GClassGlueDataPointer & classData,
		const GObjectInstancePointer & objectInstance,
		const GScriptInstanceCv cv
	)
{
	const ObjectKey key = ObjectKey(
		classData->getMetaClass(),
		objectInstance->getInstanceAddress(),
		cv
	);
	GObjectGlueDataPointer result;
	auto it = this->objectMap.find(key);
	if(it != this->objectMap.end()) {
		result = it->second.lock();
	}
	if(! result) {
		result = GObjectGlueDataPointer(new GObjectGlueData(this->context.lock(), classData, objectInstance, cv));
	}
	return result;
}

void GBindingPool::glueDataAdded(const GObjectAndMethodGlueDataPointer & glueData)
{
	this->glueDataAdded(glueData->getObjectData());
	this->glueDataAdded(glueData->getMethodData());

	const ObjectAndMethodKey key = ObjectAndMethodKey(
		glueData->getObjectData().get(),
		glueData->getMethodData().get()
	);
	this->objectAndMethodMap[key] = GWeakObjectAndMethodGlueDataPointer(glueData);
}

void GBindingPool::glueDataRemoved(const GObjectAndMethodGlueDataPointer & glueData)
{
	this->glueDataRemoved(glueData->getObjectData());
	this->glueDataRemoved(glueData->getMethodData());

	const ObjectAndMethodKey key = ObjectAndMethodKey(
		glueData->getObjectData().get(),
		glueData->getMethodData().get()
	);
	this->objectAndMethodMap.erase(key);
}

GObjectAndMethodGlueDataPointer GBindingPool::newObjectAndMethodGlueData(
		const GObjectGlueDataPointer & objectData,
		const GMethodGlueDataPointer & methodData
	)
{
	const ObjectAndMethodKey key = ObjectAndMethodKey(
		objectData.get(),
		methodData.get()
	);
	GObjectAndMethodGlueDataPointer result;
	auto it = this->objectAndMethodMap.find(key);
	if(it != this->objectAndMethodMap.end()) {
		result = it->second.lock();
	}
	if(! result) {
		result = GObjectAndMethodGlueDataPointer(new GObjectAndMethodGlueData(this->context.lock(), objectData, methodData));
	}
	return result;
}

void GBindingPool::glueDataAdded(const GEnumGlueDataPointer & glueData)
{
	this->enumMap[glueData->getMetaEnum()] = GWeakEnumGlueDataPointer(glueData);
}

void GBindingPool::glueDataRemoved(const GEnumGlueDataPointer & glueData)
{
	this->enumMap.erase(glueData->getMetaEnum());
}

GEnumGlueDataPointer GBindingPool::newEnumGlueData(IMetaEnum * metaEnum)
{
	GEnumGlueDataPointer result;
	auto it = this->enumMap.find(metaEnum);
	if(it != this->enumMap.end()) {
		result = it->second.lock();
	}
	if(! result) {
		result = GEnumGlueDataPointer(new GEnumGlueData(this->context.lock(), metaEnum));
	}
	return result;
}

void GBindingPool::glueDataAdded(const GAccessibleGlueDataPointer & glueData)
{
	const AccessibleKey key = AccessibleKey(objectAddressFromVariant(glueData->getInstance()), glueData->getAccessible());
	this->accessibleMap[key] = GWeakAccessibleGlueDataPointer(glueData);
}

void GBindingPool::glueDataRemoved(const GAccessibleGlueDataPointer & glueData)
{
	const AccessibleKey key = AccessibleKey(objectAddressFromVariant(glueData->getInstance()), glueData->getAccessible());
	this->accessibleMap.erase(key);
}

GAccessibleGlueDataPointer GBindingPool::newAccessibleGlueData(void * instance, IMetaAccessible * accessible)
{
	const AccessibleKey key = AccessibleKey(objectAddressFromVariant(instance), accessible);
	GAccessibleGlueDataPointer result;
	auto it = this->accessibleMap.find(key);
	if(it != this->accessibleMap.end()) {
		result = it->second.lock();
	}
	if(! result) {
		result = GAccessibleGlueDataPointer(new GAccessibleGlueData(this->context.lock(), instance, accessible));
	}
	return result;
}

void GBindingPool::glueDataAdded(const GRawGlueDataPointer & /*glueData*/)
{
}

void GBindingPool::glueDataRemoved(const GRawGlueDataPointer & /*glueData*/)
{
}

GRawGlueDataPointer GBindingPool::newRawGlueData(const GVariant & data)
{
	return GRawGlueDataPointer(new GRawGlueData(this->context.lock(), data));
}

void GBindingPool::glueDataAdded(const GOperatorGlueDataPointer & glueData)
{
	const OperatorKey key = OperatorKey(glueData->getObjectData().get(), glueData->getMetaClass(), glueData->getOp());
	this->operatorMap[key] = GWeakOperatorGlueDataPointer(glueData);
}

void GBindingPool::glueDataRemoved(const GOperatorGlueDataPointer & glueData)
{
	const OperatorKey key = OperatorKey(glueData->getObjectData().get(), glueData->getMetaClass(), glueData->getOp());
	this->operatorMap.erase(key);
}

GOperatorGlueDataPointer GBindingPool::newOperatorGlueData(
		const GObjectGlueDataPointer & objectData,
		IMetaClass * metaClass,
		const GMetaOpType op
	)
{
	const OperatorKey key = OperatorKey(objectData.get(), metaClass, op);
	GOperatorGlueDataPointer result;
	auto it = this->operatorMap.find(key);
	if(it != this->operatorMap.end()) {
		result = it->second.lock();
	}
	if(! result) {
		result = GOperatorGlueDataPointer(new GOperatorGlueData(this->context.lock(), objectData, metaClass, op));
	}

	return result;
}

void GBindingPool::objectInstanceAdded(const GObjectInstancePointer & objectData)
{
	void * instance = objectAddressFromVariant(objectData->getInstance());
	auto it = this->instanceMap.find(instance);
	if(it == this->instanceMap.end() || it->second.expired()) {
		this->instanceMap[instance] = GWeakObjectInstancePointer(objectData);
	}
}

void GBindingPool::objectInstanceDestroyed(const GObjectInstance * objectData)
{
	void * instance = objectAddressFromVariant(objectData->getInstance());
	this->instanceMap.erase(instance);
}

GObjectInstancePointer GBindingPool::findObjectInstance(const GVariant & instance)
{
	auto it = this->instanceMap.find(objectAddressFromVariant(instance));

	if(it != this->instanceMap.end() && ! it->second.expired()) {
		GObjectInstancePointer data(it->second);
		return data;
	}

	return GObjectInstancePointer();
}

GClassGlueDataPointer GBindingPool::getClassData(IMetaClass * metaClass)
{
	auto it = this->classMap.find(metaClass);
	if(it != this->classMap.end() && it->second) {
		return it->second;
	}

	GClassGlueDataPointer result = GClassGlueDataPointer(new GClassGlueData(this->context.lock(), metaClass, this->metaMap.getMetaClassMap(metaClass)));
	this->classMap[metaClass] = result;
	return result;
}

void GBindingPool::classDestroyed(IMetaClass * metaClass)
{
	this->classMap.erase(metaClass);
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
	auto context = this->shared_from_this();
	GObjectInstancePointer objectInstance(this->getBindingPool()->findObjectInstance(instance));

	if(! objectInstance) {
		objectInstance = GObjectInstance::create(context, instance, classData, allowGC);
	}

	return this->getBindingPool()->newObjectGlueData(classData, objectInstance, cv);
}

GMethodGlueDataPointer GBindingContext::newMethodGlueData(const GScriptValue & scriptValue)
{
	return this->getBindingPool()->newMethodGlueData(scriptValue);
}

GEnumGlueDataPointer GBindingContext::newEnumGlueData(IMetaEnum * metaEnum)
{
	return this->getBindingPool()->newEnumGlueData(metaEnum);
}

GAccessibleGlueDataPointer GBindingContext::newAccessibleGlueData(void * instance, IMetaAccessible * accessible)
{
	return this->getBindingPool()->newAccessibleGlueData(instance, accessible);
}

GRawGlueDataPointer GBindingContext::newRawGlueData(const GVariant & data)
{
	return this->getBindingPool()->newRawGlueData(data);
}

GObjectAndMethodGlueDataPointer GBindingContext::newObjectAndMethodGlueData(const GObjectGlueDataPointer & objectData, const GMethodGlueDataPointer & methodData)
{
	return this->getBindingPool()->newObjectAndMethodGlueData(objectData, methodData);
}

GOperatorGlueDataPointer GBindingContext::newOperatorGlueData(const GObjectGlueDataPointer & objectData, IMetaClass * metaClass, GMetaOpType op)
{
	return this->getBindingPool()->newOperatorGlueData(objectData, metaClass, op);
}

GBindingPool * GBindingContext::getBindingPool()
{
	if(! this->bindingPool) {
		this->bindingPool.reset(new GBindingPool(this->shared_from_this()));
	}
	
	return this->bindingPool.get();
}


} //namespace bind_internal

} //namespace cpgf

