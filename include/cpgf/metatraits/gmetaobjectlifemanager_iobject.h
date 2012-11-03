#ifndef __GMETAOBJECTLIFEMANAGER_IOBJECT_H
#define __GMETAOBJECTLIFEMANAGER_IOBJECT_H



namespace cpgf {

struct IObject;
struct GMetaTraitsParam;
struct IMetaClass;
struct IMetaObjectLifeManager;

} // namespace cpgf


namespace cpgf_metatraits {

cpgf::IMetaObjectLifeManager * metaTraitsCreateObjectLifeManager(const cpgf::GMetaTraitsParam & /*param*/, cpgf::IObject *);
cpgf::IMetaObjectLifeManager * metaTraitsCreateObjectLifeManager(const cpgf::GMetaTraitsParam & /*param*/, cpgf::IObject **);

} // namespace cpgf_metatraits



#endif
