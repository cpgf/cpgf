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
	virtual void G_API_CC retainObject(void * object, IMetaClass *) {
		static_cast<IObject *>(object)->addReference();
	}
	
	virtual void G_API_CC releaseObject(void * object, IMetaClass *) {
		static_cast<IObject *>(object)->releaseReference();
	}
};


} // unnamed namespace


IMetaObjectLifeManager * metaTraitsCreateObjectLifeManager(IObject *, const GMetaTraitsParam & /*param*/)
{
	return new GMetaObjectLifeManagerIObject();
}



} // namespace cpgf
