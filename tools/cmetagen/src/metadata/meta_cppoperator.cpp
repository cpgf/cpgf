/*@@cpgf@@*/
// Auto generated file, don't modify.

#include "metadata/meta_cppoperator.h"

using namespace cpgf;

namespace metadata {

void partial_createMetaClass_metagen_CppOperator(cpgf::GDefineMetaInfo metaInfo)
{
    typedef cpgf::GDefineMetaClass<metagen::CppOperator > MetaType;
    MetaType meta = MetaType::fromMetaClass(metaInfo.getMetaClass());
    buildMetaClass_metagen_CppOperator(meta);
}

void partial_createMetaClass_metagen_CppOperator(cpgf::GDefineMetaInfo metaInfo);
cpgf::GDefineMetaInfo createMetaClass_metagen_CppOperator()
{
    typedef cpgf::GDefineMetaClass<metagen::CppOperator > MetaType;
    
    MetaType _d = MetaType::Policy<cpgf::MakePolicy<GMetaRuleDefaultConstructorAbsent > >::declare("CppOperator");
    cpgf::GDefineMetaInfo meta = _d.getMetaInfo();
    
    partial_createMetaClass_metagen_CppOperator(meta);
    
    return meta;
}

} // namespace metadata
