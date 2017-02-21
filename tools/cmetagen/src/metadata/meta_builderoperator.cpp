/*@@cpgf@@*/
// Auto generated file, don't modify.

#include "metadata/meta_builderoperator.h"

using namespace cpgf;

namespace metadata {

void partial_createMetaClass_metagen_BuilderOperator(cpgf::GDefineMetaInfo metaInfo)
{
    typedef cpgf::GDefineMetaClass<metagen::BuilderOperator > MetaType;
    MetaType meta = MetaType::fromMetaClass(metaInfo.getMetaClass());
    buildMetaClass_metagen_BuilderOperator(meta);
}

void partial_createMetaClass_metagen_BuilderOperator(cpgf::GDefineMetaInfo metaInfo);
cpgf::GDefineMetaInfo createMetaClass_metagen_BuilderOperator()
{
    typedef cpgf::GDefineMetaClass<metagen::BuilderOperator > MetaType;
    
    MetaType _d = MetaType::Policy<cpgf::MakePolicy<GMetaRuleDefaultConstructorAbsent > >::declare("BuilderOperator");
    cpgf::GDefineMetaInfo meta = _d.getMetaInfo();
    
    partial_createMetaClass_metagen_BuilderOperator(meta);
    
    return meta;
}

} // namespace metadata
