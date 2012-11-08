// Auto generated file, don't modify.

#include "irrlicht.h"
#include "IFileArchive.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_IFileArchive.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_Global_ifilearchive()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    buildMetaClass_Global_ifilearchive(0, _d);
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_IArchiveLoader()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::io::IArchiveLoader, irr::IReferenceCounted> _nd = GDefineMetaClass<irr::io::IArchiveLoader, irr::IReferenceCounted>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("IArchiveLoader");
        buildMetaClass_IArchiveLoader(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_IFileArchive()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::io::IFileArchive, irr::IReferenceCounted> _nd = GDefineMetaClass<irr::io::IFileArchive, irr::IReferenceCounted>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("IFileArchive");
        buildMetaClass_IFileArchive(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


