/*@@cpgf@@*/
// Auto generated file, don't modify.

#include "metadata/meta_buildernamespace.h"

using namespace cpgf;

namespace metadata {

void partial_createMetaClass_metagen_BuilderNamespace(cpgf::GDefineMetaInfo metaInfo)
{
    typedef cpgf::GDefineMetaClass<metagen::BuilderNamespace > MetaType;
    MetaType meta = MetaType::fromMetaClass(metaInfo.getMetaClass());
    buildMetaClass_metagen_BuilderNamespace(meta);
}

void partial_createMetaClass_metagen_BuilderNamespace(cpgf::GDefineMetaInfo metaInfo);
cpgf::GDefineMetaInfo createMetaClass_metagen_BuilderNamespace()
{
    typedef cpgf::GDefineMetaClass<metagen::BuilderNamespace > MetaType;
    
    MetaType _d = MetaType::Policy<cpgf::MakePolicy<GMetaRuleDefaultConstructorAbsent > >::declare("BuilderNamespace");
    cpgf::GDefineMetaInfo meta = _d.getMetaInfo();
    
    partial_createMetaClass_metagen_BuilderNamespace(meta);
    
    return meta;
}

} // namespace metadata
