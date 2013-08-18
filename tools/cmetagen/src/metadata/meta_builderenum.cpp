/*@@cpgf@@*/
// Auto generated file, don't modify.

#include "metadata/meta_builderenum.h"

using namespace cpgf;

namespace metadata {

void partial_createMetaClass_metagen_BuilderEnum(cpgf::GDefineMetaInfo metaInfo)
{
    typedef cpgf::GDefineMetaClass<metagen::BuilderEnum > MetaType;
    MetaType meta = MetaType::fromMetaClass(metaInfo.getMetaClass());
    buildMetaClass_metagen_BuilderEnum(meta);
}

void partial_createMetaClass_metagen_BuilderEnum(cpgf::GDefineMetaInfo metaInfo);
cpgf::GDefineMetaInfo createMetaClass_metagen_BuilderEnum()
{
    typedef cpgf::GDefineMetaClass<metagen::BuilderEnum > MetaType;
    
    MetaType _d = MetaType::Policy<cpgf::MakePolicy<GMetaRuleDefaultConstructorAbsent > >::declare("BuilderEnum");
    cpgf::GDefineMetaInfo meta = _d.getMetaInfo();
    
    partial_createMetaClass_metagen_BuilderEnum(meta);
    
    return meta;
}

} // namespace metadata
