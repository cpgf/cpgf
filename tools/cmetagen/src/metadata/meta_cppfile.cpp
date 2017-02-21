/*@@cpgf@@*/
// Auto generated file, don't modify.

#include "metadata/meta_cppfile.h"

using namespace cpgf;

namespace metadata {

void partial_createMetaClass_metagen_CppFile(cpgf::GDefineMetaInfo metaInfo)
{
    typedef cpgf::GDefineMetaClass<metagen::CppFile > MetaType;
    MetaType meta = MetaType::fromMetaClass(metaInfo.getMetaClass());
    buildMetaClass_metagen_CppFile(meta);
}

void partial_createMetaClass_metagen_CppFile(cpgf::GDefineMetaInfo metaInfo);
cpgf::GDefineMetaInfo createMetaClass_metagen_CppFile()
{
    typedef cpgf::GDefineMetaClass<metagen::CppFile > MetaType;
    
    MetaType _d = MetaType::Policy<cpgf::MakePolicy<GMetaRuleDefaultConstructorAbsent > >::declare("CppFile");
    cpgf::GDefineMetaInfo meta = _d.getMetaInfo();
    
    partial_createMetaClass_metagen_CppFile(meta);
    
    return meta;
}

} // namespace metadata
