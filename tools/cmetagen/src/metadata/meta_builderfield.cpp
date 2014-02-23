/*@@cpgf@@*/
// Auto generated file, don't modify.

#include "metadata/meta_builderfield.h"

using namespace cpgf;

namespace metadata {

void partial_createMetaClass_metagen_BuilderField(cpgf::GDefineMetaInfo metaInfo)
{
    typedef cpgf::GDefineMetaClass<metagen::BuilderField > MetaType;
    MetaType meta = MetaType::fromMetaClass(metaInfo.getMetaClass());
    buildMetaClass_metagen_BuilderField(meta);
}

void partial_createMetaClass_metagen_BuilderField(cpgf::GDefineMetaInfo metaInfo);
cpgf::GDefineMetaInfo createMetaClass_metagen_BuilderField()
{
    typedef cpgf::GDefineMetaClass<metagen::BuilderField > MetaType;
    
    MetaType _d = MetaType::Policy<cpgf::MakePolicy<GMetaRuleDefaultConstructorAbsent > >::declare("BuilderField");
    cpgf::GDefineMetaInfo meta = _d.getMetaInfo();
    
    partial_createMetaClass_metagen_BuilderField(meta);
    
    return meta;
}

} // namespace metadata
