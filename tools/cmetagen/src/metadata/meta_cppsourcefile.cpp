/*@@cpgf@@*/
// Auto generated file, don't modify.

#include "metadata/meta_cppsourcefile.h"

using namespace cpgf;

namespace metadata {

void partial_createMetaClass_metagen_CppSourceFile(cpgf::GDefineMetaInfo metaInfo)
{
    typedef cpgf::GDefineMetaClass<metagen::CppSourceFile > MetaType;
    MetaType meta = MetaType::fromMetaClass(metaInfo.getMetaClass());
    buildMetaClass_metagen_CppSourceFile(meta);
}

void partial_createMetaClass_metagen_CppSourceFile(cpgf::GDefineMetaInfo metaInfo);
cpgf::GDefineMetaInfo createMetaClass_metagen_CppSourceFile()
{
    typedef cpgf::GDefineMetaClass<metagen::CppSourceFile > MetaType;
    
    MetaType _d = MetaType::Policy<cpgf::MakePolicy<GMetaRuleDefaultConstructorAbsent > >::declare("CppSourceFile");
    cpgf::GDefineMetaInfo meta = _d.getMetaInfo();
    
    partial_createMetaClass_metagen_CppSourceFile(meta);
    
    return meta;
}

} // namespace metadata
