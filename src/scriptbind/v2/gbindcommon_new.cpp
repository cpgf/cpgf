#include "gbindcommon_new.h"


namespace cpgf {

namespace _bind_internal {

GBindingContext::GBindingContext(IMetaService * service, const GScriptConfig & config)
	: service(service), config(config)
{
}

GBindingContext::~GBindingContext()
{
}


GObjectUserData::GObjectUserData(const GContextPointer & context, const GClassUserDataPointer & classUserData, void * instance,
	bool allowGC, ObjectPointerCV cv, ObjectUserDataType dataType)
	: super(udtObject, context), classUserData(classUserData), instance(instance), allowGC(allowGC), cv(cv), dataType(dataType)
{
	switch(dataType) {
		case cudtInterface:
			this->allowGC = false;
			this->interfaceObject.reset(static_cast<IObject *>(instance));
			break;

		default:
			break;
	}

	GScopedInterface<IMetaScriptWrapper> scriptWrapper(metaGetItemExtendType(this->getMetaClass(), GExtendTypeCreateFlag_ScriptWrapper).getScriptWrapper());
	if(scriptWrapper) {
//		scriptWrapper->setScriptDataStorage(instance, this->getDataStorage());
	}
}

GObjectUserData::~GObjectUserData()
{
	if(this->allowGC) {
		this->getMetaClass()->destroyInstance(this->instance);
	}
}



} // namespace _bind_internal


} // namespace cpgf



