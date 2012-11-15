#include "cpgf/metadata/irrlicht/gmetaobjectlifemanager_irrlicht_ireferencecounted.h"

#include "cpgf/gapiutil.h"
#include "cpgf/gmetaapi.h"

#include "cpgf/metatraits/gmetaobjectlifemanager.h"


namespace cpgf {

namespace irrlicht_internal {

class GMetaObjectLifeManagerIrrlichIReferenceCounted : public cpgf::IMetaObjectLifeManager
{
	G_INTERFACE_IMPL_OBJECT
	
public:
	explicit GMetaObjectLifeManagerIrrlichIReferenceCounted(const GTypeConverterCallback & caster) : caster(caster) {}

protected:
	virtual void G_API_CC retainObject(void * object) {
//		static_cast<irr::IReferenceCounted *>(this->caster(object))->grab();
	}
	
	virtual void G_API_CC releaseObject(void * object) {
//		static_cast<irr::IReferenceCounted *>(this->caster(object))->drop();
	}
	
	virtual void G_API_CC freeObject(void * object, cpgf::IMetaClass *) {
		static_cast<irr::IReferenceCounted *>(this->caster(object))->drop();
	}

	virtual void G_API_CC returnedFromMethod(void * object) {
	}
	
private:
	GTypeConverterCallback caster;
};


IMetaObjectLifeManager * doCreateObjectLifeManagerForIrrReferenceCounted(const GTypeConverterCallback & caster)
{
	return new irrlicht_internal::GMetaObjectLifeManagerIrrlichIReferenceCounted(caster);
}


} // namespace irrlicht_internal

} // namespace cpgf

