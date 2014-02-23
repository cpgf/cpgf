/*@@cpgf@@*/
// Auto generated file, don't modify.

#include "metadata/meta_cppfield.h"

using namespace cpgf;

namespace metadata {

void partial_createMetaClass_metagen_CppField(cpgf::GDefineMetaInfo metaInfo)
{
    typedef cpgf::GDefineMetaClass<metagen::CppField > MetaType;
    MetaType meta = MetaType::fromMetaClass(metaInfo.getMetaClass());
    buildMetaClass_metagen_CppField(meta);
}

void partial_createMetaClass_metagen_CppField(cpgf::GDefineMetaInfo metaInfo);
cpgf::GDefineMetaInfo createMetaClass_metagen_CppField()
{
    typedef cpgf::GDefineMetaClass<metagen::CppField > MetaType;
    
    MetaType _d = MetaType::Policy<cpgf::MakePolicy<GMetaRuleDefaultConstructorAbsent > >::declare("CppField");
    cpgf::GDefineMetaInfo meta = _d.getMetaInfo();
    
    partial_createMetaClass_metagen_CppField(meta);
    
    return meta;
}

} // namespace metadata
