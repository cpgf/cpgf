#ifndef __GMETAOBJECTLIFEMANAGER_IOBJECT_H
#define __GMETAOBJECTLIFEMANAGER_IOBJECT_H



namespace cpgf {

struct IObject;
struct GMetaTraitsParam;
struct IMetaClass;
struct IMetaObjectLifeManager;

IMetaObjectLifeManager * metaTraitsCreateObjectLifeManager(const GMetaTraitsParam & /*param*/, IObject *);


} // namespace cpgf



#endif
