/*@@cpgf@@*/
// Auto generated file, don't modify.

#include "metadata/meta_builderdestructor.h"

using namespace cpgf;

namespace metadata {

void partial_createMetaClass_metagen_BuilderDestructor(cpgf::GDefineMetaInfo metaInfo)
{
    typedef cpgf::GDefineMetaClass<metagen::BuilderDestructor > MetaType;
    MetaType meta = MetaType::fromMetaClass(metaInfo.getMetaClass());
    buildMetaClass_metagen_BuilderDestructor(meta);
}

void partial_createMetaClass_metagen_BuilderDestructor(cpgf::GDefineMetaInfo metaInfo);
cpgf::GDefineMetaInfo createMetaClass_metagen_BuilderDestructor()
{
    typedef cpgf::GDefineMetaClass<metagen::BuilderDestructor > MetaType;
    
    MetaType _d = MetaType::Policy<cpgf::MakePolicy<GMetaRuleDefaultConstructorAbsent > >::declare("BuilderDestructor");
    cpgf::GDefineMetaInfo meta = _d.getMetaInfo();
    
    partial_createMetaClass_metagen_BuilderDestructor(meta);
    
    return meta;
}

} // namespace metadata
