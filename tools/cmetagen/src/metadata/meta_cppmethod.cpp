/*@@cpgf@@*/
// Auto generated file, don't modify.

#include "metadata/meta_cppmethod.h"

using namespace cpgf;

namespace metadata {

void partial_createMetaClass_metagen_CppMethod(cpgf::GDefineMetaInfo metaInfo)
{
    typedef cpgf::GDefineMetaClass<metagen::CppMethod > MetaType;
    MetaType meta = MetaType::fromMetaClass(metaInfo.getMetaClass());
    buildMetaClass_metagen_CppMethod(meta);
}

void partial_createMetaClass_metagen_CppMethod(cpgf::GDefineMetaInfo metaInfo);
cpgf::GDefineMetaInfo createMetaClass_metagen_CppMethod()
{
    typedef cpgf::GDefineMetaClass<metagen::CppMethod > MetaType;
    
    MetaType _d = MetaType::Policy<cpgf::MakePolicy<GMetaRuleDefaultConstructorAbsent > >::declare("CppMethod");
    cpgf::GDefineMetaInfo meta = _d.getMetaInfo();
    
    partial_createMetaClass_metagen_CppMethod(meta);
    
    return meta;
}

} // namespace metadata
