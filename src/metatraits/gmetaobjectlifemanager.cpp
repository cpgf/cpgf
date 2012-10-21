#include "cpgf/metatraits/gmetaobjectlifemanager.h"
#include "cpgf/gmetaapi.h"


namespace cpgf {

IMetaObjectLifeManager * metaTraitsCreateObjectLifeManager(const GMetaTraitsParam & /*param*/, ...)
{
	return NULL;
}

void metaTraitsRetainObject(IMetaObjectLifeManager * traits, void * object, IMetaClass * metaClass)
{
	if(traits != NULL) {
		traits->retainObject(object, metaClass);
	}
}

void metaTraitsReleaseObject(IMetaObjectLifeManager * traits, void * object, IMetaClass * metaClass)
{
	if(traits != NULL) {
		traits->releaseObject(object, metaClass);
	}
	else {
		metaClass->destroyInstance(object);
	}
}


} // namespace cpgf


