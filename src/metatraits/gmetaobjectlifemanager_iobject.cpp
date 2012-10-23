#include "cpgf/metatraits/gmetaobjectlifemanager_iobject.h"

#include "cpgf/gapiutil.h"
#include "cpgf/gmetaapi.h"

#include "cpgf/metatraits/gmetaobjectlifemanager.h"


namespace cpgf {

namespace {

class GMetaObjectLifeManagerIObject : public IMetaObjectLifeManager
{
	G_INTERFACE_IMPL_OBJECT

protected:
	virtual void G_API_CC retainObject(void * object) {
		static_cast<IObject *>(object)->addReference();
	}
	
	virtual void G_API_CC releaseObject(void * object) {
		static_cast<IObject *>(object)->releaseReference();
	}
	
	virtual void G_API_CC freeObject(void * object, IMetaClass *) {
		releaseObject(object);
	}

	virtual void G_API_CC returnedFromMethod(void * object) {
		releaseObject(object);
	}
};


} // unnamed namespace


IMetaObjectLifeManager * metaTraitsCreateObjectLifeManager(const GMetaTraitsParam & /*param*/, IObject *)
{
	return new GMetaObjectLifeManagerIObject();
}



} // namespace cpgf
