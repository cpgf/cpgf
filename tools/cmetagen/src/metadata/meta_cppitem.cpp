/*@@cpgf@@*/
// Auto generated file, don't modify.

#include "metadata/meta_cppitem.h"

using namespace cpgf;

namespace metadata {

void partial_createMetaClass_metagen_namespace_cppitem(cpgf::GDefineMetaInfo metaInfo)
{
    typedef GDefineMetaNamespace MetaType;
    MetaType meta = MetaType::fromMetaClass(metaInfo.getMetaClass());
    buildMetaClass_metagen_namespace_cppitem(meta);
}

void partial_createMetaClass_metagen_CppNamedItem(cpgf::GDefineMetaInfo metaInfo)
{
    typedef cpgf::GDefineMetaClass<metagen::CppNamedItem > MetaType;
    MetaType meta = MetaType::fromMetaClass(metaInfo.getMetaClass());
    buildMetaClass_metagen_CppNamedItem(meta);
}

void partial_createMetaClass_metagen_CppItem(cpgf::GDefineMetaInfo metaInfo)
{
    typedef cpgf::GDefineMetaClass<metagen::CppItem > MetaType;
    MetaType meta = MetaType::fromMetaClass(metaInfo.getMetaClass());
    buildMetaClass_metagen_CppItem(meta);
}

void partial_createMetaClass_metagen_namespace_cppitem(cpgf::GDefineMetaInfo metaInfo);
cpgf::GDefineMetaInfo createMetaClass_metagen_namespace_cppitem()
{
    typedef GDefineMetaNamespace MetaType;
    
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    cpgf::GDefineMetaInfo meta = _d.getMetaInfo();
    
    partial_createMetaClass_metagen_namespace_cppitem(meta);
    
    return meta;
}

void partial_createMetaClass_metagen_CppNamedItem(cpgf::GDefineMetaInfo metaInfo);
cpgf::GDefineMetaInfo createMetaClass_metagen_CppNamedItem()
{
    typedef cpgf::GDefineMetaClass<metagen::CppNamedItem > MetaType;
    
    MetaType _d = MetaType::Policy<cpgf::MakePolicy<GMetaRuleDefaultConstructorAbsent > >::declare("CppNamedItem");
    cpgf::GDefineMetaInfo meta = _d.getMetaInfo();
    
    partial_createMetaClass_metagen_CppNamedItem(meta);
    
    return meta;
}

void partial_createMetaClass_metagen_CppItem(cpgf::GDefineMetaInfo metaInfo);
cpgf::GDefineMetaInfo createMetaClass_metagen_CppItem()
{
    typedef cpgf::GDefineMetaClass<metagen::CppItem > MetaType;
    
    MetaType _d = MetaType::Policy<cpgf::MakePolicy<GMetaRuleDefaultConstructorAbsent > >::declare("CppItem");
    cpgf::GDefineMetaInfo meta = _d.getMetaInfo();
    
    partial_createMetaClass_metagen_CppItem(meta);
    
    return meta;
}

} // namespace metadata
