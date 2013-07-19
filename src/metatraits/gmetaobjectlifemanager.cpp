#include "cpgf/metatraits/gmetaobjectlifemanager.h"
#include "cpgf/gmetaapi.h"
#include "cpgf/gscopedinterface.h"

namespace cpgf {

namespace {

class GMetaObjectLifeManagerDefault : public IMetaObjectLifeManager
{
	G_INTERFACE_IMPL_OBJECT

public:
	virtual ~GMetaObjectLifeManagerDefault() {}

protected:
	virtual void G_API_CC retainObject(void *) {
	}
	
	virtual void G_API_CC releaseObject(void *) {
	}
	
	virtual void G_API_CC freeObject(void * object, IMetaClass * metaClass) {
		metaClass->destroyInstance(object);
	}

	virtual void G_API_CC returnedFromMethod(void *) {
	}
};

GScopedInterface<IMetaObjectLifeManager> defaultObjectLifeManager;

} // unnamed namespace



namespace metatraits_internal {

IMetaObjectLifeManager * createDefaultObjectLifeManagerFromMetaTraits()
{
	if(! defaultObjectLifeManager) {
		defaultObjectLifeManager.reset(new GMetaObjectLifeManagerDefault());
	}
	defaultObjectLifeManager->addReference();
	return defaultObjectLifeManager.get();
}

} // namespace metatraits_internal


} // namespace cpgf


