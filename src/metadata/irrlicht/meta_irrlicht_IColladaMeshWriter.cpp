// Auto generated file, don't modify.

#include "irrlicht.h"
#include "IColladaMeshWriter.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_IColladaMeshWriter.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_Global_icolladameshwriter()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    buildMetaClass_Global_icolladameshwriter(0, _d);
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_IColladaMeshWriter()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::scene::IColladaMeshWriter, irr::scene::IMeshWriter> _nd = GDefineMetaClass<irr::scene::IColladaMeshWriter, irr::scene::IMeshWriter>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("IColladaMeshWriter");
        buildMetaClass_IColladaMeshWriter(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_IColladaMeshWriterNames()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::scene::IColladaMeshWriterNames, irr::IReferenceCounted> _nd = GDefineMetaClass<irr::scene::IColladaMeshWriterNames, irr::IReferenceCounted>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("IColladaMeshWriterNames");
        buildMetaClass_IColladaMeshWriterNames(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_IColladaMeshWriterProperties()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::scene::IColladaMeshWriterProperties, irr::IReferenceCounted> _nd = GDefineMetaClass<irr::scene::IColladaMeshWriterProperties, irr::IReferenceCounted>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("IColladaMeshWriterProperties");
        buildMetaClass_IColladaMeshWriterProperties(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


