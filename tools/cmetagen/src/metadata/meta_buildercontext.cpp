/*@@cpgf@@*/
// Auto generated file, don't modify.

#include "metadata/meta_buildercontext.h"

using namespace cpgf;

namespace metadata {

void partial_createMetaClass_metagen_BuilderContext(cpgf::GDefineMetaInfo metaInfo)
{
    typedef cpgf::GDefineMetaClass<metagen::BuilderContext > MetaType;
    MetaType meta = MetaType::fromMetaClass(metaInfo.getMetaClass());
    buildMetaClass_metagen_BuilderContext(meta);
}

void partial_createMetaClass_metagen_BuilderContext(cpgf::GDefineMetaInfo metaInfo);
cpgf::GDefineMetaInfo createMetaClass_metagen_BuilderContext()
{
    typedef cpgf::GDefineMetaClass<metagen::BuilderContext > MetaType;
    
    MetaType _d = MetaType::Policy<cpgf::MakePolicy<GMetaRuleCopyConstructorAbsent, GMetaRuleDefaultConstructorAbsent > >::declare("BuilderContext");
    cpgf::GDefineMetaInfo meta = _d.getMetaInfo();
    
    partial_createMetaClass_metagen_BuilderContext(meta);
    
    return meta;
}

} // namespace metadata
