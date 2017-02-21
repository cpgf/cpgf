/*@@cpgf@@*/
// Auto generated file, don't modify.

#include "metadata/meta_buildercontainer.h"

using namespace cpgf;

namespace metadata {

void partial_createMetaClass_metagen_BuilderContainer(cpgf::GDefineMetaInfo metaInfo)
{
    typedef cpgf::GDefineMetaClass<metagen::BuilderContainer > MetaType;
    MetaType meta = MetaType::fromMetaClass(metaInfo.getMetaClass());
    buildMetaClass_metagen_BuilderContainer(meta);
}

void partial_createMetaClass_metagen_BuilderContainer(cpgf::GDefineMetaInfo metaInfo);
cpgf::GDefineMetaInfo createMetaClass_metagen_BuilderContainer()
{
    typedef cpgf::GDefineMetaClass<metagen::BuilderContainer > MetaType;
    
    MetaType _d = MetaType::Policy<cpgf::MakePolicy<GMetaRuleDefaultConstructorAbsent > >::declare("BuilderContainer");
    cpgf::GDefineMetaInfo meta = _d.getMetaInfo();
    
    partial_createMetaClass_metagen_BuilderContainer(meta);
    
    return meta;
}

} // namespace metadata
