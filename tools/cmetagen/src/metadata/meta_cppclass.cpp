/*@@cpgf@@*/
// Auto generated file, don't modify.

#include "metadata/meta_cppclass.h"

using namespace cpgf;

namespace metadata {

void partial_createMetaClass_metagen_BaseClass(cpgf::GDefineMetaInfo metaInfo)
{
    typedef cpgf::GDefineMetaClass<metagen::BaseClass > MetaType;
    MetaType meta = MetaType::fromMetaClass(metaInfo.getMetaClass());
    buildMetaClass_metagen_BaseClass(meta);
}

void partial_createMetaClass_metagen_CppClass(cpgf::GDefineMetaInfo metaInfo)
{
    typedef cpgf::GDefineMetaClass<metagen::CppClass > MetaType;
    MetaType meta = MetaType::fromMetaClass(metaInfo.getMetaClass());
    buildMetaClass_metagen_CppClass(meta);
}

void partial_createMetaClass_metagen_BaseClass(cpgf::GDefineMetaInfo metaInfo);
cpgf::GDefineMetaInfo createMetaClass_metagen_BaseClass()
{
    typedef cpgf::GDefineMetaClass<metagen::BaseClass > MetaType;
    
    MetaType _d = MetaType::Policy<cpgf::MakePolicy<GMetaRuleDefaultConstructorAbsent > >::declare("BaseClass");
    cpgf::GDefineMetaInfo meta = _d.getMetaInfo();
    
    partial_createMetaClass_metagen_BaseClass(meta);
    
    return meta;
}

void partial_createMetaClass_metagen_CppClass(cpgf::GDefineMetaInfo metaInfo);
cpgf::GDefineMetaInfo createMetaClass_metagen_CppClass()
{
    typedef cpgf::GDefineMetaClass<metagen::CppClass > MetaType;
    
    MetaType _d = MetaType::Policy<cpgf::MakePolicy<GMetaRuleDefaultConstructorAbsent > >::declare("CppClass");
    cpgf::GDefineMetaInfo meta = _d.getMetaInfo();
    
    partial_createMetaClass_metagen_CppClass(meta);
    
    return meta;
}

} // namespace metadata
