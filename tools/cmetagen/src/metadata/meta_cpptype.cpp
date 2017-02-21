/*@@cpgf@@*/
// Auto generated file, don't modify.

#include "metadata/meta_cpptype.h"

using namespace cpgf;

namespace metadata {

void partial_createMetaClass_metagen_CppType(cpgf::GDefineMetaInfo metaInfo)
{
    typedef cpgf::GDefineMetaClass<metagen::CppType > MetaType;
    MetaType meta = MetaType::fromMetaClass(metaInfo.getMetaClass());
    buildMetaClass_metagen_CppType(meta);
}

void partial_createMetaClass_metagen_CppType(cpgf::GDefineMetaInfo metaInfo);
cpgf::GDefineMetaInfo createMetaClass_metagen_CppType()
{
    typedef cpgf::GDefineMetaClass<metagen::CppType > MetaType;
    
    MetaType _d = MetaType::Policy<cpgf::MakePolicy<GMetaRuleDefaultConstructorAbsent > >::declare("CppType");
    cpgf::GDefineMetaInfo meta = _d.getMetaInfo();
    
    partial_createMetaClass_metagen_CppType(meta);
    
    return meta;
}

} // namespace metadata
