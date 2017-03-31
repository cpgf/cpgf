#include "../pinclude/gscriptgluedata.h"

namespace cpgf {

namespace bind_internal {

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


} //namespace bind_internal


} //namespace cpgf

