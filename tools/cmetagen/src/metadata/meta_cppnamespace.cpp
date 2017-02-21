/*@@cpgf@@*/
// Auto generated file, don't modify.

#include "metadata/meta_cppnamespace.h"

using namespace cpgf;

namespace metadata {

void partial_createMetaClass_metagen_CppNamespace(cpgf::GDefineMetaInfo metaInfo)
{
    typedef cpgf::GDefineMetaClass<metagen::CppNamespace > MetaType;
    MetaType meta = MetaType::fromMetaClass(metaInfo.getMetaClass());
    buildMetaClass_metagen_CppNamespace(meta);
}

void partial_createMetaClass_metagen_CppNamespace(cpgf::GDefineMetaInfo metaInfo);
cpgf::GDefineMetaInfo createMetaClass_metagen_CppNamespace()
{
    typedef cpgf::GDefineMetaClass<metagen::CppNamespace > MetaType;
    
    MetaType _d = MetaType::Policy<cpgf::MakePolicy<GMetaRuleDefaultConstructorAbsent > >::declare("CppNamespace");
    cpgf::GDefineMetaInfo meta = _d.getMetaInfo();
    
    partial_createMetaClass_metagen_CppNamespace(meta);
    
    return meta;
}

} // namespace metadata
