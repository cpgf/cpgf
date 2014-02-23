/*@@cpgf@@*/
// Auto generated file, don't modify.

#include "metadata/meta_cppdestructor.h"

using namespace cpgf;

namespace metadata {

void partial_createMetaClass_metagen_CppDestructor(cpgf::GDefineMetaInfo metaInfo)
{
    typedef cpgf::GDefineMetaClass<metagen::CppDestructor > MetaType;
    MetaType meta = MetaType::fromMetaClass(metaInfo.getMetaClass());
    buildMetaClass_metagen_CppDestructor(meta);
}

void partial_createMetaClass_metagen_CppDestructor(cpgf::GDefineMetaInfo metaInfo);
cpgf::GDefineMetaInfo createMetaClass_metagen_CppDestructor()
{
    typedef cpgf::GDefineMetaClass<metagen::CppDestructor > MetaType;
    
    MetaType _d = MetaType::Policy<cpgf::MakePolicy<GMetaRuleDefaultConstructorAbsent > >::declare("CppDestructor");
    cpgf::GDefineMetaInfo meta = _d.getMetaInfo();
    
    partial_createMetaClass_metagen_CppDestructor(meta);
    
    return meta;
}

} // namespace metadata
