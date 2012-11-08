// Auto generated file, don't modify.

#include "irrlicht.h"
#include "IFileSystem.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_IFileSystem.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_IFileSystem()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::io::IFileSystem, irr::IReferenceCounted> _nd = GDefineMetaClass<irr::io::IFileSystem, irr::IReferenceCounted>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("IFileSystem");
        buildMetaClass_IFileSystem(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


