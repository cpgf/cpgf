#include "cpgf/metatraits/gmetaobjectlifemanager.h"
#include "cpgf/gmetaapi.h"


namespace cpgf {

namespace {

class GMetaObjectLifeManagerDefault : public IMetaObjectLifeManager
{
	G_INTERFACE_IMPL_OBJECT

protected:
	virtual void G_API_CC retainObject(void * object) {
	}
	
	virtual void G_API_CC releaseObject(void * object) {
	}
	
	virtual void G_API_CC freeObject(void * object, IMetaClass * metaClass) {
		metaClass->destroyInstance(object);
	}

	virtual void G_API_CC returnedFromMethod(void * object) {
	}
};

GScopedInterface<IMetaObjectLifeManager> defaultObjectLifeManager;

} // unnamed namespace



IMetaObjectLifeManager * metaTraitsCreateObjectLifeManager(const GMetaTraitsParam & /*param*/, ...)
{
	if(! defaultObjectLifeManager) {
		defaultObjectLifeManager.reset(new GMetaObjectLifeManagerDefault());
	}
	defaultObjectLifeManager->addReference();
	return defaultObjectLifeManager.get();
}


} // namespace cpgf


