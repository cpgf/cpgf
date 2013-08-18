/*@@cpgf@@*/
// Auto generated file, don't modify.

#include "metadata/meta_builderconstructor.h"

using namespace cpgf;

namespace metadata {

void partial_createMetaClass_metagen_BuilderConstructor(cpgf::GDefineMetaInfo metaInfo)
{
    typedef cpgf::GDefineMetaClass<metagen::BuilderConstructor > MetaType;
    MetaType meta = MetaType::fromMetaClass(metaInfo.getMetaClass());
    buildMetaClass_metagen_BuilderConstructor(meta);
}

void partial_createMetaClass_metagen_BuilderConstructor(cpgf::GDefineMetaInfo metaInfo);
cpgf::GDefineMetaInfo createMetaClass_metagen_BuilderConstructor()
{
    typedef cpgf::GDefineMetaClass<metagen::BuilderConstructor > MetaType;
    
    MetaType _d = MetaType::Policy<cpgf::MakePolicy<GMetaRuleDefaultConstructorAbsent > >::declare("BuilderConstructor");
    cpgf::GDefineMetaInfo meta = _d.getMetaInfo();
    
    partial_createMetaClass_metagen_BuilderConstructor(meta);
    
    return meta;
}

} // namespace metadata
