/*@@cpgf@@*/
// Auto generated file, don't modify.

#include "metadata/meta_project.h"

using namespace cpgf;

namespace metadata {

void partial_createMetaClass_metagen_Project(cpgf::GDefineMetaInfo metaInfo)
{
    typedef cpgf::GDefineMetaClass<metagen::Project > MetaType;
    MetaType meta = MetaType::fromMetaClass(metaInfo.getMetaClass());
    buildMetaClass_metagen_Project(meta);
}

void partial_createMetaClass_metagen_ProjectVisitor(cpgf::GDefineMetaInfo metaInfo)
{
    typedef cpgf::GDefineMetaClass<metagen::ProjectVisitor > MetaType;
    MetaType meta = MetaType::fromMetaClass(metaInfo.getMetaClass());
    buildMetaClass_metagen_ProjectVisitor(meta);
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

void partial_createMetaClass_metagen_ProjectVisitor(cpgf::GDefineMetaInfo metaInfo);
cpgf::GDefineMetaInfo createMetaClass_metagen_ProjectVisitor()
{
    typedef cpgf::GDefineMetaClass<metagen::ProjectVisitor > MetaType;
    
    MetaType _d = MetaType::Policy<cpgf::MakePolicy<GMetaRuleDefaultConstructorAbsent > >::declare("ProjectVisitor");
    cpgf::GDefineMetaInfo meta = _d.getMetaInfo();
    
    partial_createMetaClass_metagen_ProjectVisitor(meta);
    
    return meta;
}

} // namespace metadata
