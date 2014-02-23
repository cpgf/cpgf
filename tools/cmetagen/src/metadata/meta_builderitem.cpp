/*@@cpgf@@*/
// Auto generated file, don't modify.

#include "metadata/meta_builderitem.h"

using namespace cpgf;

namespace metadata {

void partial_createMetaClass_metagen_BuilderItem(cpgf::GDefineMetaInfo metaInfo)
{
    typedef cpgf::GDefineMetaClass<metagen::BuilderItem > MetaType;
    MetaType meta = MetaType::fromMetaClass(metaInfo.getMetaClass());
    buildMetaClass_metagen_BuilderItem(meta);
}

void partial_createMetaClass_metagen_BuilderItem(cpgf::GDefineMetaInfo metaInfo);
cpgf::GDefineMetaInfo createMetaClass_metagen_BuilderItem()
{
    typedef cpgf::GDefineMetaClass<metagen::BuilderItem > MetaType;
    
    MetaType _d = MetaType::Policy<cpgf::MakePolicy<GMetaRuleDefaultConstructorAbsent > >::declare("BuilderItem");
    cpgf::GDefineMetaInfo meta = _d.getMetaInfo();
    
    partial_createMetaClass_metagen_BuilderItem(meta);
    
    return meta;
}

} // namespace metadata
