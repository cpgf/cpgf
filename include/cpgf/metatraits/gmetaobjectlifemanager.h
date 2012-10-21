#ifndef __GMETAOBJECTLIFEMANAGER_H
#define __GMETAOBJECTLIFEMANAGER_H

#include "cpgf/gapi.h"


namespace cpgf {

struct GMetaTraitsParam;
struct IMetaClass;

struct IMetaObjectLifeManager : public IObject
{
	virtual void G_API_CC retainObject(void * object, IMetaClass * metaClass) = 0;
	virtual void G_API_CC releaseObject(void * object, IMetaClass * metaClass) = 0;
};


IMetaObjectLifeManager * metaTraitsCreateObjectLifeManager(const GMetaTraitsParam & /*param*/, ...);

void metaTraitsRetainObject(IMetaObjectLifeManager * traits, void * object, IMetaClass * metaClass);
void metaTraitsReleaseObject(IMetaObjectLifeManager * traits, void * object, IMetaClass * metaClass);


} // namespace cpgf



#endif
