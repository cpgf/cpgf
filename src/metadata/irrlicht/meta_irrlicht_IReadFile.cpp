// Auto generated file, don't modify.

#include "irrlicht.h"
#include "IReadFile.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_IReadFile.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_Global_ireadfile()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    buildMetaClass_Global_ireadfile(0, _d);
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_IReadFile()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::io::IReadFile, irr::IReferenceCounted> _nd = GDefineMetaClass<irr::io::IReadFile, irr::IReferenceCounted>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("IReadFile");
        buildMetaClass_IReadFile(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


