/*@@cpgf@@*/
// Auto generated file, don't modify.

#include "metadata/meta_builderclass.h"

using namespace cpgf;

namespace metadata {

void partial_createMetaClass_metagen_BuilderClass(cpgf::GDefineMetaInfo metaInfo)
{
    typedef cpgf::GDefineMetaClass<metagen::BuilderClass > MetaType;
    MetaType meta = MetaType::fromMetaClass(metaInfo.getMetaClass());
    buildMetaClass_metagen_BuilderClass(meta);
}

void partial_createMetaClass_metagen_BuilderClass(cpgf::GDefineMetaInfo metaInfo);
cpgf::GDefineMetaInfo createMetaClass_metagen_BuilderClass()
{
    typedef cpgf::GDefineMetaClass<metagen::BuilderClass > MetaType;
    
    MetaType _d = MetaType::Policy<cpgf::MakePolicy<GMetaRuleDefaultConstructorAbsent > >::declare("BuilderClass");
    cpgf::GDefineMetaInfo meta = _d.getMetaInfo();
    
    partial_createMetaClass_metagen_BuilderClass(meta);
    
    return meta;
}

} // namespace metadata
