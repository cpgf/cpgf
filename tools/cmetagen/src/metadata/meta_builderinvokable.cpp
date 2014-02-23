/*@@cpgf@@*/
// Auto generated file, don't modify.

#include "metadata/meta_builderinvokable.h"

using namespace cpgf;

namespace metadata {

void partial_createMetaClass_metagen_BuilderInvokable(cpgf::GDefineMetaInfo metaInfo)
{
    typedef cpgf::GDefineMetaClass<metagen::BuilderInvokable > MetaType;
    MetaType meta = MetaType::fromMetaClass(metaInfo.getMetaClass());
    buildMetaClass_metagen_BuilderInvokable(meta);
}

void partial_createMetaClass_metagen_namespace_builderinvokable(cpgf::GDefineMetaInfo metaInfo)
{
    typedef GDefineMetaNamespace MetaType;
    MetaType meta = MetaType::fromMetaClass(metaInfo.getMetaClass());
    buildMetaClass_metagen_namespace_builderinvokable(meta);
}

void partial_createMetaClass_metagen_BuilderInvokable(cpgf::GDefineMetaInfo metaInfo);
cpgf::GDefineMetaInfo createMetaClass_metagen_BuilderInvokable()
{
    typedef cpgf::GDefineMetaClass<metagen::BuilderInvokable > MetaType;
    
    MetaType _d = MetaType::Policy<cpgf::MakePolicy<GMetaRuleDefaultConstructorAbsent > >::declare("BuilderInvokable");
    cpgf::GDefineMetaInfo meta = _d.getMetaInfo();
    
    partial_createMetaClass_metagen_BuilderInvokable(meta);
    
    return meta;
}

void partial_createMetaClass_metagen_namespace_builderinvokable(cpgf::GDefineMetaInfo metaInfo);
cpgf::GDefineMetaInfo createMetaClass_metagen_namespace_builderinvokable()
{
    typedef GDefineMetaNamespace MetaType;
    
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    cpgf::GDefineMetaInfo meta = _d.getMetaInfo();
    
    partial_createMetaClass_metagen_namespace_builderinvokable(meta);
    
    return meta;
}

} // namespace metadata
