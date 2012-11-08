// Auto generated file, don't modify.

#include "irrlicht.h"
#include "IWriteFile.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_IWriteFile.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_Global_iwritefile()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    buildMetaClass_Global_iwritefile(0, _d);
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_IWriteFile()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::io::IWriteFile, irr::IReferenceCounted> _nd = GDefineMetaClass<irr::io::IWriteFile, irr::IReferenceCounted>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("IWriteFile");
        buildMetaClass_IWriteFile(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


