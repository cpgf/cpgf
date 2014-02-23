/*@@cpgf@@*/
// Auto generated file, don't modify.

#include "metadata/meta_cppinvokable.h"

using namespace cpgf;

namespace metadata {

void partial_createMetaClass_metagen_CppInvokable(cpgf::GDefineMetaInfo metaInfo)
{
    typedef cpgf::GDefineMetaClass<metagen::CppInvokable > MetaType;
    MetaType meta = MetaType::fromMetaClass(metaInfo.getMetaClass());
    buildMetaClass_metagen_CppInvokable(meta);
}

void partial_createMetaClass_metagen_CppInvokable(cpgf::GDefineMetaInfo metaInfo);
cpgf::GDefineMetaInfo createMetaClass_metagen_CppInvokable()
{
    typedef cpgf::GDefineMetaClass<metagen::CppInvokable > MetaType;
    
    MetaType _d = MetaType::Policy<cpgf::MakePolicy<GMetaRuleDefaultConstructorAbsent > >::declare("CppInvokable");
    cpgf::GDefineMetaInfo meta = _d.getMetaInfo();
    
    partial_createMetaClass_metagen_CppInvokable(meta);
    
    return meta;
}

} // namespace metadata
