/*@@cpgf@@*/
// Auto generated file, don't modify.

#include "metadata/meta_cppconstructor.h"

using namespace cpgf;

namespace metadata {

void partial_createMetaClass_metagen_CppConstructor(cpgf::GDefineMetaInfo metaInfo)
{
    typedef cpgf::GDefineMetaClass<metagen::CppConstructor > MetaType;
    MetaType meta = MetaType::fromMetaClass(metaInfo.getMetaClass());
    buildMetaClass_metagen_CppConstructor(meta);
}

void partial_createMetaClass_metagen_CppConstructor(cpgf::GDefineMetaInfo metaInfo);
cpgf::GDefineMetaInfo createMetaClass_metagen_CppConstructor()
{
    typedef cpgf::GDefineMetaClass<metagen::CppConstructor > MetaType;
    
    MetaType _d = MetaType::Policy<cpgf::MakePolicy<GMetaRuleDefaultConstructorAbsent > >::declare("CppConstructor");
    cpgf::GDefineMetaInfo meta = _d.getMetaInfo();
    
    partial_createMetaClass_metagen_CppConstructor(meta);
    
    return meta;
}

} // namespace metadata
