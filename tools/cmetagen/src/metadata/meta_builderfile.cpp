/*@@cpgf@@*/
// Auto generated file, don't modify.

#include "metadata/meta_builderfile.h"

using namespace cpgf;

namespace metadata {

void partial_createMetaClass_metagen_BuilderFile(cpgf::GDefineMetaInfo metaInfo)
{
    typedef cpgf::GDefineMetaClass<metagen::BuilderFile > MetaType;
    MetaType meta = MetaType::fromMetaClass(metaInfo.getMetaClass());
    buildMetaClass_metagen_BuilderFile(meta);
}

void partial_createMetaClass_metagen_BuilderFile(cpgf::GDefineMetaInfo metaInfo);
cpgf::GDefineMetaInfo createMetaClass_metagen_BuilderFile()
{
    typedef cpgf::GDefineMetaClass<metagen::BuilderFile > MetaType;
    
    MetaType _d = MetaType::Policy<cpgf::MakePolicy<GMetaRuleDefaultConstructorAbsent > >::declare("BuilderFile");
    cpgf::GDefineMetaInfo meta = _d.getMetaInfo();
    
    partial_createMetaClass_metagen_BuilderFile(meta);
    
    return meta;
}

} // namespace metadata
