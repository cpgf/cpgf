/*@@cpgf@@*/
// Auto generated file, don't modify.

#include "metadata/meta_cppenum.h"

using namespace cpgf;

namespace metadata {

void partial_createMetaClass_metagen_CppEnumValue(cpgf::GDefineMetaInfo metaInfo)
{
    typedef cpgf::GDefineMetaClass<metagen::CppEnumValue > MetaType;
    MetaType meta = MetaType::fromMetaClass(metaInfo.getMetaClass());
    buildMetaClass_metagen_CppEnumValue(meta);
}

void partial_createMetaClass_metagen_CppEnum(cpgf::GDefineMetaInfo metaInfo)
{
    typedef cpgf::GDefineMetaClass<metagen::CppEnum > MetaType;
    MetaType meta = MetaType::fromMetaClass(metaInfo.getMetaClass());
    buildMetaClass_metagen_CppEnum(meta);
}

void partial_createMetaClass_metagen_CppEnumValue(cpgf::GDefineMetaInfo metaInfo);
cpgf::GDefineMetaInfo createMetaClass_metagen_CppEnumValue()
{
    typedef cpgf::GDefineMetaClass<metagen::CppEnumValue > MetaType;
    
    MetaType _d = MetaType::Policy<cpgf::MakePolicy<GMetaRuleDefaultConstructorAbsent > >::declare("CppEnumValue");
    cpgf::GDefineMetaInfo meta = _d.getMetaInfo();
    
    partial_createMetaClass_metagen_CppEnumValue(meta);
    
    return meta;
}

void partial_createMetaClass_metagen_CppEnum(cpgf::GDefineMetaInfo metaInfo);
cpgf::GDefineMetaInfo createMetaClass_metagen_CppEnum()
{
    typedef cpgf::GDefineMetaClass<metagen::CppEnum > MetaType;
    
    MetaType _d = MetaType::Policy<cpgf::MakePolicy<GMetaRuleDefaultConstructorAbsent > >::declare("CppEnum");
    cpgf::GDefineMetaInfo meta = _d.getMetaInfo();
    
    partial_createMetaClass_metagen_CppEnum(meta);
    
    return meta;
}

} // namespace metadata
