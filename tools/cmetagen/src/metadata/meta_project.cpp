/*@@cpgf@@*/
// Auto generated file, don't modify.

#include "meta_project.h"

using namespace cpgf;

namespace metadata {

void partial_createMetaClass_metagen_Project(cpgf::GDefineMetaInfo metaInfo)
{
    typedef cpgf::GDefineMetaClass<metagen::Project > MetaType;
    MetaType meta = MetaType::fromMetaClass(metaInfo.getMetaClass());
    buildMetaClass_metagen_Project(meta);
}

void partial_createMetaClass_metagen_Project(cpgf::GDefineMetaInfo metaInfo);
cpgf::GDefineMetaInfo createMetaClass_metagen_Project()
{
    typedef cpgf::GDefineMetaClass<metagen::Project > MetaType;
    
    MetaType _d = MetaType::Policy<cpgf::MakePolicy<GMetaRuleCopyConstructorAbsent > >::declare("Project");
    cpgf::GDefineMetaInfo meta = _d.getMetaInfo();
    
    partial_createMetaClass_metagen_Project(meta);
    
    return meta;
}

} // namespace metadata
