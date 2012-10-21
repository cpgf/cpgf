#ifndef __GMETAOBJECTLIFEMANAGER_IOBJECT_H
#define __GMETAOBJECTLIFEMANAGER_IOBJECT_H



namespace cpgf {

struct IObject;
struct GMetaTraitsParam;
struct IMetaClass;
struct IMetaObjectLifeManager;

IMetaObjectLifeManager * metaTraitsCreateObjectLifeManager(IObject *, const GMetaTraitsParam & /*param*/);


} // namespace cpgf



#endif
