/*@@cpgf@@*/
// Auto generated file, don't modify.

#include "metadata/meta_cppcontainer.h"

using namespace cpgf;

namespace metadata {

void partial_createMetaClass_metagen_CppContainer(cpgf::GDefineMetaInfo metaInfo)
{
    typedef cpgf::GDefineMetaClass<metagen::CppContainer > MetaType;
    MetaType meta = MetaType::fromMetaClass(metaInfo.getMetaClass());
    buildMetaClass_metagen_CppContainer(meta);
}

void partial_createMetaClass_metagen_namespace_cppcontainer(cpgf::GDefineMetaInfo metaInfo)
{
    typedef GDefineMetaNamespace MetaType;
    MetaType meta = MetaType::fromMetaClass(metaInfo.getMetaClass());
    buildMetaClass_metagen_namespace_cppcontainer(meta);
}

void partial_createMetaClass_metagen_CppContainer(cpgf::GDefineMetaInfo metaInfo);
cpgf::GDefineMetaInfo createMetaClass_metagen_CppContainer()
{
    typedef cpgf::GDefineMetaClass<metagen::CppContainer > MetaType;
    
    MetaType _d = MetaType::Policy<cpgf::MakePolicy<GMetaRuleDefaultConstructorAbsent > >::declare("CppContainer");
    cpgf::GDefineMetaInfo meta = _d.getMetaInfo();
    
    partial_createMetaClass_metagen_CppContainer(meta);
    
    return meta;
}

void partial_createMetaClass_metagen_namespace_cppcontainer(cpgf::GDefineMetaInfo metaInfo);
cpgf::GDefineMetaInfo createMetaClass_metagen_namespace_cppcontainer()
{
    typedef GDefineMetaNamespace MetaType;
    
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    cpgf::GDefineMetaInfo meta = _d.getMetaInfo();
    
    partial_createMetaClass_metagen_namespace_cppcontainer(meta);
    
    return meta;
}

} // namespace metadata
