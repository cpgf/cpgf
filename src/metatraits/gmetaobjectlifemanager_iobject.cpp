#include "cpgf/metatraits/gmetaobjectlifemanager_iobject.h"

#include "cpgf/gapiutil.h"
#include "cpgf/gmetaapi.h"


namespace cpgf {

namespace {

class GMetaObjectLifeManagerIObject : public IMetaObjectLifeManager
{
	G_INTERFACE_IMPL_OBJECT

public:
	explicit GMetaObjectLifeManagerIObject(const GTypeConverterCallback & caster) : caster(caster) {}
	virtual ~GMetaObjectLifeManagerIObject() {}

protected:
	virtual void G_API_CC retainObject(void * object) {
		static_cast<IObject *>(this->caster(object))->addReference();
	}
	
	virtual void G_API_CC releaseObject(void * object) {
		static_cast<IObject *>(this->caster(object))->releaseReference();
	}
	
	virtual void G_API_CC freeObject(void * object, IMetaClass *) {
		releaseObject(object);
	}

	virtual void G_API_CC returnedFromMethod(void * object) {
		releaseObject(object);
	}

private:
	GTypeConverterCallback caster;
};


} // unnamed namespace

namespace metatraits_internal {

	IMetaObjectLifeManager * doCreateObjectLifeManagerForIObject(const GTypeConverterCallback & caster)
	{
		return new cpgf::GMetaObjectLifeManagerIObject(caster);
	}

} // namespace metatraits_internal


} // namespace cpgf

